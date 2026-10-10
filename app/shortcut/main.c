#include <switch.h>

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#define SWITCHNOW_DEFAULT_NRO "sdmc:/switch/NVStreamNX/NVStreamNX.nro"
#define SWITCHNOW_ACTIVE_NRO "sdmc:/switch/NVStreamNX/active_nro_path.txt"

static int hex_value(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'a' && ch <= 'f')
        return 10 + ch - 'a';
    if (ch >= 'A' && ch <= 'F')
        return 10 + ch - 'A';
    return -1;
}

static bool decode_value(const char* source, char* destination, size_t size)
{
    size_t written = 0;
    while (*source)
    {
        if (written + 1 >= size)
            return false;
        if (*source != '%')
        {
            destination[written++] = *source++;
            continue;
        }
        if (!source[1] || !source[2])
            return false;
        const int high = hex_value(source[1]);
        const int low = hex_value(source[2]);
        if (high < 0 || low < 0)
            return false;
        destination[written++] = (char)((high << 4) | low);
        source += 3;
    }
    destination[written] = '\0';
    return true;
}

static bool make_manifest_path(const char* nro_path, char* output, size_t size)
{
    if (!nro_path || !*nro_path || strlen(nro_path) + 1 > size)
        return false;
    strcpy(output, nro_path);
    char* extension = strrchr(output, '.');
    if (!extension || strcasecmp(extension, ".nro"))
        return false;
    if ((size_t)(extension - output) + strlen(".nvstreamnx") + 1 > size)
        return false;
    strcpy(extension, ".nvstreamnx");
    return true;
}

static bool regular_file(const char* path, time_t* modified)
{
    struct stat info;
    if (!path || stat(path, &info) != 0 || !S_ISREG(info.st_mode))
        return false;
    if (modified)
        *modified = info.st_mtime;
    return true;
}

static bool supported_switch_now_nro(const char* path)
{
    if (!path || strncmp(path, "sdmc:/", 6))
        return false;
    const char* name = strrchr(path, '/');
    name = name ? name + 1 : path;
    const size_t length = strlen(name);
    if (strncmp(name, "NVStreamNX", 9) || length < 13 ||
        strcasecmp(name + length - 4, ".nro"))
        return false;
    if (strstr(name, "Shortcut") || strstr(name, "Updater") ||
        strstr(name, "ForwarderInstaller"))
        return false;
    return true;
}

static bool read_first_line(
    const char* path, char* output, size_t output_size)
{
    FILE* file = fopen(path, "rb");
    if (!file)
        return false;
    const bool read = fgets(output, (int)output_size, file) != NULL;
    fclose(file);
    if (!read)
        return false;
    char* end = strpbrk(output, "\r\n");
    if (end)
        *end = '\0';
    return output[0] != '\0';
}

static bool read_legacy_target(
    const char* manifest_path, char* output, size_t output_size)
{
    FILE* file = fopen(manifest_path, "rb");
    if (!file)
        return false;

    bool found = false;
    char line[4096];
    while (fgets(line, sizeof(line), file))
    {
        static const char prefix[] = "nro_path=";
        if (strncmp(line, prefix, sizeof(prefix) - 1))
            continue;
        char* end = strpbrk(line, "\r\n");
        if (end)
            *end = '\0';
        char decoded[FS_MAX_PATH];
        if (decode_value(line + sizeof(prefix) - 1, decoded, sizeof(decoded)) &&
            supported_switch_now_nro(decoded))
        {
            snprintf(output, output_size, "%s", decoded);
            found = true;
        }
        break;
    }
    fclose(file);
    return found;
}

static void consider_candidate(
    const char* path, char* best, size_t best_size, time_t* best_modified)
{
    time_t modified = 0;
    if (!supported_switch_now_nro(path) || !regular_file(path, &modified) ||
        modified < *best_modified)
        return;
    snprintf(best, best_size, "%s", path);
    *best_modified = modified;
}

static void scan_directory(
    const char* directory, char* best, size_t best_size,
    time_t* best_modified)
{
    DIR* handle = opendir(directory);
    if (!handle)
        return;
    struct dirent* entry = NULL;
    while ((entry = readdir(handle)) != NULL)
    {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;
        char path[FS_MAX_PATH];
        const int length = snprintf(
            path, sizeof(path), "%s/%s", directory, entry->d_name);
        if (length < 0 || (size_t)length >= sizeof(path))
            continue;
        consider_candidate(path, best, best_size, best_modified);
    }
    closedir(handle);
}

static bool resolve_target_path(
    const char* manifest_path, char* output, size_t output_size)
{
    char active[FS_MAX_PATH];
    if (read_first_line(SWITCHNOW_ACTIVE_NRO, active, sizeof(active)) &&
        supported_switch_now_nro(active) && regular_file(active, NULL))
    {
        snprintf(output, output_size, "%s", active);
        return true;
    }

    output[0] = '\0';
    time_t best_modified = 0;
    consider_candidate(
        SWITCHNOW_DEFAULT_NRO, output, output_size, &best_modified);
    scan_directory(
        "sdmc:/switch/NVStreamNX", output, output_size, &best_modified);
    scan_directory("sdmc:/switch", output, output_size, &best_modified);

    char legacy[FS_MAX_PATH];
    if (read_legacy_target(manifest_path, legacy, sizeof(legacy)))
        consider_candidate(legacy, output, output_size, &best_modified);
    return output[0] != '\0';
}

int main(int argc, char* argv[])
{
    if (argc < 1 || !argv[0])
        return 1;

    char manifest_path[FS_MAX_PATH];
    if (!make_manifest_path(argv[0], manifest_path, sizeof(manifest_path)))
        return 1;

    FILE* manifest = fopen(manifest_path, "rb");
    if (!manifest)
        return 1;
    fclose(manifest);

    char target_path[FS_MAX_PATH];
    if (!resolve_target_path(manifest_path, target_path, sizeof(target_path)))
        return 1;

    char next_arguments[2048];
    const int length = snprintf(
        next_arguments, sizeof(next_arguments), "\"%s\" \"%s\"",
        target_path, manifest_path);
    if (length < 0 || (size_t)length >= sizeof(next_arguments))
        return 1;

    envSetNextLoad(target_path, next_arguments);
    return 0;
}
