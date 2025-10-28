# <img src="https://github.com/Sezzary/SilentMath/blob/master/Icon.png" alt="Icon" width="64" height="64"/> Silent Engine

Track decompilation progress here: https://github.com/Vatuu/silent-hill-decomp

## Current foundations

- Math wrappers and functions (similar to DXTK but with some usability improvements)
- Input handling
- Savegame handling
- Config options handling
- Window handling
- Asset streaming
- Timestep handling
- Parallel task handling
- Filesystem handling
- Clean logging
- Renderer with swappable backends.
- Sound system
- Various utilities pulled from other projects including a `Bitfield` class and a `BoundingVolumeHierarchy` class.

All considerations are being made to make this a flexible, readable, and extendable foundation. Portability is in mind as a top priority for multi-platform support from inception.

### TODOs

- The decomp must be farther along before the bulk of the porting work can begin. More basic things like the boot screen and game menus *could* be ported already with some effort, but it's not worth doing yet while the decomp is in constant flux.
- Must switch to GCC as the compiler.
- Forward renderer.
- Sound system. Need to write a `KDC`+`VAB` -> `XM` converter?
- Parsers for all proprietary game file types.
- Math needs more work.
- Extensive documentation.

## Building (Windows/macOS/Linux)

WIP. Ideally the project will be able to cross-compile between all main development platforms.

### Install dependencies

The project has the following requirements:
- cmake
- git
- python3

### Clone the repository

Clone https://github.com/Sezzary/SilentMath to your desired directory. Cloning recursively will ensure all the following submodules are included in the process:
- assimp
- FlatBuffers
- GLM
- ImGui
- ImGuizmo
- json
- SDL3
- spdlog
- stb

```
git clone --recursive https://github.com/Sezzary/SilentMath
git submodule update --init --recursive
```

### Build the code (Debug/Release)

Available `Make` commands:
- `debug`: Runs Debug build configuration and builds a Debug executable.
- `release`: Runs Debug build configuration and builds a Release executable.
- `configure-debug`: Runs Debug build configuration.
- `configure-release`: Runs Release build configuration.
- `build-debug`: Builds a Debug executable.
- `build-release`: Builds a Release executable.

## Contributing

Too early for this, but feel free to take a look around in the meantime!
