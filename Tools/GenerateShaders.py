"""
Shader Generator

Generates shaders from .HLSL sources to be used by a platform-specific engine executable at runtime.
If generated shaders already exist and are outdated, they will be overwritten.

Usage:
    `python Tools/GenerateShaders.py [-os <string>]`

Arguments:
    --buildOs, -os : The platform to generate shaders for. Must be one of the following (case-insensitive):
                     `Windows` : Generates .SPV and .DXIL shaders.
                     `macOS`   : Generates .MSL shaders.
                     `Linux`   : Generates .SPV shaders.
"""


import logging
import os
import platform
import re
import shutil
import subprocess
import sys

from argparse import ArgumentParser
from pathlib  import Path


SHADERCROSS_NAME = "shadercross"
BASE_PATH        = Path(__file__).parent
SHADERCROSS_PATH = BASE_PATH / SHADERCROSS_NAME
SOURCES_PATH     = BASE_PATH / "../Source/Renderer/Common/Shaders"
OUTPUT_PATH      = BASE_PATH / "../Build/Assets/Shaders"
TEMP_OUTPUT_PATH = OUTPUT_PATH / ".temp"


def _create_parser():
    """
    Create an argument parser for the script.
    """
    parser = ArgumentParser()
    parser.add_argument("--buildOs", "-os", type=str)
    return parser


def _get_shadercross_exe():
    """
    Get the path to the appropriate `shadercross` executable based on the system OS.
    """
    # Define executable path corresponding for current platform.
    system_os = platform.system()
    if system_os == "Windows":
        shadercross_exe = os.path.join(SHADERCROSS_PATH, "Windows", SHADERCROSS_NAME + ".exe")
    elif system_os == "Darwin": # macOS.
        shadercross_exe = os.path.join(SHADERCROSS_PATH, "MacOs", SHADERCROSS_NAME)
    elif system_os == "Linux":
        shadercross_exe = os.path.join(SHADERCROSS_PATH, "Linux", SHADERCROSS_NAME)
    else:
        raise Exception(f"'{system_os}' is unsupported.")

    if not os.path.isfile(shadercross_exe):
        raise Exception(f"`{SHADERCROSS_NAME}` executable not found at '{shadercross_exe}'.")

    return shadercross_exe


def _get_output_formats(build_os: str):
    """
    Get the platform-specific shader formats to build according to the passed `build_os` argument.
    """
    build_os = build_os.lower()
    if build_os == "windows":
        formats = [".spv", ".dxil"]
    elif build_os == "macos":
        formats = [".msl"]
    elif build_os == "linux":
        formats = [".spv"]
    else:
        raise Exception(f"Passed invalid `build_os` argument `{build_os}`.")

    return formats


def _get_shader_headers(source_path: Path):
    """
    Get the `HLSLI` headers of a shader source.
    """
    INCLUDE_REGEX = re.compile(r'#include\s+["<](.*?)[">]')

    header_paths = set()

    # Collect nested header paths.
    stack = [source_path]
    while stack:
        # Ignore header path if previously collected.
        current_path = stack.pop()
        if current_path in header_paths or not current_path.is_file():
            continue

        # Collect header path.
        if current_path != source_path:
            header_paths.add(current_path)

        try:
            content = current_path.read_text(encoding='utf-8')
            for match in re.finditer(INCLUDE_REGEX, content):
                header_path = (current_path.parent / match.group(1)).resolve()
                if header_path not in header_paths:
                    stack.append(header_path)
        except OSError:
            pass

    return header_paths


def _cleanup():
    """
    Delete temporary build files.
    """
    shutil.rmtree(TEMP_OUTPUT_PATH, ignore_errors=True)


def main():
    try:
        logging.basicConfig(level=logging.INFO)
        _cleanup()

        logging.info("Generating shaders...")

        # Setup.
        args            = _create_parser().parse_args()
        shadercross_exe = _get_shadercross_exe()
        formats         = _get_output_formats(args.buildOs)
        os.makedirs(OUTPUT_PATH,      exist_ok=True)
        os.makedirs(TEMP_OUTPUT_PATH, exist_ok=True)

        # Collect all shader sources.
        shader_sources = list(Path(SOURCES_PATH).rglob("*.hlsl"))

        # Build shaders to temporary output folder.
        build_count = 0
        fail_names  = []
        for shader_source in shader_sources:
            # Define base name.
            name = Path(os.path.splitext(shader_source)[0]).name

            # Get shader headers.
            shader_headers = _get_shader_headers(shader_source)

            for format in formats:
                # Define output name.
                output_name = name + format

                # Define output file.
                shader_output      = OUTPUT_PATH      / output_name
                temp_shader_output = TEMP_OUTPUT_PATH / output_name

                # Check if new shader build is required.
                run_new_build = False
                if os.path.isfile(shader_output):
                    newest_time = os.path.getmtime(shader_source)
                    for shader_header in shader_headers:
                        newest_time = max(newest_time, os.path.getmtime(shader_header))

                    run_new_build = newest_time > os.path.getmtime(shader_output)
                else:
                    run_new_build = True

                # Run generation command.
                if run_new_build:
                    command = [
                        shadercross_exe, shader_source,
                        "-I", SOURCES_PATH,
                        "-o", temp_shader_output
                    ]
                    result  = subprocess.run(command, capture_output=True)

                    # Report status.
                    if result.returncode == 0:
                        logging.info(f"`{output_name}`")
                        build_count += 1
                    else:
                        logging.error(f"Command error for `{output_name}`: {result.stderr.decode()}")
                        fail_names.append(output_name)

        # Delete failed shaders from temporary output folder.
        for fail_name in fail_names:
            temp_shader_output = TEMP_OUTPUT_PATH / fail_name
            if os.path.isfile(temp_shader_output):
                os.remove(temp_shader_output)

        # Copy contents of temporary output folder to real output folder.
        for shader_output in os.listdir(TEMP_OUTPUT_PATH):
            shutil.copy(TEMP_OUTPUT_PATH / shader_output, OUTPUT_PATH / shader_output)

        # Report status.
        if build_count == 0 and len(fail_names) == 0:
            logging.info("Shaders are up-to-date.")
        else:
            success_str = f"{build_count} shader{"" if build_count == 1 else "s"} built successfully."
            fail_str    = (f" {len(fail_names)} failed:" if len(fail_names) > 0 else "")
            logging.info(success_str + fail_str)

            for fail_name in fail_names:
                logging.info(f"`{fail_name}`")

            # Fail.
            if len(fail_names) > 0:
                sys.exit(1)

        _cleanup()
    except Exception as ex:
        _cleanup()

        # Fail.
        logging.error(f"{ex}")
        sys.exit(1)


if __name__ == "__main__":
    main()
