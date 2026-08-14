#pragma once

#include <stdbool.h>

int writefile(const char *filename, const unsigned char *start, const unsigned char *end, bool executable);

int ensuredirectory(const char *directory);

int forkcommand(const char *directory, const char *command);
