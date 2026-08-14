#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "util.h"

#ifdef _WIN32
#define PATHSEP '\\'
#else
#define PATHSEP '/'
#endif

#define EMBED_SYMBOL(name) name

extern const unsigned char EMBED_SYMBOL(_binary_src_wrapper_unix_sh_start)[];
extern const unsigned char EMBED_SYMBOL(_binary_src_wrapper_unix_sh_end)[];
extern const unsigned char EMBED_SYMBOL(_binary_src_wrapper_windows_bat_start)[];
extern const unsigned char EMBED_SYMBOL(_binary_src_wrapper_windows_bat_end)[];

int main(int argc, const char *argv[])
{
    if (argc > 2)
    {
        fprintf(stderr, "Usage: %s [directory]\n", argv[0]);
        return 2;
    }

    const char *directory = argc == 2 ? argv[1] : ".";
    if (ensuredirectory(directory) != 0)
        return 1;

    char filename_unix[strlen(directory) + sizeof(PATHSEP) + sizeof("pyfloor.sh")];
    snprintf(filename_unix, sizeof(filename_unix), "%s%cpyfloor.sh", directory, PATHSEP);

    char filename_windows[strlen(directory) + sizeof(PATHSEP) + sizeof("pyfloor.bat")];
    snprintf(filename_windows, sizeof(filename_windows), "%s%cpyfloor.bat", directory, PATHSEP);

    if (writefile(filename_unix, _binary_src_wrapper_unix_sh_start, _binary_src_wrapper_unix_sh_end, true) != 0)
        return 1;

    if (writefile(filename_windows, _binary_src_wrapper_windows_bat_start, _binary_src_wrapper_windows_bat_end, false) != 0)
        return 1;

#ifdef _WIN32
    char *filename = filename_windows;
#else
    char *filename = filename_unix;
#endif
    char command[sizeof('.') + strlen(filename) - strlen(directory) + sizeof(" init")];
    snprintf(command, sizeof(command), ".%s%s", filename + strlen(directory), " init");
    if (forkcommand(directory, (const char *)command) == 0)
    {
        printf("\nStarted PyFloor project!\n"
               "\tDepending on your platform, either `%s` or `%s` can be run for help and further actions.\n"
               "\tCommit/push these scripts for easy setup and organization!\n",
               filename_unix, filename_windows);

        return 0;
    }

    return 1;
}
