"""
Source: https://github.com/Vatuu/silent-hill-decomp/tree/master/tools/silentassets
Modified by Sezz, 2026

Silent Hill Asset Extractor

Extracts assets from a Silent Hill PSX ROM.
Supports final releases, partial demos, and prototypes.

Usage:
    `python Tools/ExtractAssets.py <outputFolder> [-exe <path>] [-fs <path>] [-fh <path>] [-c]`

Arguments:
    outputFolder        : Path to the directory where extracted assets will be saved.
    --executable,  -exe : Path to the main game executable (SLUS/SLES/SLPM).
    --silentFile,  -fs  : Path to the `.SILENT` archive.
    --hillFile,    -fh  : Path to the `.HILL` archive.
    --exeChecksum, -c   : Verify the integrity of the executable without extracting.
"""


import logging
import os

from argparse    import ArgumentParser, FileType
from dataclasses import dataclass
from enum        import IntFlag
from itertools   import chain
from pathlib     import Path
from struct      import Struct
from typing      import BinaryIO, Iterable
from zlib        import crc32


class RomFlags(IntFlag):
    NONE                 = 0
    ENCRYPTED_1ST_FOLDER = 1 << 0
    NO_XA_CONTAINER      = 1 << 1
    ALT_FILE_STRUCT      = 1 << 2


class SubmodeFlags(IntFlag):
    NONE  = 0
    VIDEO = 1 << 1
    AUDIO = 1 << 2
    DATA  = 1 << 3


@dataclass
class Release:
    id:         str
    name:       str
    checksum:   int
    toc_offset: int
    fileCount:  int
    dirs:       list[str]
    filetypes:  list[str]
    flags:      RomFlags


@dataclass
class TableEntry:
    path:   Path
    type:   str
    size:   int
    offset: int


FILESIZE_STEP      = 256
MODE_1_SECTOR_SIZE = 2048
MODE_2_SECTOR_SIZE = 2336

ENTRY_STRUCT      = Struct("<3I")
CD_XA_SYNC_HEADER = b'\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x00' + b'\x00\x00\x00\x02'

# Description of each file type.
FILE_TYPE_NAMES = {
    "TIM": "Texture Image",
    "VAB": "Sound Bank",
    "BIN": "Overlay Data",
    "DMS": "Cutscene Data",
    "ANM": "Animation Data",
    "PLM": "Map Geometry Data",
    "IPD": "Map Data",
    "ILM": "Character Data",
    "TMD": "Mesh Data",
    "DAT": "Demo Data",
    "KDT": "Audio Metadata",
    "CMP": "Compressed Data",
    "TXT": "Dummy Plaintext",
    "UU1": "Unused 1",
    "UU2": "Unused 2",
    "UU3": "Unused 3",
    "UU4": "Unused 4",
    "XA":  "CD Track Data"
}

# Mapping of type index -> type string. Mappings change in certain releases.
FILE_TYPES = [
    "TIM", "VAB", "BIN", "DMS", "ANM", "PLM", "IPD", "ILM", 
    "TMD", "DAT", "KDT", "CMP", "TXT", "UU1", "UU2", "XA"
]

FILE_TYPES_DEMO = [
    "TIM", "VAB", "BIN", "ANM", "DMS", "PLM", "IPD", "ILM", 
    "TMD", "DAT", "KDT", "CMP", "TXT", "UU1", "UU2", "XA"
]

FILE_TYPES_OPM16 = [
    "TIM", "VAB", "BIN", "ANM", "DMS", "PLM", "IPD", "ILM", 
    "TMD", "KDT", "CMP", "UU1", "UU2", "UU3", "UU4", "XA"
]

# Mapping of directory index -> dir string. Mappings change in certain releases.
DIRS_NTSC = [
    "1ST", "ANIM", "BG", "CHARA", "ITEM", "MISC", "SND", "TEST", 
    "TIM", "VIN", "XA"
]

