#include "home_shortcut.hpp"

#include "app_version.hpp"
#include "atomic_file_replace.hpp"
#include "cover_image_cache.hpp"
#include "nro_shortcut_policy.hpp"

#include <borealis/extern/nanovg/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#ifdef __SWITCH__
#include <dirent.h>
#include <switch.h>
#include <sys/stat.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <jansson.h>
#include <limits>
#include <sstream>
#include <utility>
#include <vector>

namespace opennow::shortcut
{
namespace
{

constexpr const char* kDefaultExecutable =
    "sdmc:/switch/NVStreamNX/NVStreamNX.nro";
constexpr const char* kShortcutTemplate =
    "romfs:/shortcut/NVStreamNXShortcut.nro";
constexpr const char* kDefaultIcon =
    "romfs:/icon/icon.jpg";
constexpr const char* kInstallerRomfsPath =
    "romfs:/shortcut/NVStreamNXForwarderInstaller.nro";
constexpr const char* kInstallerSdPath =
    "sdmc:/switch/NVStreamNX/.shortcut/NVStreamNXForwarderInstaller.nro";
constexpr const char* kInstallerRequestPath =
    "sdmc:/switch/NVStreamNX/forwarder_request.json";
constexpr const char* kInstallerResultPath =
    "sdmc:/switch/NVStreamNX/forwarder_result.json";
constexpr const char* kActiveExecutablePath =
    "sdmc:/switch/NVStreamNX/active_nro_path.txt";
constexpr const char* kShortcutRoot =
    "sdmc:/switch/NVStreamNX Shortcuts";

std::string g_executable_path = kDefaultExecutable;

bool ReadBinaryFile(const std::string& path, std::vector<std::uint8_t>& data)
{
    std::ifstream stream(path, std::ios::binary);
    if (!stream.is_open())
        return false;
    data.assign(
        std::istreambuf_iterator<char>(stream),
        std::istreambuf_iterator<char>());
    return !data.empty();
}

bool WriteBinaryFileAtomically(
    const std::string& path, const std::vector<std::uint8_t>& data)
{
    const std::string temporary = path + ".tmp";
    {
        std::ofstream stream(temporary, std::ios::binary | std::ios::trunc);
        if (!stream.is_open())
            return false;
        stream.write(
            reinterpret_cast<const char*>(data.data()),
            static_cast<std::streamsize>(data.size()));
        if (!stream.good())
            return false;
    }
    return storage::ReplaceWithTemporaryFile(temporary, path);
}

bool WriteTextFileAtomically(const std::string& path, const std::string& data)
{
    return WriteBinaryFileAtomically(
        path, std::vector<std::uint8_t>(data.begin(), data.end()));
}

std::string ReadTextFile(const std::string& path)
{
    std::ifstream stream(path, std::ios::binary);
    if (!stream.is_open())
        return {};
    return {
        std::istreambuf_iterator<char>(stream),
        std::istreambuf_iterator<char>()};
}

void EnsureShortcutDirectories(const std::string& game_directory)
{
#ifdef __SWITCH__
    mkdir("sdmc:/switch", 0777);
    mkdir("sdmc:/switch/NVStreamNX Shortcuts", 0777);
    mkdir(game_directory.c_str(), 0777);
#else
    (void)game_directory;
#endif
}

std::string JsonEscape(std::string_view value)
{
    std::string escaped;
    escaped.reserve(value.size() + 8);
    for (unsigned char ch : value)
    {
        switch (ch)
        {
            case '"': escaped += "\\\""; break;
            case '\\': escaped += "\\\\"; break;
            case '\b': escaped += "\\b"; break;
            case '\f': escaped += "\\f"; break;
            case '\n': escaped += "\\n"; break;
            case '\r': escaped += "\\r"; break;
            case '\t': escaped += "\\t"; break;
            default:
                if (ch >= 0x20)
                    escaped.push_back(static_cast<char>(ch));
                break;
        }
    }
    return escaped;
}

void JpegWriteCallback(void* context, void* data, int size)
{
    if (!context || !data || size <= 0)
        return;
    auto& output = *static_cast<std::vector<std::uint8_t>*>(context);
    const auto* bytes = static_cast<const std::uint8_t*>(data);
    output.insert(output.end(), bytes, bytes + size);
}

std::vector<std::uint8_t> MakeSquareJpeg(const std::string& encoded)
{
    if (encoded.empty() ||
        encoded.size() > static_cast<size_t>(std::numeric_limits<int>::max()))
        return {};

    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* decoded = stbi_load_from_memory(
        reinterpret_cast<const unsigned char*>(encoded.data()),
        static_cast<int>(encoded.size()), &width, &height, &channels, 3);
    if (!decoded || width <= 0 || height <= 0)
    {
        stbi_image_free(decoded);
        return {};
    }

    constexpr int kOutputSize = 256;
    const int crop_size = std::min(width, height);
    const int crop_x = (width - crop_size) / 2;
    const int crop_y = (height - crop_size) / 2;
    std::vector<std::uint8_t> square(kOutputSize * kOutputSize * 3);
    for (int y = 0; y < kOutputSize; ++y)
    {
        const int source_y = crop_y +
            std::min(crop_size - 1, y * crop_size / kOutputSize);
        for (int x = 0; x < kOutputSize; ++x)
        {
            const int source_x = crop_x +
                std::min(crop_size - 1, x * crop_size / kOutputSize);
            const size_t source =
                (static_cast<size_t>(source_y) * width + source_x) * 3;
            const size_t destination =
                (static_cast<size_t>(y) * kOutputSize + x) * 3;
            std::copy_n(decoded + source, 3, square.begin() + destination);
        }
    }
    stbi_image_free(decoded);

    for (int quality : {92, 86, 78, 68})
    {
        std::vector<std::uint8_t> jpeg;
        if (stbi_write_jpg_to_func(
                JpegWriteCallback, &jpeg, kOutputSize, kOutputSize, 3,
                square.data(), quality) &&
            !jpeg.empty() && jpeg.size() < 0x20000)
            return jpeg;
    }
    return {};
}

std::optional<LaunchRequest> ReadManifestArgument(
    const std::vector<std::string>& arguments)
{
    for (const std::string& argument : arguments)
    {
        if (argument.size() < 10 ||
            argument.substr(argument.size() - 10) != ".nvstreamnx")
            continue;
        const std::string manifest = ReadTextFile(argument);
        if (manifest.empty())
            return std::nullopt;
        return Parse(manifest);
    }
    return std::nullopt;
}

#ifdef __SWITCH__
bool IsRegularFile(const std::string& path)
{
    struct stat info {};
    return stat(path.c_str(), &info) == 0 && S_ISREG(info.st_mode);
}

bool IsUsableExecutablePath(const std::string& path)
{
    if (path.size() < 11 || path.size() >= FS_MAX_PATH ||
        path.rfind("sdmc:/", 0) != 0 || path.find_first_of("\r\n") != std::string::npos)
        return false;
    const size_t slash = path.find_last_of('/');
    const std::string name = path.substr(slash == std::string::npos ? 0 : slash + 1);
    if (name.rfind("NVStreamNX", 0) != 0 || name.size() < 5 ||
        name.substr(name.size() - 4) != ".nro")
        return false;
    return IsRegularFile(path);
}

void RepairManifest(
    const std::string& path, RepairResult& result)
{
    ++result.scanned;
    const std::string document = ReadTextFile(path);
    auto request = Parse(document);
    if (!request)
    {
        ++result.failed;
        return;
    }
    if (request->executable_path == g_executable_path)
        return;
    request->executable_path = g_executable_path;
    if (WriteTextFileAtomically(path, Serialize(*request)))
        ++result.repaired;
    else
        ++result.failed;
}

void RepairManifestDirectory(
    const std::string& directory, RepairResult& result, bool recurse)
{
    DIR* handle = opendir(directory.c_str());
    if (!handle)
        return;
    while (dirent* entry = readdir(handle))
    {
        const std::string name = entry->d_name;
        if (name == "." || name == "..")
            continue;
        const std::string path = directory + "/" + name;
        struct stat info {};
        if (stat(path.c_str(), &info) != 0)
            continue;
        if (recurse && S_ISDIR(info.st_mode))
        {
            RepairManifestDirectory(path, result, false);
            continue;
        }
        if (!S_ISREG(info.st_mode) || name.size() < 10 ||
            name.substr(name.size() - 10) != ".nvstreamnx")
            continue;
        RepairManifest(path, result);
    }
    closedir(handle);
}
#endif

} // namespace

void SetExecutablePath(std::string path)
{
#ifdef __SWITCH__
    if (!IsUsableExecutablePath(path))
        return;
#else
    if (path.empty() || path.find('\n') != std::string::npos || path.size() > 1024)
        return;
#endif
    g_executable_path = std::move(path);
    WriteTextFileAtomically(kActiveExecutablePath, g_executable_path + "\n");
}

const std::string& ExecutablePath()
{
    return g_executable_path;
}

RepairResult RepairLegacyShortcutManifests()
{
    RepairResult result;
#ifdef __SWITCH__
    if (IsUsableExecutablePath(g_executable_path))
        RepairManifestDirectory(kShortcutRoot, result, true);
#endif
    return result;
}

std::optional<LaunchRequest> ReadLaunchRequest(int argc, char* argv[])
{
    std::vector<std::string> arguments;
    arguments.reserve(argc > 1 ? static_cast<size_t>(argc - 1) : 0);
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i])
            arguments.emplace_back(argv[i]);
    }
    if (auto manifest = ReadManifestArgument(arguments))
        return manifest;
    return ParseArguments(arguments);
}

