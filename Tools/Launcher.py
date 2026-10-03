"""
Silent Engine Launcher

Runs a simple launcher which prompts ROM selection for asset extraction and starts the Silent Engine application.
"""


import customtkinter
import hashlib
import logging
import multiprocessing
import os
import platform
import shutil
import subprocess
import sys

from customtkinter import filedialog
from pathlib       import Path
from static_ffmpeg import run


DUMPSXISO_NAME          = "dumpsxiso"
VGMSTREAM_NAME          = "vgmstream-cli"
CONVERT_SOUND_BANK_NAME = "convertSoundBank.py"
KDT_TOOL_NAME           = "kdt-tool.py"
EXTRACT_ASSETS_NAME     = "ExtractAssets.py"
CONVERT_MEDIA_NAME      = "ConvertMedia.py"
CONVERT_MUSIC_SEQ_NAME  = "ConvertMusicSequence.py"
BASE_PATH               = Path(sys.executable).parent
TEMP_BASE_PATH          = Path(getattr(sys, '_MEIPASS', os.path.abspath(".")))
ASSETS_PATH             = BASE_PATH / "Assets" / "Stream" / "Psx"
ASSETS_AUDIO_PATH       = BASE_PATH / "Assets" / "Audio"
ASSETS_VIDEO_PATH       = BASE_PATH / "Assets" / "Video"
ASSETS_TRACKS_PATH      = BASE_PATH / "Assets" / "Tracks"

KDT_EXT = ".KDT"
MID_EXT = ".MID"
MPG_EXT = ".MPG"
SF2_EXT = ".SF2"
VAB_EXT = ".VAB"
WAV_EXT = ".WAV"

# Checksums for supported ROMs.
ROM_CHECKSUMS = [
    0x80145C264A974B210E75A020967F10B7F806E22C, # NTSC-J   | SLPM-86192 | Rev 0 99-01-26
    0x34278D31D9B9B12B3B5DB5E45BCBE548991ECBC7, # NTSC 1.1 | SLUS-00707 | 99-02-10
    0x83B8D89398829493457A48BE9820300ADE8703D4, # NTSC-J   | SLPM-86192 | Rev 1/Rev 2 99-06-02
    0xCA67A93E507B999A0040242362162E442F2BA07E  # PAL      | SLES-01514 | 99-06-07
]


def _get_python_cmd():
    """
    Get the platform-specific system Python command.
    """
    system_os = platform.system().lower()
    return "python" if system_os == "windows" else "python3"


def get_ffmpeg_cmd():
    """
    Get the platform-specific system FFmpeg command.
    If FFmpeg is missing, it will fall back on a static download.
    """
    ffmpeg_exe = shutil.which("ffmpeg")
    if ffmpeg_exe:
        return ffmpeg_exe

    ffmpeg_exe = run.get_or_fetch_platform_executables_else_raise()
    return ffmpeg_exe


def _get_dumpsxiso_exe():
    """
    Get the platform-specific `dumpsxiso` executable to use.
    """
    # Define executable path.
    system_os = platform.system().lower()
    if system_os == "windows":
        dumpsxiso_exe = TEMP_BASE_PATH / f"{DUMPSXISO_NAME}.exe"
    elif system_os == "darwin" or system_os == "linux": # `darwin` = macOS.
        dumpsxiso_exe = TEMP_BASE_PATH / DUMPSXISO_NAME
    else:
        raise Exception(f"'{system_os}' is unsupported.")

    if not os.path.isfile(dumpsxiso_exe):
        raise Exception(f"`{DUMPSXISO_NAME}` executable not found at '{dumpsxiso_exe}'.")

    # Set permissions.
    if system_os in ["darwin", "linux"]:
        os.chmod(dumpsxiso_exe, 0o755)

    return dumpsxiso_exe


def _get_vgmstream_exe():
    """
    Get the platform-specific `vgmstream-cli` executable to use.
    """
    # Define executable path.
    system_os = platform.system().lower()
    if system_os == "windows":
        vgmstream_exe = TEMP_BASE_PATH / f"{VGMSTREAM_NAME}.exe"
    elif system_os == "darwin" or system_os == "linux": # `darwin` = macOS.
        vgmstream_exe = TEMP_BASE_PATH / VGMSTREAM_NAME
    else:
        raise Exception(f"'{system_os}' is unsupported.")

    if not os.path.isfile(vgmstream_exe):
        raise Exception(f"`{VGMSTREAM_NAME}` executable not found at '{vgmstream_exe}'.")

    # Set permissions.
    if system_os in ["darwin", "linux"]:
        os.chmod(vgmstream_exe, 0o755)

    return vgmstream_exe