DIRS_PAL = [
    "1ST", "ANIM", "BG", "CHARA", "ITEM", "MISC", "SND", "TEST", 
    "TIM", "VIN", "VIN2", "VIN3", "VIN4", "VIN5", "XA"
]

DIRS_DEMO = [
    "1ST", "ANIM", "BG", "CHARA", "ITEM", "MISC", "SND", "TIM",
    "VIN", "XA"
]

DIRS_OPM16 = [
    "1ST", "ANIM", "BG", "CHARA", "ITEM", "SND", "TEST", "TIM", 
    "VIN", "XA"
]

RELEASES = (
    # Final releases.
    Release("NTSC-J Rev 0 99-01-26",       "SLPM-86192", 0x1532C55C, 0xB91C, 2074, DIRS_NTSC, FILE_TYPES, RomFlags.ENCRYPTED_1ST_FOLDER),
    Release("NTSC 1.1 99-02-10",           "SLUS-00707", 0xCF9CD8E5, 0xB91C, 2074, DIRS_NTSC, FILE_TYPES, RomFlags.ENCRYPTED_1ST_FOLDER),
    Release("NTSC-J Rev 1/Rev 2 99-06-02", "SLPM-86192", 0xEB733CAA, 0xB91C, 2074, DIRS_NTSC, FILE_TYPES, RomFlags.ENCRYPTED_1ST_FOLDER),
    Release("PAL 99-06-07",                "SLES-01514", 0x337E4A60, 0xB8FC, 2310, DIRS_PAL,  FILE_TYPES, RomFlags.ENCRYPTED_1ST_FOLDER),

    # Partial demo releases.
    Release("NTSC OPM16 Demo 98-10-19", "SCUS-94278", 0x2534D4BE, 0xAA90, 886,  DIRS_OPM16, FILE_TYPES_OPM16, RomFlags.ALT_FILE_STRUCT),
    Release("NTSC-J Demo 98-11-06",     "SLPM-80366", 0x1BCE20E4, 0xB9B0, 849,  DIRS_DEMO,  FILE_TYPES_DEMO,  RomFlags.ALT_FILE_STRUCT),
    Release("NTSC-J Demo 98-11-06",     "SLPM-80363", 0xBE8E95CF, 0xB780, 849,  DIRS_DEMO,  FILE_TYPES_DEMO,  RomFlags.ALT_FILE_STRUCT),
    Release("PAL Demo 98-12-16",        "SLED-01735", 0x9AE067D4, 0xB648, 850,  DIRS_DEMO,  FILE_TYPES_DEMO,  RomFlags.NONE),
    Release("NTSC Demo 99-01-16",       "SLUS-90050", 0x55E85F78, 0xB648, 849,  DIRS_DEMO,  FILE_TYPES_DEMO,  RomFlags.NONE),
    Release("PAL Demo 99-06-08",        "SLED-02190", 0x08E0B752, 0xC8FC, 1015, DIRS_PAL,   FILE_TYPES,       RomFlags.ENCRYPTED_1ST_FOLDER),
    Release("PAL Demo 99-06-16",        "SLED-02186", 0x1782AA0A, 0xB8FC, 1015, DIRS_PAL,   FILE_TYPES,       RomFlags.ENCRYPTED_1ST_FOLDER),

    # Prototypes.
    Release("NTSC Preview 98-11-24",    "SLUS-45678", 0x8B88326C, 0xB71C, 1926, DIRS_NTSC, FILE_TYPES, RomFlags.NO_XA_CONTAINER),
    Release("NTSC Review 99-01-07",     "SLUS-00707", 0xCC454534, 0xB7B8, 2076, DIRS_NTSC, FILE_TYPES, RomFlags.NO_XA_CONTAINER),
    Release("NTSC Trade Demo 99-01-17", "SLUS-80707", 0xC9FFEF2A, 0xB850, 2040, DIRS_NTSC, FILE_TYPES, RomFlags.NONE),
    Release("NTSC Beta 99-01-22",       "SLUS-00707", 0x84AB9750, 0xB850, 2072, DIRS_NTSC, FILE_TYPES, RomFlags.NONE)
)