std::optional<InstallerResult> ConsumeInstallerResult()
{
    const std::string document = ReadTextFile(kInstallerResultPath);
    if (document.empty())
        return std::nullopt;

    // Consume once even if an interrupted write left malformed JSON. This
    // prevents a stale installer result from appearing on every later boot.
    std::remove(kInstallerResultPath);

    json_error_t json_error {};
    json_t* root = json_loadb(
        document.data(), document.size(), JSON_REJECT_DUPLICATES, &json_error);
    if (!root || !json_is_object(root))
    {
        if (root)
            json_decref(root);
        return std::nullopt;
    }

    InstallerResult result;
    result.success = json_is_true(json_object_get(root, "success"));
    if (json_t* name = json_object_get(root, "name"); json_is_string(name))
        result.title = json_string_value(name);
    if (json_t* error = json_object_get(root, "error"); json_is_string(error))
        result.error = json_string_value(error);
    json_decref(root);
    return result;
}

CreateResult CreateGameShortcut(LaunchRequest request)
{
    CreateResult result;
    result.title = request.title;
    request.executable_path = g_executable_path;
    if (!IsValid(request))
    {
        result.error = "The selected game does not have valid shortcut metadata.";
        return result;
    }

    const std::string stem = SafeFileStem(request.title, request.launch_app_id);
    const std::string directory =
        "sdmc:/switch/NVStreamNX Shortcuts/" + stem + " [" +
        request.launch_app_id + "]";
    EnsureShortcutDirectories(directory);

    std::vector<std::uint8_t> template_nro;
    if (!ReadBinaryFile(kShortcutTemplate, template_nro))
    {
        result.error = "The NVStreamNX shortcut launcher is missing from RomFS.";
        return result;
    }

    std::vector<std::uint8_t> icon;
    try
    {
        icon = MakeSquareJpeg(LoadCachedImageData(request.image_url));
        result.used_game_cover = !icon.empty();
    }
    catch (...)
    {
        icon.clear();
    }
    if (icon.empty() && !ReadBinaryFile(kDefaultIcon, icon))
    {
        result.error = "Could not load the NVStreamNX shortcut icon.";
        return result;
    }

    std::vector<std::uint8_t> shortcut_nro;
    if (!BuildShortcutNro(template_nro, icon, request.title, shortcut_nro))
    {
        result.error = "Could not personalize the shortcut launcher.";
        return result;
    }

    result.nro_path = directory + "/" + stem + ".nro";
    result.manifest_path = directory + "/" + stem + ".nvstreamnx";
    if (!WriteBinaryFileAtomically(result.nro_path, shortcut_nro))
    {
        result.error = "Could not write the shortcut NRO to the SD card.";
        return result;
    }
    if (!WriteTextFileAtomically(result.manifest_path, Serialize(request)))
    {
        result.error = "Could not write the shortcut launch manifest.";
        return result;
    }

    result.success = true;
    return result;
}