def _get_convert_media_py():
    """
    Get the `ConvertMedia.py` script to use.
    """
    # Define script path.
    convert_media_py = TEMP_BASE_PATH / CONVERT_MEDIA_NAME

    # Set permissions.
    system_os = platform.system().lower()
    if system_os in ["darwin", "linux"]:
        os.chmod(convert_media_py, 0o755)

    return convert_media_py


def _get_convert_music_seq_py():
    """
    Get the `ConvertMusicSequence.py` script to use.
    """
    # Define script path.
    convert_music_seq_py = TEMP_BASE_PATH / CONVERT_MUSIC_SEQ_NAME

    # Set permissions.
    system_os = platform.system().lower()
    if system_os in ["darwin", "linux"]:
        os.chmod(convert_music_seq_py, 0o755)

    return convert_music_seq_py


def _get_convert_sound_bank_py():
    """
    Get the `convertSoundBank.py` script to use.
    """
    # Define script path.
    convert_sound_bank_py = TEMP_BASE_PATH / CONVERT_SOUND_BANK_NAME

    # Set permissions.
    system_os = platform.system().lower()
    if system_os in ["darwin", "linux"]:
        os.chmod(convert_sound_bank_py, 0o755)

    return convert_sound_bank_py


def _get_extract_assets_py():
    """
    Get the `ExtractAssets.py` script to use.
    """
    # Define script path.
    extract_assets_py = TEMP_BASE_PATH / EXTRACT_ASSETS_NAME

    # Set permissions.
    system_os = platform.system().lower()
    if system_os in ["darwin", "linux"]:
        os.chmod(extract_assets_py, 0o755)

    return extract_assets_py


def _get_kdt_tool_py():
    """
    Get the `kdt-tool.py` script to use.
    """
    # Define script path.
    kdt_tool_py = TEMP_BASE_PATH / KDT_TOOL_NAME

    # Set permissions.
    system_os = platform.system().lower()
    if system_os in ["darwin", "linux"]:
        os.chmod(kdt_tool_py, 0o755)

    return kdt_tool_py


def get_checksum(file: Path):
    """
    Generate a SHA-1 checksum from a file.
    """
    sha1_hash = hashlib.sha1()
    with open(file, "rb") as _file:
        for byte_block in iter(lambda: _file.read(4096), b""):
            sha1_hash.update(byte_block)

    return int(sha1_hash.hexdigest(), 16)


def _select_rom_file():
    """
    Show a file selection window to prompt ROM selection.
    """
    file_path = filedialog.askopenfilename(
        title="Select a Silent Hill ROM",
        filetypes=[("Silent Hill ROM Image", "*.bin")])
    
    checksum = get_checksum(file_path)
    if checksum not in ROM_CHECKSUMS:
        print(f"Unsupported ROM with checksum {checksum}.")
        return None

    if file_path:
        print(f"Selected: {file_path}")
        return file_path

    return None


def _dump_rom(rom_path: str):
    """
    Dump a supported Silent Hill ROM.
    Exported to `TEMP_BASE_PATH`.
    """
    logging.info("Dumping ROM...")

    # Setup.
    dumpsxiso_exe = _get_dumpsxiso_exe()

    # Run command.
    command = [
        dumpsxiso_exe,
        "-x", TEMP_BASE_PATH,
        "-s", TEMP_BASE_PATH / "Layout.xml",
        rom_path
    ]

    # Report status.
    result = subprocess.run(command)
    if result.returncode != 0:
        raise Exception(f"ROM dump failed: {result.stderr.decode()}")

    logging.info("ROM dump completed successfully.")


def _extract_assets(rom_exe: str):
    """
    Extract assets from dumped Silent Hill ROM data.
    Exported to `TEMP_BASE_PATH`.
    """
    # Setup.
    python_cmd            = _get_python_cmd()
    extract_assets_script = _get_extract_assets_py()

    # Run command.
    command = [
        python_cmd, extract_assets_script,
        ASSETS_PATH,
        "-exe", TEMP_BASE_PATH / rom_exe,
        "-fs", TEMP_BASE_PATH / "SILENT.",
        "-fh", TEMP_BASE_PATH / "HILL."
    ]
    result = subprocess.run(command)

    # Report status.
    if result.returncode != 0:
        raise Exception(f"Asset extraction failed: {result.stderr.decode()}")


def _convert_audio_and_video():
    """
    Convert `XA` (audio) and `STR` (video) asset files to usable formats.
    Exported to `ASSETS_AUDIO_PATH` and `ASSETS_VIDEO_PATH`.
    """
    logging.info("Converting audio and video...")

    # Setup.
    python_cmd       = _get_python_cmd()
    convert_media_py = _get_convert_media_py()

    # Setup.
    ffmpeg_cmd = get_ffmpeg_cmd()

    # Create folders.
    ASSETS_AUDIO_PATH.mkdir(parents=True, exist_ok=True)
    ASSETS_VIDEO_PATH.mkdir(parents=True, exist_ok=True)

    # Run through `XA` and `STR` files.
    for _file in (ASSETS_PATH / "XA").iterdir():
        # Define output folder.
        if _file.suffix == "XA":
            output_folder = ASSETS_AUDIO_PATH
        elif _file.suffix == "STR":
            output_folder = ASSETS_VIDEO_PATH
        else:
            continue
    
        # Run command.
        command = [
            python_cmd,
            convert_media_py,
            output_folder
        ]
        result = subprocess.run(command)

        # Report status.
        if result.returncode != 0:
            raise Exception(f"Asset conversion failed for file `{_file.name}`: {result.stderr.decode()}")

    logging.info("Audio and video conversion complete.")


