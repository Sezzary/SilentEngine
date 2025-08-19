import os
import platform
import subprocess
import sys
from pathlib import Path

FLATC_NAME   = "flatc"
BASE_PATH    = Path(__file__).parent
FLATC_PATH   = BASE_PATH / FLATC_NAME
SCHEMAS_PATH = BASE_PATH / "../Source/Engine/Services/Savegame/Schemas"
OUTPUT_PATH  = BASE_PATH / "../Source/Engine/Services/Savegame/Generated"

"""
Run `flatc` to generate FlatBuffers headers.
"""
def generate_savegame_headers():
    try:
        print("Generating savegame FlatBuffers headers...")

        flatc_exe = get_flatc_executable()
        os.makedirs(OUTPUT_PATH, exist_ok=True)

        # Collect all schema files at `SCHEMAS_PATH`.
        schema_files = [
            os.path.join(SCHEMAS_PATH, file) 
            for file in os.listdir(SCHEMAS_PATH) 
            if file.endswith(".fbs")
        ]

        # Build generation command.
        command = [flatc_exe, "--cpp", "-o", OUTPUT_PATH, "-I", SCHEMAS_PATH] + schema_files

        # Run generation command.
        result = subprocess.run(command, capture_output=True)
        if result.returncode != 0:
            print(f"Error running `flatc`: {result.stderr.decode()}")
            sys.exit(result.returncode)

        print("Savegame FlatBuffers header generation complete.")
    except Exception as ex:
        print(f"Error: {ex}")
        sys.exit(1)

"""
Get the path to the appropriate `flatc` executable based on the system OS.
"""
def get_flatc_executable():
    system_os = platform.system()

    # Define OS-specific executable path.
    flatc_exe = Path()
    if system_os == "Windows":
        flatc_exe = os.path.join(FLATC_PATH, "Windows", FLATC_NAME + ".exe")
    elif system_os == "Linux":
        flatc_exe = os.path.join(FLATC_PATH, "Linux", FLATC_NAME)
    elif system_os == "Darwin": # MacOS.
        flatc_exe = os.path.join(FLATC_PATH, "MacOs", FLATC_NAME)
    else:
        raise Exception(f"Unsupported OS '{system_os}'.")

    if not os.path.isfile(flatc_exe):
        raise Exception(f"`flatc` executable not found at '{flatc_exe}'.")

    return flatc_exe

if __name__ == "__main__":
    generate_savegame_headers()
