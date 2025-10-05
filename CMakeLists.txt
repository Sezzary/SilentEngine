cmake_minimum_required(VERSION 3.31.6)

# Define project.
project(SilentEngine)

# Specify C and C++ standards.
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX_SCAN_FOR_MODULES ON) # @todo Want to use modules but can't get them to work.
set(CMAKE_CXX_MODULES ON)

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

# @todo Switch to GCC.
# Set compiler warning flags.
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3") # MSVC warning flags.
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall") # GCC/Clang warning flags.
endif()

# Link libraries statically.
set(BUILD_SHARED_LIBS OFF)

# Add project header paths.
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Source
    ${CMAKE_CURRENT_LIST_DIR}/..
)

# Prevent SDL_shadercross from looking for system packages.
set(SDLSHADERCROSS_VENDORED ON)

# Add library directories.
add_subdirectory(Libraries/assimp)
add_subdirectory(Libraries/flatbuffers)
add_subdirectory(Libraries/glad/cmake)
add_subdirectory(Libraries/glm)
add_subdirectory(Libraries/json)
add_subdirectory(Libraries/SDL)
add_subdirectory(Libraries/SDL_shadercross)
add_subdirectory(Libraries/spdlog)

# Add library header paths.
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/assimp/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/flatbuffers/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/glad/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/glm
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/imgui/backends
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/ImGuizmo
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/json/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/SDL/include
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/SDL_shadercross
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/spdlog
    ${CMAKE_CURRENT_LIST_DIR}/Libraries/stb
)

# Add libraries.
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    assimp
    flatbuffers
    glm
    SDL3::SDL3-static
    SDL3_shadercross::SDL3_shadercross-static
    spdlog
)

# Set shader directories.
set(SHADER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Shaders)
set(SHADER_BUILD_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/Shaders)
set(SHADER_DXBC_DIR ${SHADER_BUILD_DIR}/Dxbc)
set(SHADER_DXIL_DIR ${SHADER_BUILD_DIR}/Dxil)
set(SHADER_HLSL_DIR ${SHADER_BUILD_DIR}/Hlsl)
set(SHADER_MSL_DIR ${SHADER_BUILD_DIR}/Msl)
set(SHADER_SPV_DIR ${SHADER_BUILD_DIR}/Spv)

# Ensure shader directories exist.
file(MAKE_DIRECTORY ${SHADER_BUILD_DIR})
file(MAKE_DIRECTORY ${SHADER_DXBC_DIR})
file(MAKE_DIRECTORY ${SHADER_DXIL_DIR})
file(MAKE_DIRECTORY ${SHADER_HLSL_DIR})
file(MAKE_DIRECTORY ${SHADER_MSL_DIR})
file(MAKE_DIRECTORY ${SHADER_SPV_DIR})

# @todo NOT YET!!!
# Clean output directories.
#file(GLOB OLD_SHADERS "${SHADER_HLSL_DIR}/*" "${SHADER_MSL_DIR}/*" "${SHADER_SPV_DIR}/*")
#foreach(OLD_SHADER ${OLD_SHADERS})
#    file(REMOVE ${OLD_SHADER})
#endforeach()

## SDL_shadercross executable.
#set(SHADERCROSS_EXECUTABLE shadercross)
#
## Collect all shader files in source directory.
#file(GLOB SHADERS "${SHADER_SOURCE_DIR}/*.hlsl")
#
## Collect shader compilation targets.
#foreach(SHADER ${SHADERS})
#    get_filename_component(FILE_NAME ${SHADER} NAME)
#
#    # Set output paths for compiled shaders (DXBC, DXIL, HLSL, MSL, SPIR-V).
#    set(OUTPUT_DXBC ${SHADER_DXBC_DIR}/${FILE_NAME}.dxbc)
#    set(OUTPUT_DXIL ${SHADER_DXIL_DIR}/${FILE_NAME}.dxil)
#    set(OUTPUT_HLSL ${SHADER_HLSL_DIR}/${FILE_NAME}.hlsl)
#    set(OUTPUT_MSL ${SHADER_MSL_DIR}/${FILE_NAME}.msl)
#    set(OUTPUT_SPV ${SHADER_SPV_DIR}/${FILE_NAME}.spv)
#
#    # HLSL (source).
#    add_custom_command(
#        TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy ${OUTPUT_HLSL} ${DESTINATION_DIR}/${FILE_NAME}.hlsl
#        COMMENT "Copying HLSL shader ${FILE_NAME}."
#    )
#
#    # Strip `.hlsl` extension.
#    get_filename_component(FILE_BASENAME ${FILE_NAME} NAME_WE)
#
#    # DXBC.
#    add_custom_command(
#        OUTPUT ${OUTPUT_DXBC}
#        COMMAND ${SHADERCROSS_EXECUTABLE} ${SHADER} -s HLSL -d DXBC -t fragment -e main -o ${OUTPUT_DXBC}
#        DEPENDS ${SHADER}
#        COMMENT "Compiling shader ${FILE_NAME} to DXBC."
#    )
#
#    # DXIL.
#    add_custom_command(
#        OUTPUT ${OUTPUT_DXIL}
#        COMMAND ${SHADERCROSS_EXECUTABLE} ${SHADER} -s HLSL -d DXIL -t fragment -e main -o ${OUTPUT_DXIL}
#        DEPENDS ${SHADER}
#        COMMENT "Compiling shader ${FILE_NAME} to DXIL."
#    )
#
#    # MSL.
#    add_custom_command(
#        OUTPUT ${OUTPUT_MSL}
#        COMMAND ${SHADERCROSS_EXECUTABLE} ${SHADER} -s HLSL -d MSL -t fragment -e main -o ${OUTPUT_MSL}
#        DEPENDS ${SHADER}
#        COMMENT "Compiling shader ${FILE_NAME} to MSL."
#    )
#
#    # SPIR-V.
#    add_custom_command(
#        OUTPUT ${OUTPUT_SPV}
#        COMMAND ${SHADERCROSS_EXECUTABLE} ${SHADER} -s HLSL -d SPIRV -t fragment -e main -o ${OUTPUT_SPV}
#        DEPENDS ${SHADER}
#        COMMENT "Compiling shader ${FILE_NAME} to SPIR-V."
#    )
#
#    list(APPEND COMPILED_SHADERS ${OUTPUT_SPV} ${OUTPUT_MSL} ${OUTPUT_HLSL})
#endforeach()
#
## Create custom target to run shader compilation.
#add_custom_target(CompileShaders ALL DEPENDS ${COMPILED_SHADERS})