def _convert_tracks():
    """
    Convert `KDT` and `VAB` asset files to usable formats.
    Exported to `ASSETS_TRACKS_PATH`
    """
    MOVE_ALL = True # @debug Move entire folder to inspect output.

    logging.info("Converting tracks...")

    # Setup.
    python_cmd            = _get_python_cmd()
    convert_music_seq_py  = _get_convert_music_seq_py()
    vgmstream_exe         = _get_vgmstream_exe()
    kdt_tool_py           = _get_kdt_tool_py()
    convert_sound_bank_py = _get_convert_sound_bank_py()

    # Create folder.
    ASSETS_TRACKS_PATH.mkdir(parents=True, exist_ok=True)

    # Run through `KDT` and `VAB` files.
    for _file in (ASSETS_PATH / "SND").glob(f"*{VAB_EXT}"):
        kdt_file = _file.with_suffix(KDT_EXT)
        vab_file = _file

        has_kdt = kdt_file.exists()
        logging.info(f"Converting `{kdt_file.name}` and `{vab_file.name}`" if has_kdt else f"`{vab_file.name}`...")

        # Run command.
        if has_kdt:
            command = [
                python_cmd,
                convert_music_seq_py,
                "-exe", vgmstream_exe,
                "-ktp", kdt_tool_py,
                "-csp", convert_sound_bank_py,
                "-ikf", kdt_file,
                "-ivf", vab_file,
                TEMP_BASE_PATH / "SND"
            ]
        else:
            command = [
                python_cmd,
                convert_music_seq_py,
                "-exe", vgmstream_exe,
                "-ktp", kdt_tool_py,
                "-csp", convert_sound_bank_py,
                "-ivf", vab_file,
                TEMP_BASE_PATH / "SND"
            ]
        result = subprocess.run(command)

        # Move `MID` and `SF2` files to `ASSETS_TRACKS_PATH`.
        for _folder in (TEMP_BASE_PATH / "SND").iterdir():
            if _folder.is_dir():
                # Clear existing output if it exists.
                dest_folder = ASSETS_TRACKS_PATH / _folder.name
                if dest_folder.exists():
                    shutil.rmtree(dest_folder)

                if MOVE_ALL:
                    shutil.move(_folder, dest_folder)
                else:
                    dest_folder.mkdir(parents=True, exist_ok=True)

                    mid_files = list(_folder.glob(f"*{_folder.stem}{MID_EXT}"))
                    sf2_files = list(_folder.glob(f"*{_folder.stem}{SF2_EXT}"))
                    for _file in mid_files + sf2_files:
                        if _file.exists():
                            # Move file into the newly created folder.
                            shutil.move(_file, dest_folder / _file.name)

        # Report status.
        if result.returncode != 0:
            raise Exception(f"Asset conversion failed for files `{kdt_file.name}` and `{vab_file.name}`" if has_kdt else f"`{vab_file.name}`: {result.stderr.decode()}")

    # @todo Move files, cleanup.

    logging.info("Tracks conversion complete.")


def main():
    try:
        WIDTH  = 500
        HEIGHT = 400

        multiprocessing.freeze_support()
        logging.basicConfig(level=logging.INFO)

        customtkinter.set_appearance_mode("Dark")

        # Create window object.
        root = customtkinter.CTk()
        root.title("Silent Engine Launcher")
        root.resizable(False, False)
        root.geometry(f"{WIDTH}x{HEIGHT}")

        # Add label.
        label = customtkinter.CTkLabel(root, text="Select a Silent Hill ROM.")
        label.pack(expand=True)

        def handle_click():
            # Get ROM path.
            rom_path = _select_rom_file()
            if rom_path:
                label.configure(text=f"Path: ...{rom_path[-30:]}")

                _dump_rom(rom_path)
                _extract_assets("SLUS_007.07") # @todo Dehardcode executable.
                _convert_audio_and_video()
                #_convert_tracks()

        button = customtkinter.CTkButton(root, text="Browse Files", command=handle_click)
        button.pack(expand=True)

        # Run window.
        root.mainloop()
    except Exception as ex:
        logging.error(f"Error: {ex}")
        sys.exit(1)


if __name__ == "__main__":
    main()
