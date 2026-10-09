"""
Source: https://github.com/Nisto/kdt-tool
Modified by Sezz, 2026

kdt-tool

General-purpose tool for Konami's sequenced music format "KDT1" allowing you to:
- Convert a file to MIDI
- Dump event data in a human-readable format

Usage
- Convert to MIDI: `python3 kdt-tool.py -c SOURCE.KDT`
- Dump event data: `python3 kdt-tool.py -l SOURCE.KDT`

Known issues
MIDI conversion is not perfect, but it will get you notes, timing, track configuration and the most basic commands
converted. The remaining events which needs support is likely a subset stemming from Sony's SEQp format and should be
relatively easy to add hopefully, but I have not had the time to really look into it.

So far I have only tested the script with Silent Hill and Suikoden 2. There may be issues in calculating accurate tempos
for other games that use sequence command 0xC7 to set the tempo, as this command appears to be using a
(console-specific?) calculation to compensate for some kind of timing delay(?) Both Silent Hill and Suikoden 2
multiplies the tempo parameter by 2 and adds 2 (e.g. (29 * 2) + 2 = 60). However, using this equation (even for Silent
Hill or Suikoden 2 themselves), the tempo will still sound off when converted to a standard MIDI and played back on any
modern PC. In the KCET driver for Silent Hill 2 (PlayStation 2), the equation was changed to ((x * 2) + 10), which
appears to give a more accurate real-world tempo, so I've decided to use it universally. If needed, change it in the
sources and report your results please. Thanks!
"""


import os
import struct
import sys


