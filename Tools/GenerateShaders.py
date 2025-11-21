"""
Platform-Specific Shader Generator

Generates shaders from .HLSL sources to be used by a platform-specific engine executable at runtime.

Usage:
    `python Tools/GenerateShaders.py <build_os>`

Arguments:
    `<build_os>` : The platform to generate shaders for. Must be one of the following (case-insensitive):
                   `"Windows"` : Generates .SPV and .DXIL shaders.
                   `"macOS"`   : Generates .MSL shaders.
                   `"Linux"`   : Generates .SPV shaders.
"""

import os
import platform
import shutil
import subprocess
import sys

from pathlib import Path

SHADERCROSS_NAME = "shadercross"
BASE_PATH        = Path(__file__).parent
SHADERCROSS_PATH = BASE_PATH / SHADERCROSS_NAME
SOURCES_PATH     = BASE_PATH / "../Source/Renderer/Shaders"
OUTPUT_PATH      = BASE_PATH / "../Build/Debug/Debug/Shaders" # @todo Make common output path for Debug and Release. .EXEs can be in Bin/Debug and Bin/Release.
TEMP_OUTPUT_PATH = OUTPUT_PATH / ".Temp"

def generate_shaders():
    """
    Run `shadercross` to generate platform-specific shaders. If generated shaders already exist and are outdated, they will be overwritten.
    """
    try:
        print("Generating shaders...")

        # Ensure temporary output folder is deleted.
        if os.path.isfile(TEMP_OUTPUT_PATH):
            shutil.rmtree(TEMP_OUTPUT_PATH)

        # Setup.
        shadercross_exe = get_shadercross_executable()
        formats         = get_output_formats()
        os.makedirs(OUTPUT_PATH,      exist_ok=True)
        os.makedirs(TEMP_OUTPUT_PATH, exist_ok=True)

        # Collect all shader sources.
        shader_sources = [
            os.path.join(SOURCES_PATH, file)
            for file in os.listdir(SOURCES_PATH) if file.endswith(".hlsl")
        ]

        # Build shaders to temporary output folder.
        build_count = 0
        fail_names  = []
        for shader_source in shader_sources:
            # Define base name.
            name = Path(os.path.splitext(shader_source)[0]).name

            for format in formats:
                # Define output name.
                output_name = name + format

                # Define output file.
                shader_output      = OUTPUT_PATH      / output_name
                temp_shader_output = TEMP_OUTPUT_PATH / output_name

                # Check if new shader build is required.
                run_new_build = False
                if os.path.isfile(shader_output):
                    run_new_build = os.path.getmtime(shader_source) > os.path.getmtime(shader_output)
                else:
                    run_new_build = True

                # Run generation command.
                if run_new_build:
                    command = [shadercross_exe, shader_source, "-o", temp_shader_output]

                    result = subprocess.run(command, capture_output=True)
                    if result.returncode == 0:
                        build_count += 1
                    else:
                        print(f"Command error for `{output_name}`: {result.stderr.decode()}")
                        fail_names.append(output_name)

        # Delete failed shaders from temporary output folder.
        for fail_name in fail_names:
            temp_shader_output = TEMP_OUTPUT_PATH / fail_name
            if os.path.isfile(temp_shader_output):
                os.remove(temp_shader_output)

        # Copy contents of temporary output folder to real output folder.
        for shader_output in os.listdir(TEMP_OUTPUT_PATH):
            shutil.copy(TEMP_OUTPUT_PATH / shader_output, OUTPUT_PATH / shader_output)
        shutil.rmtree(TEMP_OUTPUT_PATH)

        # Report status.
        if build_count == 0 and len(fail_names) == 0:
            print("Shaders are up-to-date.")
        else:
            print(f"{build_count} shader{"" if build_count == 1 else "s"} built successfully." + f" {len(fail_names)} failed:" if len(fail_names) > 0 else "")
            for fail_name in fail_names:
                print(fail_name)
    except Exception as ex:
        # Ensure temporary output folder is deleted.
        if os.path.isfile(TEMP_OUTPUT_PATH):
            shutil.rmtree(TEMP_OUTPUT_PATH)

        # Report exception.
        print(f"Error: {ex}")
        sys.exit(1)

def get_shadercross_executable():
    """
    Get the path to the appropriate `shadercross` executable based on the system OS.
    """
    # Setup.
    system_os = platform.system()

    # Define executable path corresponding for current platform.
    if system_os == "Windows":
        shadercross_exe = os.path.join(SHADERCROSS_PATH, "Windows", SHADERCROSS_NAME + ".exe")
    elif system_os == "Linux":
        shadercross_exe = os.path.join(SHADERCROSS_PATH, "Linux", SHADERCROSS_NAME)
    elif system_os == "Darwin": # macOS.
        shadercross_exe = os.path.join(SHADERCROSS_PATH, "MacOs", SHADERCROSS_NAME)
    else:
        raise Exception(f"'{system_os}' is unsupported.")

    if not os.path.isfile(shadercross_exe):
        raise Exception(f"`{SHADERCROSS_NAME}` executable not found at '{shadercross_exe}'.")

    return shadercross_exe

def get_output_formats():
    """
    Get the platform-specific shader formats to build according to the passed `build_os` argument.
    """
    if len(sys.argv) > 1:
        build_os = sys.argv[1].lower()
        if build_os == "windows":
            formats = [".spv", ".dxil"]
        elif build_os == "macos":
            formats = [".msl"]
        elif build_os == "linux":
            formats = [".spv"]
        else:
            raise Exception(f"Passed invalid `build_os` argument `{build_os}`.")
    else:
        raise Exception("No `build_os` argument passed.")

    return formats

generate_shaders()
