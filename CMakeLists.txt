cmake_minimum_required(VERSION 3.28.3)

# @todo
# - Modules don't work.
# - Integrate Assimp library for more standard asset format support if it makes sense later.
# - Integrate curl library for auto updates later.
# - Integrate ImGuizmo library for potential debug interactivily later.
# - If FluidSynth is ever used, add this to build steps in the `README.md`:
# Build the FluidSynth dependency locally.
# ```
# cd Libraries/fluidsynth && mkdir build && cd build && cmake .. && cd ../../..
# 
# ```

# ==============
# Project Setup
# ==============

# Define project.
project(SilentEngine C CXX)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find dependencies.
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Add executable.
file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS "Source/*.cpp"
)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${CMAKE_PROJECT_NAME}"
    DEBUG_POSTFIX "_d"
)

# Set compiler options.
if(MSVC)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
        /W3     # Enable warning level 3.
        /wd4018 # Suppress sign comparison warnings.
        /wd4244 # Suppress narrowing/conversion warnings. Required for C struct compatibility.
        /wd4267 # Suppress type conversion warnings.
        $<$<CONFIG:Release>:/O2 /Zi>
    )
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/DEBUG>
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
        -Wall                # Enable common warnings.
        -Wno-narrowing       # Silence narrowing entirely. Required for C struct compatibility.
        -Wno-changes-meaning # Suppress name shadowing errors.
        -Wformat-extra-args  # Suppress format string errors.
        -Wno-sign-compare    # Suppress sign comparison warnings.
        $<$<CONFIG:Release>:-O2 -g>
    )
endif()

# Set macro definitions.
execute_process(
    COMMAND git remote get-url origin
    OUTPUT_VARIABLE SILENT_GIT_REPO
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    OUTPUT_VARIABLE SILENT_GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND git rev-parse HEAD
    OUTPUT_VARIABLE SILENT_GIT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
target_compile_definitions(
    ${CMAKE_PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG_MODE _DEBUG>
    SILENT_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}"
    SILENT_COMPILER_VERSION="${CMAKE_CXX_COMPILER_VERSION}"
    SILENT_GIT_REPO="${SILENT_GIT_REPO}"
    SILENT_GIT_BRANCH="${SILENT_GIT_BRANCH}"
    SILENT_GIT_COMMIT="${SILENT_GIT_COMMIT}"
)

# Set system name.
if(WIN32)
    set(SYSTEM_NAME "Windows")
elseif(APPLE)
    set(SYSTEM_NAME "MacOS")
elseif(UNIX)
    set(SYSTEM_NAME "Linux")
else()
    set(SYSTEM_NAME "Unknown")
endif()

# Generate launcher.
add_custom_target(
    GenerateLauncher ALL
    COMMAND Python3::Interpreter "${CMAKE_CURRENT_SOURCE_DIR}/Tools/GenerateLauncher.py"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
add_dependencies(${CMAKE_PROJECT_NAME} GenerateLauncher)

# Generate shaders.
add_custom_target(
    GenerateShaders ALL
    COMMAND Python3::Interpreter "${CMAKE_CURRENT_SOURCE_DIR}/Tools/GenerateShaders.py" "-os" "${SYSTEM_NAME}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
add_dependencies(${CMAKE_PROJECT_NAME} GenerateShaders)

# =================
# Dependency Setup
# =================

# Add library sources.
file(GLOB LIB_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui/backends/imgui_impl_sdl3.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui/backends/imgui_impl_sdlgpu3.cpp"
    #"${CMAKE_CURRENT_LIST_DIR}/Libraries/ImGuizmo/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Libraries/smol-atlas/src/*.cpp"
)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${LIB_SOURCES})

# Link libraries statically.
set(BUILD_SHARED_LIBS OFF)

# Add project header paths.
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Source
    ${CMAKE_CURRENT_LIST_DIR}/..
)

# Add library directories.
#add_subdirectory(Libraries/assimp)
#add_subdirectory(Libraries/curl)
#add_subdirectory(Libraries/fluidsynth)
add_subdirectory(Libraries/freetype)
add_subdirectory(Libraries/glm)
add_subdirectory(Libraries/json)
add_subdirectory(Libraries/psyz/psyz)
#add_subdirectory(Libraries/SDL) # @todo PSY-Z also uses SDL and keeping this line causes a conflict.
add_subdirectory(Libraries/sol2)
add_subdirectory(Libraries/spdlog)
add_subdirectory(Libraries/utfcpp)
add_subdirectory(Libraries/yalantinglibs)

# Add library header paths.
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE
    #${CMAKE_CURRENT_LIST_DIR}/Libraries/assimp/include
    #${CMAKE_CURRENT_LIST_DIR}/Libraries/curl/include
    #${CMAKE_CURRENT_LIST_DIR}/Libraries/fluidsynth/build/include
    #${CMAKE_CURRENT_LIST_DIR}/Libraries/fluidsynth/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/freetype/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/glm
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui/backends
    #${CMAKE_CURRENT_LIST_DIR}/Libraries/ImGuizmo
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/json/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/LuaJIT/src
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/pl_mpeg
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/psyz/psyz/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/SDL/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/smol-atlas/src
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/sol2/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/spdlog
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/stb
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/utfcpp/source
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/yalantinglibs/include
)

# Create target for LuaJIT library.
add_library(luajit STATIC IMPORTED)
set_target_properties(luajit PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/Libraries/LuaJIT/src/libluajit.a")

# Set PSY-Z compile definition.
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE __psyz)

# Add libraries.
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    #assimp
    #libcurl
    #libfluidsynth
    freetype
    glm
    luajit
    psyz
    sol2
    SDL3::SDL3-static
    spdlog
    yalantinglibs
)
