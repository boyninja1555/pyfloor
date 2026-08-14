#include "util.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <sys/wait.h>
#include <windows.h>
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

int forkcommand(const char *directory, const char *command)
{
#ifdef _WIN32
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char *mutable_command = _strdup(command);
    if (mutable_command == NULL)
        return 1;

    BOOL ok = CreateProcessA(NULL, mutable_command, NULL, NULL, FALSE, 0, NULL, directory, &si, &pi);
    free(mutable_command);
    if (!ok)
        return 1;

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exit_code;
    BOOL got_exit_code = GetExitCodeProcess(pi.hProcess, &exit_code);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    if (!got_exit_code)
        return 1;

    return (int)exit_code;

#else
    pid_t pid = fork();
    if (pid < 0)
        return 1;

    if (pid == 0)
    {
        if (chdir(directory) != 0)
            _exit(127);

        execl("/bin/sh", "sh", "-c", command, (char *)NULL);
        _exit(127);
    }

    int status;

    if (waitpid(pid, &status, 0) < 0)
        return 1;

    if (WIFEXITED(status))
        return WEXITSTATUS(status);

    if (WIFSIGNALED(status))
        return 128 + WTERMSIG(status);

    return 1;
#endif
}
