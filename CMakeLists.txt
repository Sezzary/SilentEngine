cmake_minimum_required(VERSION 3.31.6)

# @todo
# - Switch to GCC.
# - Modules don't work.
# - Assimp doesn't work.

# Define project.
project(SilentEngine)

# Specify C and C++ standards.
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)

# Enable modules.
if(MSVC)
    add_compile_options(/std:c++20)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-fmodules)
endif()

# Add executable.
file(GLOB_RECURSE SOURCES "Source/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME "${CMAKE_PROJECT_NAME}" DEBUG_POSTFIX "_d")

# Add library sources.
file(GLOB LIB_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/Libraries/glad/src/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui/backends/imgui_impl_opengl3.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui/backends/imgui_impl_sdl3.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui/backends/imgui_impl_sdlgpu3.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Libraries/ImGuizmo/*.cpp"
)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${LIB_SOURCES})

# Set `Debug` build definitions.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
add_definitions(-D_DEBUG)
endif()

# Set compiler options for `Debug` and `Release` modes.
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    $<$<CONFIG:DEBUG>:-DDEBUG_MODE>
    $<$<CONFIG:RELEASE>:-O3>
)

# Set compiler warning flags.
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

# Link libraries statically.
set(BUILD_SHARED_LIBS OFF)

# Add project header paths.
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Source
    ${CMAKE_CURRENT_LIST_DIR}/..
)

# Add library directories.
#add_subdirectory(Libraries/assimp)
add_subdirectory(Libraries/flatbuffers)
add_subdirectory(Libraries/freetype)
add_subdirectory(Libraries/glad/cmake)
add_subdirectory(Libraries/glm)
add_subdirectory(Libraries/harfbuzz)
add_subdirectory(Libraries/json)
add_subdirectory(Libraries/SDL)
add_subdirectory(Libraries/spdlog)
add_subdirectory(Libraries/utfcpp)

# Add library header paths.
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE
    #${CMAKE_CURRENT_LIST_DIR}/Libraries/assimp/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/flatbuffers/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/freetype/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/glad/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/glm
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/harfbuzz
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui/backends
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/ImGuizmo
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/json/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/SDL/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/spdlog
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/stb
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/utfcpp/source
)

# Add libraries.
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    #assimp
    flatbuffers
    freetype
    glm
    harfbuzz
    SDL3::SDL3-static
    spdlog
)