# Mappings of file renames. 
# @todo Renames will shift the file order and can only be done when reliance on the file table is no longer necessary.
FILE_RENAMES = {
    "CHARA/MTH.ILM":  "CHARA/MOTH.ILM",
    "CHARA/WRM.ILM":  "CHARA/WORM.ILM",
    "CHARA/BIRD.ILM": "CHARA/REBIRD.ILM",
    "TEST/EI.TIM":    "CHARA/EI.TIM",
    "TEST/DEV.TIM":   "CHARA/MAN.TIM"
}


def _create_parser():
    """
    Create an argument parser for the script.
    """
    parser = ArgumentParser()
    parser.add_argument("--executable", "-exe", type=FileType("rb"))
    parser.add_argument("--silentFile", "-fs", type=FileType("rb"))
    parser.add_argument("--hillFile", "-fh", type=FileType("rb"))
    parser.add_argument("outputFolder", type=Path, default=".")
    parser.add_argument("--exeChecksum", "-c", action="store_true")
    return parser


def _get_checksum(exe: BinaryIO):
    """
    Generate a SHA-1 checksum from a release executable.

    :param exe: Executable to use.
    """
    exe.seek(0)
    return crc32(exe.read(4096))


def _detect_release(checksum: int, name: str) -> Release:
    """
    Detect if a release executable is supported by the script.

    :param checksum: Checksum of the release.
    :param exe: Name of the release executable.
    """
    for release in RELEASES:
        if checksum == release.checksum:
            logging.info(f"Determined the release as {release.id} ({release.name}).")
            return release

    logging.error(f"Executable `{name}` has an unrecognized checksum: [{checksum:08X}]")
    logging.error("\tIt is not a supported Silent Hill executable.")
    return None


def _parse_entry(entry, release):
    meta, file_0, file_1 = ENTRY_STRUCT.unpack(entry)

    if not release.flags & RomFlags.ALT_FILE_STRUCT:
        name = "".join(chain(
            (chr(32 + ((file_0 >> shift) & 0x3F)) for shift in range(4, 28, 6)),
            (chr(32 + ((file_1 >> shift) & 0x3F)) for shift in range(0, 24, 6))
        )).strip()

        # size, lba, name, path, type
        return meta >> 19, meta & 0x7FFFF, name, release.dirs[file_0 & 0xF], release.filetypes[(file_1 >> 24) & 0xF]
    else:
        dir_idx_0 = (meta >> 31) & 0x1
        dir_idx_1 = file_0 & 0x7

        name_0 = (file_0 >> 3) & 0x1FFFFFFF
        name_1 = file_1 & 0x7FFFF

        ext_idx = (file_1 >> 19) & 0xFF
        dir_idx = (dir_idx_1 << 1) | dir_idx_0

        name_bits = (name_1 << 29) | name_0

        name = "".join(
            chr(32 + ((name_bits >> shift) & 0x3F))
            for shift in range(0, 48, 6)
        ).strip()

        # size, lba, name, path, type
        return (meta >> 19) & 0xFFF, meta & 0x7FFFF, name, release.dirs[dir_idx], release.filetypes[ext_idx]


def _format_entry(size, lba, name, path, type, release):
    name    = name.ljust(8)
    namesep = ','.join(f"'{name[i]}'" for i in range(0, len(name)))

    return f'{{ {lba:#07x}, {size:4d}, {release.dirs.index(path):2d}, FN({namesep}), {release.filetypes.index(type):2d} }}'


