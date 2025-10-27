#pragma once

#include <cstdint>

namespace Silent
{
    using byte   = char;
    using uchar  = unsigned char;
    using ushort = unsigned short;
    using uint   = unsigned int;
    using ulong  = unsigned long;
    using int8   = int8_t;
    using int16  = int16_t;
    using int32  = int32_t;
    using int64  = int64_t;
    using uint8  = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;

    using q0_8   = int8;
    using q11_4  = int16;
    using q7_8   = int16;
    using q3_12  = int16;
    using q27_4  = int32;
    using q25_6  = int32;
    using q23_8  = int32;
    using q19_12 = int32;
    using q8_8   = uint16;
    using q24_8  = uint32;
    using q20_12 = uint32;

    using s8  = int8_t;
    using s16 = int16_t;
    using s32 = int32_t;
    using s64 = int64_t;
    using u8  = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    constexpr int NO_VALUE = -1;
}