class Kdt:
    SIZE_LIMIT         = 50 * 1024 * 1024
    HEADER_SIZE        = 0x10
    FILE_SIZE_OFFSET   = 0x4
    TRACKDIV_OFFSET    = 0x8
    TRACK_COUNT_OFFSET = 0xC
    TRACK_SIZES_OFFSET = 0x10

    def __init__(self, path, log=False, convert=False):
        self.path    = path
        self.log     = log
        self.convert = convert

        # Check valid max file size.
        if os.path.getsize(self.path) > Kdt.SIZE_LIMIT:
            sys.exit("ERROR: File too large: %s" % self.path)

        # Read file buffer.
        with open(self.path, "rb") as kdt:
            self.buffer = kdt.read()

        # Check valid magic and header size.
        if self.buffer[: 4] != b"KDT1" or os.path.getsize(self.path) < Kdt.HEADER_SIZE:
            sys.exit("ERROR: Not a valid KDT1 file: %s" % self.path)

        # Read header.
        self.file_size   = struct.unpack("<I", self.buffer[Kdt.FILE_SIZE_OFFSET   : Kdt.FILE_SIZE_OFFSET   + 4])[0] # 4 bytes.
        self.tickdiv     = struct.unpack("<H", self.buffer[Kdt.TRACKDIV_OFFSET    : Kdt.TRACKDIV_OFFSET    + 2])[0] # 2 bytes.
        # 2 bytes of padding.
        self.track_count = struct.unpack("<H", self.buffer[Kdt.TRACK_COUNT_OFFSET : Kdt.TRACK_COUNT_OFFSET + 2])[0] # 2 bytes.
        # 2 bytes of padding.

        self.bpm = 120

        # Check valid file size.
        if self.file_size > os.path.getsize(self.path):
            sys.exit("ERROR: Indicated filesize exceeds actual filesize: %s" % self.path)

        self.buffer = bytearray(self.buffer[: self.file_size])

        if self.convert:
            self.midi               = bytearray(self.file_size * 4)
            self.midi_output_offset = 0

        if self.track_count > 0:
            self.track_sizes   = []
            self.track_offsets = []

            self.offset = Kdt.TRACK_SIZES_OFFSET + (self.track_count * 2)
            track_sizes = self.buffer[Kdt.TRACK_SIZES_OFFSET : self.offset]

            for track_size in struct.iter_unpack("<H", track_sizes):
                self.track_sizes.append(track_size[0])
                self.track_offsets.append(self.offset)
                self.offset += track_size[0]

            self.set_track(0)

    def set_track(self, track_idx):
        self.track_idx          = track_idx
        self.track_size         = self.track_sizes[track_idx]
        self.track_start_offset = self.track_offsets[track_idx]
        self.track_end_offset   = self.track_start_offset + self.track_size
        self.offset             = self.track_start_offset
        self.time               = 0
        self.is_running         = False
        self.channel            = 0

    def read_sequence(self):
        if self.log:
            print("%-10s" % ("0x%04X" % (self.offset - self.track_start_offset)), end="")

            mm, ss = divmod(self.time * 60 / self.tickdiv / self.bpm, 60)
            print("%-22s" % ("%d (%02d:%07.4f)" % (self.time, int(mm), ss)), end="")

        if not self.is_running:
            if self.log:
                print("%-11s" % "Time", end="")

            self.read_delta_time()
        else:
            if self.buffer[self.offset] & 0x80:
                if self.log:
                    print("%-11s" % "Command", end="")

                self.read_command()
            else:
                if self.log:
                    print("%-11s" % "Key", end="")

                self.read_note()

            # Instead of having delta-times of 0 between events, KDT1 uses the MSB in the command parameter/note
            # velocity to save some bytes.
            if self.convert:
                if self.is_running:
                    if self.offset < self.track_end_offset:
                        self.midi[self.midi_output_offset] = 0
                        self.midi_output_offset           += 1

    def read_delta_time(self):
        if self.convert:
            self.midi[self.midi_output_offset] = self.buffer[self.offset]
            self.midi_output_offset           += 1

        ticks        = self.buffer[self.offset] & 0x7F
        has_more     = self.buffer[self.offset] & 0x80
        self.offset += 1

        while has_more:
            if self.convert:
                self.midi[self.midi_output_offset] = self.buffer[self.offset]
                self.midi_output_offset           += 1

            ticks      <<= 7
            ticks       |= self.buffer[self.offset] & 0x7F
            has_more     = self.buffer[self.offset] & 0x80
            self.offset += 1

        if self.log:
            print("%d" % ticks)

        self.time      += ticks
        self.is_running = False

    def read_note(self):
        self.note       = self.buffer[self.offset]     & 0x7F
        self.velocity   = self.buffer[self.offset + 1] & 0x7F
        self.is_running = self.buffer[self.offset + 1] & 0x80
        self.offset    += 2

        if self.log:
            print("%-11s%s" % ("0x%02X" % self.note,
                               "0x%02X" % self.velocity))

        if self.convert:
            self.midi[self.midi_output_offset + 0] = (0x90 if self.velocity else 0x80) | self.channel
            self.midi[self.midi_output_offset + 1] = self.note
            self.midi[self.midi_output_offset + 2] = self.velocity
            self.midi_output_offset               += 3

    def read_command(self):
        cmd = self.buffer[self.offset] & 0x7F

        param = None
        if cmd == 0x4A:
            self.is_running = False
            self.offset    += 1
        elif cmd == 0x4B:
            self.is_running = True
            self.offset    += 1
        else:
            param           = self.buffer[self.offset + 1] & 0x7F
            self.is_running = self.buffer[self.offset + 1] & 0x80
            self.offset    += 2

        if self.log:
            print("%-11s" % ("0x%02X" % cmd), end="")
            print("%-24s" % (("0x%02X" % param) if param is not None else ""), end="")

        if cmd == 0x1:
            if self.log:
                print("Modulation")

            if self.convert:
                self.midi[self.midi_output_offset + 0] = 0xB0 | self.channel
                self.midi[self.midi_output_offset + 1] = 0x1
                self.midi[self.midi_output_offset + 2] = param
                self.midi_output_offset               += 3

        elif cmd == 0x6:
            if self.log:
                # See PlayStation Technical Reference CD: `\DEVREFS\SOUNDxx.PDF`
                if self.nrpn1 <= 16: # Tone number.
                    if self.nrpn2 == 0: # Attribute number.
                        print("NRPN - Set Priority")
                    elif self.nrpn2 == 1:
                        print("NRPN - Set Mode")
                    elif self.nrpn2 == 2:
                        print("NRPN - Set Limit Low")
                    elif self.nrpn2 == 3:
                        print("NRPN - Set Limit High")
                    elif self.nrpn2 == 4:
                        print("NRPN - Set ADSR (AR-L)")
                    elif self.nrpn2 == 5:
                        print("NRPN - Set ADSR (AR-E)")
                    elif self.nrpn2 == 6:
                        print("NRPN - Set ADSR (DR)")
                    elif self.nrpn2 == 7:
                        print("NRPN - Set ADSR (SL)")
                    elif self.nrpn2 == 8:
                        print("NRPN - Set ADSR (SR-L)")
                    elif self.nrpn2 == 9:
                        print("NRPN - Set ADSR (SR-E)")
                    elif self.nrpn2 == 10:
                        print("NRPN - Set ADSR (RR-L)")
                    elif self.nrpn2 == 11:
                        print("NRPN - Set ADSR (RR-E)")
                    elif self.nrpn2 == 12:
                        print("NRPN - Set ADSR (SR +/-)")
                    elif self.nrpn2 == 13:
                        print("NRPN - Set Vibrate Time")
                    elif self.nrpn2 == 14:
                        print("NRPN - Set Portamento Depth")
                    elif self.nrpn2 == 15:
                        print("NRPN - Set Reverb Type")
                    elif self.nrpn2 == 16:
                        print("NRPN - Set Reverb Depth")
                    elif self.nrpn2 == 17:
                        print("NRPN - Set Echo Feedback")
                    elif self.nrpn2 == 18:
                        print("NRPN - Set Echo Delay Time")
                    elif self.nrpn2 == 19:
                        print("NRPN - Set Delay Delay Time")
                    elif self.nrpn2 == 21:
                        print("NRPN - Set Vibrate Depth")
                    elif self.nrpn2 == 22:
                        print("NRPN - Set Portamento Time")
                    else:
                        print("NRPN - Set Unknown Attribute")
                elif self.nrpn1 == 20:
                    print("NRPN - Set Loop Count")
                elif self.nrpn1 == 40:
                    print("NRPN - Set Mark Callback Value")
                else:
                    print("NRPN Data Entry")

            if self.convert:
                self.midi[self.midi_output_offset + 0] = 0xB0 | self.channel
                self.midi[self.midi_output_offset + 1] = 0x6
                self.midi[self.midi_output_offset + 2] = param
                self.midi_output_offset               += 3

        elif cmd == 0x7:
            if self.log:
                print("Set Volume (Channel)")

            if self.convert:
                self.midi[self.midi_output_offset + 0] = 0xB0 | self.channel
                self.midi[self.midi_output_offset + 1] = 0x7
                self.midi[self.midi_output_offset + 2] = param
                self.midi_output_offset               += 3
        elif cmd == 0xA:
            if self.log:
                print("Set Panning")

            if self.convert:
                self.midi[self.midi_output_offset + 0] = 0xB0 | self.channel
                self.midi[self.midi_output_offset + 1] = 0xA
                self.midi[self.midi_output_offset + 2] = param
                self.midi_output_offset               += 3
        elif cmd == 0xB:
            if self.log:
                print("Set Volume (Expression)")

            if self.convert:
                self.midi[self.midi_output_offset + 0] = 0xB0 | self.channel
                self.midi[self.midi_output_offset + 1] = 0xB
                self.midi[self.midi_output_offset + 2] = param
                self.midi_output_offset               += 3
        elif cmd == 0xF:
            if self.log:
                print("Stereo Widening (?)")

            if self.convert:
                self.midi[self.midi_output_offset : self.midi_output_offset + 22] = b"\xFF\x01\x13Stereo Widening (?)"
                self.midi_output_offset                                          += 22
        elif cmd == 0x40:
            if self.log:
                print("Damper/Sustain Pedal")

            if self.convert:
                self.midi[self.midi_output_offset + 0] = 0xB0 | self.channel
                self.midi[self.midi_output_offset + 1] = 0x40
                self.midi[self.midi_output_offset + 2] = param
                self.midi_output_offset               += 3
        elif cmd == 0x46:
            self.channel = param & 0xF

            if self.log:
                print("Set Channel")

            if self.convert:
                # Tenth channel is "drum channel" and could result in a quiet track in both Awave and fb2k.
                if self.channel >= 9:
                    self.channel = (self.channel + 1) & 0xF

                self.midi[self.midi_output_offset : self.midi_output_offset + 14] = b"\xFF\x01\x0BSet Channel"
                self.midi_output_offset                                          += 14

        elif cmd == 0x47:
            self.bpm = min(10 + (param * 2), 255)
            if self.log:
                print("Set Tempo (10-255 BPM, divisible by two)")

            if self.convert:
                # Microseconds per quarter-note (beat) = microseconds per minute / beats per minute
                mpqn                     = 60000000 // self.bpm
                self.midi[self.midi_output_offset + 0] = 0xFF
                self.midi[self.midi_output_offset + 1] = 0x51
                self.midi[self.midi_output_offset + 2] = 0x3
                self.midi[self.midi_output_offset + 3] = (mpqn >> 16) & 0xFF
                self.midi[self.midi_output_offset + 4] = (mpqn >>  8) & 0xFF
                self.midi[self.midi_output_offset + 5] = (mpqn >>  0) & 0xFF
                self.midi_output_offset               += 6
        elif cmd == 0x48:
            if self.log:
                print("Pitch Bend")

            if self.convert:
                self.midi[self.midi_output_offset + 0] = 0xE0 | self.channel
                self.midi[self.midi_output_offset + 1] = 0 # LSB (cents)
                self.midi[self.midi_output_offset + 2] = param # MSB (semitones)
                self.midi_output_offset               += 3
        elif cmd == 0x49:
            if self.log:
                print("Set Instrument")

            if self.convert:
                self.midi[self.midi_output_offset + 0] = 0xC0 | self.channel
                self.midi[self.midi_output_offset + 1] = param
                self.midi_output_offset               += 2
        elif cmd == 0x4A:
            if self.log:
                print("Note Off Last Note (Reset Running Status)")

            if self.convert:
                self.midi[self.midi_output_offset + 0] = 0x80 | self.channel
                self.midi[self.midi_output_offset + 1] = self.note
                self.midi[self.midi_output_offset + 2] = 0
                self.midi_output_offset               += 3
        elif cmd == 0x4B:
            if self.log:
                print("Note Off Last Note (Sustain Running Status)")

            if self.convert:
                self.midi[self.midi_output_offset + 0] = 0x80 | self.channel
                self.midi[self.midi_output_offset + 1] = self.note
                self.midi[self.midi_output_offset + 2] = 0
                self.midi_output_offset               += 3
        elif cmd == 0x4C:
            self.bpm = param & 0x7F

            if self.log:
                print("Set Tempo (0-127 BPM)")

            if self.convert:
                mpqn = 60000000 // self.bpm
                self.midi[self.midi_output_offset + 0] = 0xFF
                self.midi[self.midi_output_offset + 1] = 0x51
                self.midi[self.midi_output_offset + 2] = 0x3
                self.midi[self.midi_output_offset + 3] = (mpqn >> 16) & 0xFF
                self.midi[self.midi_output_offset + 4] = (mpqn >>  8) & 0xFF
                self.midi[self.midi_output_offset + 5] = (mpqn >>  0) & 0xFF
                self.midi_output_offset               += 6
        elif cmd == 0x4D:
            self.bpm = param | 0x80

            if self.log:
                print("Set Tempo (128-255 BPM)")

            if self.convert:
                mpqn = 60000000 // self.bpm
                self.midi[self.midi_output_offset + 0] = 0xFF
                self.midi[self.midi_output_offset + 1] = 0x51
                self.midi[self.midi_output_offset + 2] = 0x3
                self.midi[self.midi_output_offset + 3] = (mpqn >> 16) & 0xFF
                self.midi[self.midi_output_offset + 4] = (mpqn >>  8) & 0xFF
                self.midi[self.midi_output_offset + 5] = (mpqn >>  0) & 0xFF
                self.midi_output_offset               += 6
        elif cmd == 0x5B:
            if self.log:
                print("Set Reverb Depth")

            if self.convert:
                self.midi[self.midi_output_offset + 0] = 0xB0 | self.channel
                self.midi[self.midi_output_offset + 1] = 0x5B
                self.midi[self.midi_output_offset + 2] = param
                self.midi_output_offset               += 3
        elif cmd == 0x62:
            if self.log:
                print("NRPN (LSB)")

            if self.convert:
                self.midi[self.midi_output_offset : self.midi_output_offset + 13] = b"\xFF\x01\x0ANRPN (LSB)"
                self.midi_output_offset                                          += 13

            self.nrpn2 = param
        elif cmd == 0x63:
            if param == 20:
                if self.log:
                    print("NRPN - Set Loop Start")

                if self.convert:
                    self.midi[self.midi_output_offset : self.midi_output_offset + 13] = b"\xFF\x01\x0ALoop Start"
                    self.midi_output_offset                                          += 13
            elif param == 30:
                if self.log:
                    print("NRPN - Set Loop End")

                if self.convert:
                    self.midi[self.midi_output_offset : self.midi_output_offset + 11] = b"\xFF\x01\x08Loop End"
                    self.midi_output_offset                                          += 11
            elif param == 40:
                if self.log:
                    print("NRPN - Set Mark")

                if self.convert:
                    self.midi[self.midi_output_offset : self.midi_output_offset + 7] = b"\xFF\x01\x04Mark"
                    self.midi_output_offset                                         += 7
            else:
                if self.log:
                    print("NRPN (MSB)")

                if self.convert:
                    self.midi[self.midi_output_offset : self.midi_output_offset + 13] = b"\xFF\x01\x0ANRPN (MSB)"
                    self.midi_output_offset                                          += 13

            self.nrpn1 = param
        elif cmd == 0x76:
            if self.log:
                print("Seq Beat")

            if self.convert:
                self.midi[self.midi_output_offset : self.midi_output_offset + 11] = b"\xFF\x01\x08Seq Beat"
                self.midi_output_offset += 11

        elif cmd == 0x7F:
            if self.log:
                print("End of Track")

            if self.convert:
                self.midi[self.midi_output_offset + 0] = 0xFF
                self.midi[self.midi_output_offset + 1] = 0x2F
                self.midi[self.midi_output_offset + 2] = 0x0
                self.midi_output_offset               += 3

        else:
            if self.log:
                print("Unknown")

            if self.convert:
                self.midi[self.midi_output_offset : self.midi_output_offset + 4] = b"\xFF\x01\x01\x3F"
                self.midi_output_offset                                         += 4


