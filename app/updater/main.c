#include <mbedtls/sha256.h>
#include <switch.h>

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static const char* Target = "sdmc:/switch/NVStreamNX/NVStreamNX.nro";
static const char* Staged = "sdmc:/switch/NVStreamNX/NVStreamNX.nro.update";
static const char* Marker = "sdmc:/switch/NVStreamNX/NVStreamNX.nro.update.sha256";
static const char* Backup = "sdmc:/switch/NVStreamNX/NVStreamNX.nro.previous";
static const char* LogPath = "sdmc:/switch/NVStreamNX/update.log";

static void write_log(const char* format, ...)
{
    FILE* file = fopen(LogPath, "ab");
    if (!file) return;
    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);
    fclose(file);
}

static bool read_expected(char expected[65])
{
    FILE* file = fopen(Marker, "rb");
    if (!file) return false;
    const int matched = fscanf(file, "%64s", expected);
    fclose(file);
    if (matched != 1 || strlen(expected) != 64) return false;
    for (size_t index = 0; index < 64; ++index)
    {
        if (!isxdigit((unsigned char)expected[index])) return false;
        expected[index] = (char)tolower((unsigned char)expected[index]);
    }
    return true;
}

static bool checksum_file(const char* path, char result[65])
{
    FILE* file = fopen(path, "rb");
    if (!file) return false;
    mbedtls_sha256_context context;
    mbedtls_sha256_init(&context);
    if (mbedtls_sha256_starts(&context, 0) != 0)
    {
        fclose(file);
        return false;
    }
    static unsigned char buffer[16 * 1024];
    size_t count;
    while ((count = fread(buffer, 1, sizeof(buffer), file)) > 0)
        if (mbedtls_sha256_update(&context, buffer, count) != 0)
        {
            fclose(file);
            mbedtls_sha256_free(&context);
            return false;
        }
    const bool read_ok = !ferror(file);
    fclose(file);
    unsigned char digest[32];
    const bool finish_ok = mbedtls_sha256_finish(&context, digest) == 0;
    mbedtls_sha256_free(&context);
    if (!read_ok || !finish_ok) return false;
    static const char digits[] = "0123456789abcdef";
    for (size_t index = 0; index < sizeof(digest); ++index)
    {
        result[index * 2] = digits[digest[index] >> 4];
        result[index * 2 + 1] = digits[digest[index] & 15];
    }
    result[64] = '\0';
    return true;
}

static bool verified(const char* path)
{
    char expected[65] = {0};
    char actual[65] = {0};
    return read_expected(expected) && checksum_file(path, actual) && strcmp(expected, actual) == 0;
}

int main(int argc, char** argv)
{
    const bool requested = argc > 1 && argv[1] && strcmp(argv[1], "--finish-update") == 0;
    write_log("[helper] start requested=%d\n", requested ? 1 : 0);
    if (!requested || !verified(Staged))
    {
        write_log("[helper] staged file verification failed\n");
        return 0;
    }
    unlink(Backup);
    if (rename(Target, Backup) != 0)
    {
        write_log("[helper] backup failed errno=%d\n", errno);
        return 0;
    }
    if (rename(Staged, Target) != 0 || !verified(Target))
    {
        const int saved = errno;
        unlink(Target);
        rename(Backup, Target);
        fsdevCommitDevice("sdmc");
        write_log("[helper] install failed errno=%d; rolled back\n", saved);
        return 0;
    }
    if (R_FAILED(fsdevCommitDevice("sdmc")))
    {
        unlink(Target);
        rename(Backup, Target);
        fsdevCommitDevice("sdmc");
        write_log("[helper] commit failed; rolled back\n");
        return 0;
    }
    write_log("[helper] update installed; returning to HOME\n");
    return 0;
}