def _decrypt_overlay(data: bytes):
    output       = bytearray(data)
    output_array = memoryview(output).cast("I") # `uint32`
    seed         = 0

    for i, value in enumerate(output_array):
        seed             = (seed + 0x01309125) & 0xFFFFFFFF
        seed             = (seed * 0x03A452f7) & 0xFFFFFFFF
        output_array[i] ^= seed

    return output


def _decompress_lzss_file(data: bytes) -> bytes:
    """
    LZSS decompressor based on JP1.0 0x800CC924 code.
    Used (pointlessly) to compress the encrypted `HP_SAFE1.BIN`/`S__SAFE2.BIN` files, making them larger than the
    uncompressed versions. Also used (pointlessly) to compress the .CMP files in the `TEST` folder, which also have the
    uncompressed versions alongside them.
    """
    # Read expected output size from first 4 bytes.
    expected_size = int.from_bytes(data[0:4], byteorder='little')

    input_ptr = 4 # Start after size header.
    output    = bytearray()

    # Ring buffer (Sliding Window)
    window     = bytearray([0] * 4096)
    window_ptr = 0xFEE # Standard initial window pointer.

    tag_register = 0 

    while input_ptr < len(data) and expected_size > len(output):
        # "Tag Reload" Trick.
        # Shift register; if the 'sentinel' bit at 0x100 is gone, 
        # 8 bits have been processed and need a new tag byte.
        tag_register >>= 1

        if (tag_register & 0x100) == 0:
            if input_ptr >= len(data):
                break

            # Load new tag and set sentinel bit at 9th position.
            tag_register = data[input_ptr] | 0xFF00
            input_ptr   += 1

        # LSB contains current flag.
        if tag_register & 0x1:
            # 1 = Literal byte.
            if input_ptr >= len(data):
                break

            byte = data[input_ptr]
            input_ptr += 1

            output.append(byte)
            window[window_ptr] = byte
            window_ptr = (window_ptr + 1) & 0xFFF
        else:
            # 0 = Reference (2 bytes).
            if input_ptr + 1 >= len(data):
                break

            byte_0     = data[input_ptr]
            byte_1     = data[input_ptr + 1]
            input_ptr += 2

            offset = byte_0 | ((byte_1 & 0xF0) << 4)
            length = (byte_1 & 0x0F) + 3

            for _ in range(length):
                byte = window[offset]
                output.append(byte)

                # Write to window and move `window_ptr`.
                window[window_ptr] = byte
                window_ptr         = (window_ptr + 1) & 0xFFF

                # Move reference offset forward (circularly).
                offset = (offset + 1) & 0xFFF

    return bytes(output)


def _extract_cd_stream(data: bytes, base_path: Path):
    # Check for `XaFlags.VIDEO` or `XaFlags.DATA` flag to determine if format should be `STR` (video) or `XA` (audio).
    is_video_stream = False
    for i in range(0, len(data), MODE_2_SECTOR_SIZE):
        if i + 2 < len(data):
            submode = data[i + 2]
            if submode & (SubmodeFlags.VIDEO | SubmodeFlags.DATA):
                is_video_stream = True
                break

    # Set output path.
    ext         = ".STR" if is_video_stream else ".XA"
    output_path = base_path.with_name(f"{base_path.name}{ext}")
    logging.info(f"Creating `{output_path.name}`...")

    # Process and combine.
    with output_path.open("wb") as _file:
        for offset in range(0, len(data), MODE_2_SECTOR_SIZE):
            sector = data[offset : offset + MODE_2_SECTOR_SIZE]
            if len(sector) < MODE_2_SECTOR_SIZE:
                break

            submode = sector[2]
            if (submode & (SubmodeFlags.VIDEO | SubmodeFlags.AUDIO | SubmodeFlags.DATA)):
                _file.write(CD_XA_SYNC_HEADER + sector)