def convert_kdt_to_midi(path):
    kdt = Kdt(path, log=False, convert=True)

    # 0x0: File format identifier.
    kdt.midi[kdt.midi_output_offset : kdt.midi_output_offset + 4] = b"MThd"
    kdt.midi_output_offset                                       += 4

    # 0x4: Size of header: 6.
    kdt.midi[kdt.midi_output_offset : kdt.midi_output_offset + 4] = b"\x00\x00\x00\x06"
    kdt.midi_output_offset                                       += 4

    # 0x8: MIDI type: 1.
    kdt.midi[kdt.midi_output_offset : kdt.midi_output_offset + 2] = b"\x00\x01"
    kdt.midi_output_offset                                       += 2

    # 0xA: Track count.
    kdt.midi[kdt.midi_output_offset : kdt.midi_output_offset + 2] = struct.pack(">H", kdt.track_count)
    kdt.midi_output_offset                                       += 2

    # 0xC: Pulses per quarter-note.
    kdt.midi[kdt.midi_output_offset : kdt.midi_output_offset + 2] = struct.pack(">H", kdt.tickdiv)
    kdt.midi_output_offset                                       += 2

    for track_idx in range(kdt.track_count):
        kdt.set_track(track_idx)

        mtrkoff = kdt.midi_output_offset

        # 0x0: Track chunk identifier.
        kdt.midi[kdt.midi_output_offset : kdt.midi_output_offset + 4] = b"MTrk"
        kdt.midi_output_offset                                       += 4

        # 0x4: Track size (temporary).
        kdt.midi[kdt.midi_output_offset : kdt.midi_output_offset + 4] = b"\x00\x00\x00\x00"
        kdt.midi_output_offset                                       += 4

        # 0x8: Delta time | meta event | meta type: track name | track name length.
        kdt.midi[kdt.midi_output_offset : kdt.midi_output_offset + 4] = b"\x00\xFF\x03\x08"
        kdt.midi_output_offset                                       += 4

        # 0xC: Track name.
        kdt.midi[kdt.midi_output_offset : kdt.midi_output_offset + 8] = bytes("Track %02d" % track_idx, encoding="ascii")
        kdt.midi_output_offset                                       += 8

        while kdt.offset < kdt.track_end_offset:
            kdt.read_sequence()

        kdt.midi[mtrkoff + 4 : mtrkoff + 8] = struct.pack(">I", kdt.midi_output_offset-mtrkoff - 8)

    with open(os.path.splitext(kdt.path)[0] + ".MID", "wb") as midi:
        midi.write(kdt.midi[: kdt.midi_output_offset])


def dump_events(path):
    kdt = Kdt(path, log=True, convert=False)

    for track_idx in range(kdt.track_count):
        kdt.set_track(track_idx)

        print("TRACK %02d (0x%04X)" % (track_idx, kdt.offset))
        print("========================================================================================================================")
        print("Offset    Time                  Event      Value      Parameter / Velocity    Description")
        print()

        while kdt.offset < kdt.track_end_offset:
            kdt.read_sequence()

        print("\n" * 5)


def main(argc=len(sys.argv), argv=sys.argv):

    if argc != 3:
        script_name = os.path.basename(argv[0])
        print("Usage:")
        print("  Convert to MIDI: %s -c <path>" % script_name)
        print("  Dump event data: %s -l <path>" % script_name)
        return True

    path = os.path.realpath(argv[-1])

    if not os.path.isfile(path):
        print("ERROR: Invalid path")
        return True

    if argv[1] == '-c':
        convert_kdt_to_midi(path)
    elif argv[1] == '-l':
        dump_events(path)
    else:
        print("ERROR: Invalid command line")
        return True

    return False


if __name__=="__main__":
    main()
