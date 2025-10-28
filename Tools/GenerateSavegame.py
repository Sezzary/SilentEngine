"""
Savegame Header Generator

Generates savegame FlatBuffers headers from schemas to be used in savegame system code.

Usage:
    `python Tools/GenerateSavegame.py`
"""

import os
import platform
import subprocess
import sys

from pathlib import Path

FLATC_NAME    = "flatc"
BASE_PATH     = Path(__file__).parent
FLATC_PATH    = BASE_PATH / FLATC_NAME
SAVEGAME_PATH = BASE_PATH / "../Source/Savegame"
SCHEMAS_PATH  = SAVEGAME_PATH / "Schemas"
OUTPUT_PATH   = SAVEGAME_PATH / "Generated"

def generate_savegame_headers():
    """
    Run `flatc` to generate savegame FlatBuffers headers.
    """
    try:
        print("Generating savegame FlatBuffers headers...")

        # Setup.
        flatc_exe = get_flatc_executable()
        os.makedirs(OUTPUT_PATH, exist_ok=True)

        # Collect all schema files at `SCHEMAS_PATH`.
        schema_files = [
            os.path.join(SCHEMAS_PATH, file)
            for file in os.listdir(SCHEMAS_PATH) if file.endswith(".fbs")
        ]
        schema_files = sorted(schema_files)

        # Run generation command.
        command = [flatc_exe, "--cpp", "-o", OUTPUT_PATH, "-I", SCHEMAS_PATH] + schema_files
        result  = subprocess.run(command, capture_output=True)
        if result.returncode != 0:
            print(f"Command error: {result.stderr.decode()}")
            sys.exit(result.returncode)

        # Enforce LF end of line sequence.
        for root, _, files in os.walk(OUTPUT_PATH):
            for file in files:
                if file.endswith(".h"):
                    filePath = os.path.join(root, file)

                    with open(filePath, "rb") as f:
                        content = f.read()

                    content = content.replace(b"\r\n", b"\n")
                    with open(filePath, "wb") as f:
                        f.write(content)

        # Report status.
        print("Finished.")
    except Exception as ex:
        print(f"Error: {ex}")
        sys.exit(1)

def get_flatc_executable():
    """
    Get the path to the appropriate `flatc` executable based on the system OS.
    """
    # Setup.
    system_os = platform.system()

    # Define executable path corresponding for current platform.
    if system_os == "Windows":
        flatc_exe = os.path.join(FLATC_PATH, "Windows", FLATC_NAME + ".exe")
    elif system_os == "Linux":
        flatc_exe = os.path.join(FLATC_PATH, "Linux", FLATC_NAME)
    elif system_os == "Darwin": # macOS.
        flatc_exe = os.path.join(FLATC_PATH, "MacOs", FLATC_NAME)
    else:
        raise Exception(f"'{system_os}' is unsupported.")

    if not os.path.isfile(flatc_exe):
        raise Exception(f"`{FLATC_NAME}` executable not found at '{flatc_exe}'.")

    return flatc_exe

generate_savegame_headers()
