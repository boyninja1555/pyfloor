CC      ?= cc
OBJCOPY ?= objcopy

TARGET  := pyfloor

SRC := \
	src/cmd/main.c \
	src/cmd/util.c

OBJ := \
	build/main.o \
	build/util.o \
	build/script_unix.o \
	build/script_windows.o

CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Iinclude

ifeq ($(OS),Windows_NT)
    HOST_OS := windows
else
    UNAME_S := $(shell uname -s 2>/dev/null)

    ifeq ($(UNAME_S),Darwin)
        HOST_OS := macos
    else ifeq ($(UNAME_S),Linux)
        HOST_OS := linux
    else ifeq ($(UNAME_S),FreeBSD)
        HOST_OS := freebsd
    else
        HOST_OS := unix
    endif
endif

TARGET_TRIPLE := $(shell $(CC) -dumpmachine 2>/dev/null)

ifeq ($(HOST_OS),macos)
    OBJCOPY_INPUT := binary
    OBJCOPY_OUTPUT := macho64
    OBJCOPY_ARCH := arm64
else ifeq ($(HOST_OS),windows)
    OBJCOPY_INPUT := binary
    ifneq ($(findstring aarch64,$(TARGET_TRIPLE)),)
        OBJCOPY_OUTPUT := pe-aarch64
        OBJCOPY_ARCH   := aarch64
    else
        OBJCOPY_OUTPUT := pe-x86-64
        OBJCOPY_ARCH   := i386:x86-64
    endif
else
    OBJCOPY_INPUT := binary
    ifneq ($(findstring aarch64,$(TARGET_TRIPLE)),)
        OBJCOPY_OUTPUT := elf64-littleaarch64
        OBJCOPY_ARCH   := aarch64
    else
        OBJCOPY_OUTPUT := elf64-x86-64
        OBJCOPY_ARCH   := i386:x86-64
    endif

endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

build/main.o: src/cmd/main.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/util.o: src/cmd/util.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/script_unix.o: src/wrapper/unix.sh
	@mkdir -p build
	$(OBJCOPY) -I $(OBJCOPY_INPUT) -O $(OBJCOPY_OUTPUT) -B $(OBJCOPY_ARCH) $< $@

build/script_windows.o: src/wrapper/windows.bat
	@mkdir -p build
	$(OBJCOPY) -I $(OBJCOPY_INPUT) -O $(OBJCOPY_OUTPUT) -B $(OBJCOPY_ARCH) $< $@

clean:
	$(RM) -r build $(TARGET)

.PHONY: all clean