bool StartForwarderInstaller(
    const std::string& nro_path, const std::string& title,
    std::string& error)
{
#ifdef __SWITCH__
    if (appletGetAppletType() != AppletType_Application)
    {
        error =
            "Horizon installation requires full-memory/application mode. "
            "Hold R while opening an installed title, then launch NVStreamNX.";
        return false;
    }
    if (nro_path.empty() || nro_path.size() >= FS_MAX_PATH || title.empty() ||
        nro_path.find('\n') != std::string::npos)
    {
        error = "The forwarder target is invalid.";
        return false;
    }

    mkdir("sdmc:/switch/NVStreamNX/.shortcut", 0777);
    std::vector<std::uint8_t> installer;
    if (!ReadBinaryFile(kInstallerRomfsPath, installer))
    {
        error = "The NVStreamNX Forwarder Installer is missing from RomFS.";
        return false;
    }
    if (!WriteBinaryFileAtomically(kInstallerSdPath, installer))
    {
        error = "Could not prepare the NVStreamNX Forwarder Installer on the SD card.";
        return false;
    }

    const std::string arguments = "\"" + nro_path + "\"";
    const std::string request =
        "{\n"
        "  \"version\": 1,\n"
        "  \"name\": \"" + JsonEscape(title) + "\",\n"
        "  \"author\": \"NVStreamNX\",\n"
        "  \"displayVersion\": \"NVStreamNX\",\n"
        "  \"nroPath\": \"" + JsonEscape(nro_path) + "\",\n"
        "  \"returnPath\": \"" + JsonEscape(g_executable_path) + "\",\n"
        "  \"args\": \"" + JsonEscape(arguments) + "\"\n"
        "}\n";
    if (!WriteTextFileAtomically(kInstallerRequestPath, request))
    {
        error = "Could not write the forwarder installation request.";
        return false;
    }

    envSetNextLoad(kInstallerSdPath, kInstallerSdPath);
    brls::Application::quit();
    return true;
#else
    (void)nro_path;
    (void)title;
    error = "Horizon forwarder installation is only available on Nintendo Switch.";
    return false;
#endif
}

} // namespace opennow::shortcut
