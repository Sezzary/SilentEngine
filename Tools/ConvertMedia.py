"""
Audio and video media converter.

Converts an `XA` (audio) or `STR` (video) file to a usable format.
"""


import logging
import shutil
import subprocess
import sys

from argparse      import ArgumentParser, FileType
from pathlib       import Path
from static_ffmpeg import run


MPG_EXT = ".MPG"
STR_EXT = ".STR"
WAV_EXT = ".WAV"
XA_EXT  = ".XA"


def _create_parser():
    """
    Create an argument parser for the script.

    :return: An argument parser.
    """
    parser = ArgumentParser()
    parser.add_argument("--mediaFile", "-mf", type=FileType("rb"), help="Path to an `XA` or `STR` media file.")
    parser.add_argument("outputFolder", type=Path, help="Path to the folder where processed media will be saved.")
    return parser


def _get_ffmpeg_cmd():
    """
    Get the platform-specific system FFmpeg command.
    If FFmpeg is missing, it will fall back on a static download.

    :return: An FFmpeg command.
    """
    ffmpeg_exe = shutil.which("ffmpeg")
    if ffmpeg_exe:
        return ffmpeg_exe

    ffmpeg_exe = run.get_or_fetch_platform_executables_else_raise()
    return ffmpeg_exe


def _convert_xa_to_wav(ffmpeg_cmd: str, output_folder: Path, xa_file: Path):
    """
    Convert an `XA` audio file to `WAV`.

    :param ffmpeg_cmd: Path to the `ffmpeg` executable.
    :param output_folder: Directory where the `WAV` file will be saved.
    :param xa_file: Source `XA` file to process.
    """
    new_file = output_folder / f"{xa_file.stem}{WAV_EXT}"

    # Run command.
    command = [
        ffmpeg_cmd, "-y",
        "-hide_banner",
        "-loglevel", "error",
        "-i", xa_file,
        new_file
    ]
    result = subprocess.run(command)

    # Report status.
    if result.returncode != 0:
        logging.error(f"`XA` asset conversion failed: {result.stderr.decode()}")


def _convert_str_to_mpg(ffmpeg_cmd: str, output_folder: Path, str_file: Path):
    """
    Convert an `STR` video file to `MPG`.

    :param ffmpeg_cmd: Path to the `ffmpeg` executable.
    :param output_folder: Directory where the `MPG` file will be saved.
    :param str_file: Source `STR` file to process.
    """
    newFile = output_folder / f"{str_file.stem}{MPG_EXT}"

    # Run command.
    command = [
        ffmpeg_cmd, "-y",
        "-hide_banner",
        "-loglevel", "error",
        "-i", str_file,
        "-r", "30",
        "-c:v", "mpeg1video",
        "-q:v", "1",
        "-bf", "0",
        "-maxrate:v", "1500k",
        "-bufsize:v", "1835k",
        "-vf", "format=yuv420p",
        "-c:a", "mp2",
        "-ar", "44100",
        "-ac", "2",
        "-f", "mpeg",
        newFile
    ]
    result = subprocess.run(command)

    # Report status.
    if result.returncode != 0:
        logging.error(f"`STR` asset conversion failed: {result.stderr.decode()}")


def main():
    try:
        logging.basicConfig(level=logging.INFO)

        # Setup.
        args       = _create_parser().parse_args()
        ffmpeg_cmd = _get_ffmpeg_cmd()

        logging.info(f"Converting `{args.mediaFile}`...")

        # Create folder.
        args.outputFolder.mkdir(parents=True, exist_ok=True)

        # Convert media file.
        if args.mediaFile.suffix == XA_EXT:
            _convert_xa_to_wav(ffmpeg_cmd, args.outputFolder, args.mediaFile)
        elif args.mediaFile.suffix == STR_EXT:
            _convert_str_to_mpg(ffmpeg_cmd, args.outputFolder, args.mediaFile)
    except Exception as ex:
        logging.error(f"Error: {ex}")
        sys.exit(1)


if __name__ == "__main__":
    main()
