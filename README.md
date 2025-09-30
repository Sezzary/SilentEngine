# Silent Math

## WIP implementations

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
- Renderer (WIP)
- Sound system (WIP)
- Various utilities pulled from other projects, including a parallel task executor, a `Bitfield` class, and a `BoundingVolumeHierarchy` class.

All considerations are being made to make this a flexible, readable, and extendable foundation. Portability is in mind as a top priority for multi-platform support from inception. 

## Building (Windows/macOS/Linux)

### Install dependencies

The project has the following requirements:
- cmake
- git
- ninja
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
- SDL_shadercross
- spdlog
- spirv_cross
- stb

```
git clone --recursive https://github.com/Sezzary/SilentMath
```

### Build the code

TODO

## Additional commands

### Generating savegame headers

The port relies on the FlatBuffers library for savegame serialization. If modifying the schemas at `Source/Engine/Savegame/Schemas` to accommodate for changes in the code, make sure to regenerate the savegame headers.

```
python Tools/GenerateSavegame.py
```

### Contributing

Too early for this, but feel free to take a look around!
