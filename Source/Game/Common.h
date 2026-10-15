#pragma once

namespace Silent::Game
{
    /** @brief Computes the size of an array.
     *
     * @param arr Array.
     * @return Element count.
     */
    #define ARRAY_SIZE(arr) \
        (s32)(sizeof(arr) / sizeof((arr)[0]))
}
