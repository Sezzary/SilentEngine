"""
Silent Engine Launcher Generator

Generates a launcher application for the active platform.
If a generated launcher already exists and is outdated, it will be overwritten.
"""


import logging
import os
import platform
import shutil
import subprocess
import sys

from pathlib import Path


BASE_PATH        = Path(__file__).parent
BUILD_PATH       = BASE_PATH / "../Build"
RESOURCES_PATH   = BASE_PATH / "Resources"
TEMP_OUTPUT_PATH = BUILD_PATH / ".temp"
LAUNCHER_SPEC    = BUILD_PATH / "Launcher.spec"
LAUNCHER_SCRIPT  = BASE_PATH / "Launcher.py"


def _get_platform_name():
    """
    Get the name of the active platform.
    """
    system_os = platform.system().lower()
    if system_os == "windows":
        platform_name = "Windows"
    elif system_os == "darwin": # macOS
        platform_name = "macOs"
    elif system_os == "linux":
        platform_name = "Linux"
    else:
        raise Exception(f"'{system_os}' is unsupported.")

    return platform_name

def _get_icon_ext():
    """
    Get the platform-specific application icon extension.
    """
    system_os = platform.system().lower()
    if system_os == "windows":
        ext = ".ico"
    elif system_os == "darwin": # macOS
        ext = ".icns"
    elif system_os == "linux":
        ext = ".png" # @todo Icons must be handled differently on Linux.
    else:
        raise Exception(f"'{system_os}' is unsupported.")

    return ext

def _cleanup():
    """
    Delete temporary build files.
    """
    shutil.rmtree(TEMP_OUTPUT_PATH, ignore_errors=True)
    LAUNCHER_SPEC.unlink(missing_ok=True)

def main():
    try:
        logging.basicConfig(level=logging.INFO)
        _cleanup()

        logging.info("Generating launcher...")

        # Setup.
        system_os     = platform.system().lower()
        platform_name = _get_platform_name()
        icon_ext      = _get_icon_ext()
        exe_ext       = ".exe" if system_os == "windows" else ""
        colon         = ";"    if system_os == "windows" else ":"
        launcher_exes = [
            BUILD_PATH / f"Launcher_d{exe_ext}", # @todo Create separate debug launcher.
            BUILD_PATH / f"Launcher{exe_ext}"
        ]

        # Check if new launcher build is required.
        existing_mtimes = [
            os.path.getmtime(file) for file in launcher_exes
            if os.path.exists(file)
        ]
        newest_exe_mtime = max(existing_mtimes, default=0)
        run_new_build    = os.path.getmtime(LAUNCHER_SCRIPT) > newest_exe_mtime

        # Run generation command.
        if run_new_build:
            command = [
                "pyinstaller",
                "--onefile", "--windowed", "--noconfirm",
                "--icon", RESOURCES_PATH / f"Icon{icon_ext}",
                "--collect-all", "dateutil",
                "--collect-all", "numpy",
                "--collect-all", "static_ffmpeg",
                "--collect-all", "wave",
                "--hidden-import", "charset_normalizer",
                "--add-binary", BASE_PATH / "dumpsxiso" / f"{platform_name}" / f"dumpsxiso{exe_ext}{colon}.",
                "--add-binary", BASE_PATH / "vgmstream-cli" / f"{platform_name}" / f"vgmstream-cli{exe_ext}{colon}.",
                "--add-data", BASE_PATH / f"ExtractAssets.py{colon}.",
                "--add-data", BASE_PATH / f"ConvertMedia.py{colon}.",
                "--add-data", BASE_PATH / f"ConvertMusicSequence.py{colon}.",
                "--add-data", BASE_PATH / "freepats-tools" / f"convertSoundBank.py{colon}.",
                "--add-data", BASE_PATH / "freepats-tools" / f"sf2.py{colon}.",
                "--add-data", BASE_PATH / "freepats-tools" / f"sfz.py{colon}.",
                "--add-data", BASE_PATH / "kdt-tool" / f"kdt-tool.py{colon}.",
                "--distpath", BUILD_PATH,
                "--workpath", TEMP_OUTPUT_PATH,
                "--specpath", BUILD_PATH,
                BASE_PATH / "Launcher.py"
            ]
            result  = subprocess.run(command)

            # Report status and copy launcher to final output folders.
            if result.returncode != 0:
                raise Exception(f"Failed to generate launcher: {result.stderr.decode()}")

            logging.info("Launcher generated successfully.")
        else:
            logging.info("Launcher is up-to-date.")

        _cleanup()
    except Exception as ex:
        _cleanup()

        # Fail.
        logging.error(f"Error: {ex}")
        sys.exit(1)

if __name__ == "__main__":
    main()
