#pragma once

#include <cstdint>

namespace Silent
{
    using byte   = char;
    using uchar  = unsigned char;
    using ushort = unsigned short;
    using uint   = unsigned int;
    using ulong  = unsigned long;
    using char8  = char8_t;
    using char16 = char16_t;
    using char32 = char32_t;
    using int8   = int8_t;
    using int16  = int16_t;
    using int32  = int32_t;
    using int64  = int64_t;
    using uint8  = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;

    using q0_7   = int8;
    using q11_4  = int16;
    using q8_7   = int16;
    using q7_8   = int16;
    using q3_12  = int16;
    using q27_4  = int32;
    using q25_6  = int32;
    using q24_7  = int32;
    using q23_8  = int32;
    using q21_10 = int32;
    using q19_12 = int32;
    using q51_12 = int64;
    using q0_8   = uint8;
    using q8_8   = uint16;
    using q4_12  = uint16;
    using q24_8  = uint32;
    using q20_12 = uint32;
    using q52_12 = uint64;

    using s8  = int8;
    using s16 = int16;
    using s32 = int32;
    using s64 = int64;
    using u8  = uint8;
    using u16 = uint16;
    using u32 = uint32;
    using u64 = uint64;

    constexpr    int  NO_VALUE     = -1;
    static const auto EMPTY_STRING = std::string();

    /** @brief 8-character string usually used for filenames. Can be compared via the `u32` field. */
    typedef union _Filename
    {
        char str[8];
        u32  u32[2];
    } u_Filename;

    /** @brief Compares 8-character filenames using `u32`. Similar to `strcmp`.
     *
     * @param a First filename.
     * @param b Second filename.
     * @return `true` if the filenames aren't equal, `false` otherwise.
     */
    #define COMPARE_FILENAMES(a, b)                                  \
        (((u_Filename*)(a))->u32[0] != ((u_Filename*)(b))->u32[0] || \
         ((u_Filename*)(a))->u32[1] != ((u_Filename*)(b))->u32[1])
}
