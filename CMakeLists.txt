cmake_minimum_required(VERSION 3.31.6)

# Define project.
project(Math)

# Specify C and C++ standards.
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX_SCAN_FOR_MODULES ON) # @todo Want to use modules but can't get them to work.
set(CMAKE_CXX_MODULES ON)

# Add executable.
file(GLOB_RECURSE SOURCES "Source/*.cpp")
add_executable(Math ${SOURCES})
set_target_properties(Math PROPERTIES OUTPUT_NAME "Math" DEBUG_POSTFIX "_d")

# Add library sources.
file(GLOB LIB_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/Libraries/glad/src/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui/backends/imgui_impl_opengl3.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui/backends/imgui_impl_sdl3.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui/backends/imgui_impl_sdlgpu3.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Libraries/ImGuizmo/*.cpp"
)
target_sources(Math PRIVATE ${LIB_SOURCES})

# Set `Debug` build definitions.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
add_definitions(-D_DEBUG)
endif()

# Set compiler options for `Debug` and `Release` modes.
target_compile_options(Math PRIVATE
    #-include "${CMAKE_CURRENT_LIST_DIR}/Source/Framework.h"
    $<$<CONFIG:DEBUG>:-DDEBUG_MODE>
    $<$<CONFIG:RELEASE>:-O3>
)

# @todo Switch to GCC.
# Set compiler warning flags.
if(MSVC)
    # MSVC warning flags.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
else()
    # GCC/Clang warning flags.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

# Link libraries statically.
set(BUILD_SHARED_LIBS OFF)

# Add project header paths.
target_include_directories(Math PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Source
    ${CMAKE_CURRENT_LIST_DIR}/..
)

# Add library directories.
add_subdirectory(Libraries/assimp)
add_subdirectory(Libraries/flatbuffers)
add_subdirectory(Libraries/glad/cmake)
add_subdirectory(Libraries/glm)
add_subdirectory(Libraries/json)
add_subdirectory(Libraries/SDL)
#add_subdirectory(Libraries/SDL_shadercross)
add_subdirectory(Libraries/spdlog)

# Add library header paths.
target_include_directories(Math SYSTEM PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/assimp/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/flatbuffers/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/glad/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/glm
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui/backends
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/ImGuizmo
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/json/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/SDL/include
    #${CMAKE_CURRENT_LIST_DIR}/Libraries/SDL_shadercross/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/spdlog
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/stb
)

# Add libraries.
target_link_libraries(Math PRIVATE
    assimp
    flatbuffers
    glm
    SDL3-static
    #SDL_shadercross
    spdlog
)

# Copy shaders to build folder.
set(SHADER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Shaders)
set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
set(SHADER_BUILD_DIR ${CMAKE_BINARY_DIR}/${BUILD_TYPE}/Shaders)
file(MAKE_DIRECTORY ${SHADER_BUILD_DIR})
file(COPY ${SHADER_SOURCE_DIR}/ DESTINATION ${SHADER_BUILD_DIR})
