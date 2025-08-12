#pragma once

namespace Silent::Math
{
    /** @brief Generates an integer in the range `[low, high]` from a random input. */
    constexpr auto GENERATE_INT = [](auto rand, auto low, auto high)
    {
        return (rand % ((high - low) + 1)) + low;
    };

    /** @brief Tests if a probability is met.
     *
     * This macro evaluates the probability by performing a bitwise AND
     * operation with a mask that has the specified number of consecutive
     * low bits set to 1.
     *
     * Bits | Mask   | Chance     | Percent
     * -----|--------|------------|---------
     * 1    | 0x1    | 1 in 2     | 50%
     * 2    | 0x3    | 1 in 4     | 25%
     * 3    | 0x7    | 1 in 8     | 12.5%
     * 4    | 0xF    | 1 in 16    | 6.25%
     * 5    | 0x1F   | 1 in 32    | 3.125%
     * 6    | 0x3F   | 1 in 64    | 1.563%
     * 7    | 0x7F   | 1 in 128   | 0.781%
     * 8    | 0xFF   | 1 in 256   | 0.391%
     * 9    | 0x1FF  | 1 in 512   | 0.195%
     * 10   | 0x3FF  | 1 in 1024  | 0.098%
     * 11   | 0x7FF  | 1 in 2048  | 0.049%
     * 12   | 0xFFF  | 1 in 4096  | 0.024%
     * 13   | 0x1FFF | 1 in 8192  | 0.012%
     * 14   | 0x3FFF | 1 in 16384 | 0.006%
     * 15   | 0x7FFF | 1 in 32768 | 0.003%
     * 16   | 0xFFFF | 1 in 65536 | 0.002%
     */
    bool TestRng(uint bits);

    /** @brief Generates a new random 32-bit unsigned integer and updates
     * `g_RngSeed`.
     *
     * This function implements a Linear Congruential Generator (LCG) Random Number
     * Generator (RNG) algorithm, as outlined in "Numerical Recipes" (Second
     * Edition, Chapter 7.1, An Even Quicker Generator). It produces a pseudo-random
     * 32-bit unsigned integer value.
     *
     * @return A new pseudo-random unsigned 32-bit integer.
     */
    uint32 GenerateInt32();

    /** @brief Generates a new random 16-bit unsigned integer.
     * 
     * This function calls `GenerateInt32` to generate a random number, then
     * shifts the result right to produce a value within the range
     * of `[0, 0x7FFF]` (16-bit).
     *
     * @return A random 16-bit unsigned integer in the range `[0, 0x7FFF]`.
     */
    uint16 GenerateInt16();

    /** @brief Generates a new random 12-bit unsigned integer.
     *
     * This function calls `GenerateInt32` to generate a random number, then
     * shifts the result right to produce a value within the range
     * of `[0, 0xFFF]` (12-bit).
     *
     * @return A random 12-bit unsigned integer in the range `[0, 0xFFF]`
     */
    uint16 GenerateInt12();

    /** @brief Returns the current random seed value.
     *
     * This function retrieves and returns the current value of the global variable
     * `g_RngSeed`.
     *
     * @return The current random seed.
     */
    uint32 GetSeed();

    /** @brief Sets the random seed to a specified value.
     *
     * This function updates the global variable `g_RngSeed` with the given
     * seed value.
     *
     * @param newSeed The new seed value to be set.
     */
    void SetSeed(uint32 newSeed);
}
