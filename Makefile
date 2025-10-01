# Variables

CMAKE       = cmake
BUILD_DIR   = $(PWD)/Build
DEBUG_DIR   = $(BUILD_DIR)/Debug
RELEASE_DIR = $(BUILD_DIR)/Release

# Targets

.PHONY: all debug release configure-debug configure-release build-debug build-release

all: build-debug build-release

debug: configure-debug build-debug

release: configure-release build-release

configure-debug:
    $(CMAKE) -S $(PWD) -B $(DEBUG_DIR) -DCMAKE_BUILD_TYPE=Debug

configure-release:
    $(CMAKE) -S $(PWD) -B $(RELEASE_DIR) -DCMAKE_BUILD_TYPE=Release

build-debug: configure-debug
    $(CMAKE) --build $(DEBUG_DIR)

build-release: configure-release
    $(CMAKE) --build $(RELEASE_DIR)