def _extract(entries:Iterable[TableEntry], output: Path, file: BinaryIO, sector_size: int, release_flags: int):
    idx = 0
    for i in entries:
        output_path = (output / i.path).absolute()
        if not output_path.parent.exists():
            output_path.parent.mkdir(parents = True)

        logging.info(f"{idx} Extracting {FILE_TYPE_NAMES[i.type]} (.{i.type}) to `{output_path}`...")

        file.seek((i.offset - entries[0].offset) * sector_size)
        size = 0
        if not i.size == 0 and i.type != "XA":
            size = i.size * FILESIZE_STEP
        elif idx + 1 < len(entries):
            size = (entries[idx + 1].offset - i.offset) * sector_size
        else:
            size = -1 # Read until end of file.

        data = file.read(size)

        if i.type == "BIN" and (release_flags & RomFlags.ENCRYPTED_1ST_FOLDER):
            if i.path.startswith("1ST"):
                logging.info(f"\tDecrypting `{i.path}`...")
                data = _decrypt_overlay(data)
            elif "HP_SAFE1" in i.path or "S__SAFE2" in i.path:
                logging.info(f"\tDecompressing/decrypting `{i.path}`...")
                data = _decrypt_overlay(_decompress_lzss_file(data))
        #elif i.type == "CMP":
        #    logging.info(f"\tDecompressing `{i.path}`...")
        #    dec_data = _decompress_lzss_file(data)
        #    output_dec_path_j = output_path.with_name(output_path.name + ".dec")
        #    with output_dec_path_j.open("wb") as _file:
        #        _file.write(dec_data)
        elif i.type == "XA":
            _extract_cd_stream(data, output_path)
            idx += 1
            continue

        with output_path.open("wb") as _file:
            _file.write(data)
        idx = idx + 1


def main():
    logging.basicConfig(level=logging.INFO)

    logging.info("Extracting assets...")

    # Setup.
    args          = _create_parser().parse_args()
    exe: BinaryIO = args.executable
    checksum      = _get_checksum(exe)

    if args.exeChecksum:
        logging.info(f"Checksum of `{exe.name}`: {checksum:08X}")
        return
    else:
        release = _detect_release(checksum, exe.name)
        if release == None:
            return

    origin_text    = f"// Generated from `{release.name}` ({release.id}).\n"
    header_text    = origin_text
    enum_text      = f"    {origin_text}"
    entries_silent = []
    entries_hill   = []
    exe.seek(release.toc_offset)
    for i in range(release.fileCount):
        rawEntry                   = exe.read(ENTRY_STRUCT.size)
        size, lba, name, dir, type = _parse_entry(rawEntry, release)

        fullPath    = os.path.join(dir, f"{name}.{type}" if not type == "XA" else f"{name}").replace("\\", "/")
        header_text += f"/* {i:4d} */ {_format_entry(size, lba, name, dir, type, release)}, // {fullPath}\n"
        enumName    = fullPath.replace("/", "_").replace("\\", "_").replace(".", "_")
        enum_text   += f"    FILE_{enumName} = {i}, // {fullPath}\n"
        entry       = TableEntry(fullPath, type, size, lba)

        match dir:
            case "XA":
                entries_hill.append(entry)
            case _:
                entries_silent.append(entry)
    exe.close

    _extract(entries_silent, args.outputFolder, args.silentFile, MODE_1_SECTOR_SIZE, release.flags)

    if not release.flags & RomFlags.NO_XA_CONTAINER and args.hillFile:
        _extract(entries_hill, args.outputFolder, args.hillFile, MODE_2_SECTOR_SIZE, release.flags)

    # Generate file table and enum.
    #with open(os.path.join(args.outputFolder, "filetable.c.inc"), "a+") as file:
    #    file.truncate(0)
    #    file.write(header_text)
    #with open(os.path.join(args.outputFolder, "fileenum.h.inc"), "a+") as file:
    #    file.truncate(0)
    #    file.write(enum_text)

    logging.info("Asset extraction completed successfully.")


if __name__ == "__main__":
    main()
