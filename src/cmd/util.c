#include "util.h"
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32

#include <direct.h>

#define MKDIR(directory) _mkdir(directory)
#define STAT _stat
#define STAT_STRUCT struct _stat

#else

#include <sys/types.h>

#define MKDIR(directory) mkdir(directory, 0777)
#define STAT stat
#define STAT_STRUCT struct stat

#endif

int writefile(const char *filename, const unsigned char *start, const unsigned char *end, bool executable)
{
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        perror(filename);
        return 1;
    }

    size_t size = (size_t)(end - start);
    if (fwrite(start, 1, size, file) != size)
    {
        perror(filename);
        fclose(file);
        return 1;
    }

    fclose(file);

#ifndef _WIN32
    if (executable && chmod(filename, 0755) != 0)
    {
        perror(filename);
        return 1;
    }
#else
    (void)executable;
#endif
    return 0;
}

int ensuredirectory(const char *directory)
{
    STAT_STRUCT info;

    if (STAT(directory, &info) == 0)
    {
        if (!S_ISDIR(info.st_mode))
        {
            fprintf(stderr, "%s exists but is not a directory.\n", directory);
            return 1;
        }

        return 0;
    }

    if (errno != ENOENT)
    {
        fprintf(stderr, "Could not inspect directory %s! %s\n", directory, strerror(errno));
        return 1;
    }

    printf("Directory %s does not exist! Create it? (y/N) ", directory);

    int answer = getchar();
    if (answer != 'y' && answer != 'Y')
        return 1;

    char buffer[strlen(directory) + 1];
    strcpy(buffer, directory);
    for (char *p = buffer; *p != '\0'; ++p)
    {
#ifdef _WIN32
        if (*p != '\\' && *p != '/')
            continue;
#else
        if (*p != '/')
            continue;
#endif
        if (p == buffer)
            continue;

#ifdef _WIN32
        if (p == buffer + 2 && buffer[1] == ':')
            continue;
#endif

        *p = '\0';

        if (buffer[0] != '\0' && MKDIR(buffer) != 0 && errno != EEXIST)
        {
            fprintf(
                stderr,
                "Could not create directory %s! %s\n",
                buffer,
                strerror(errno));
            return 1;
        }

        *p =
#ifdef _WIN32
            '\\';
#else
            '/';
#endif
    }

    if (MKDIR(buffer) != 0 && errno != EEXIST)
    {
        fprintf(stderr, "Could not create directory %s! %s\n", buffer, strerror(errno));
        return 1;
    }

    return 0;
}
