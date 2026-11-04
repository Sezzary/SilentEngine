#pragma once

namespace Silent::Game
{
    /** @brief Global variable storing the current random seed for the `Rng_Rand32` function. The value is updated with
     * each call to the RNG functions.
     */
    extern uint g_RngSeed;

    /** @brief Tests if a probability is met.
     *
     * Evaluates the probability by performing a bitwise AND
     * operation with a mask that has the specified number of consecutive
     * low bits set to 1.
     *
     * TODO: Deprecated. While this is a common bitwise pattern for efficient probability checks,
     * `Rng_GenerateInt(0, bits - 1)` calls with comments next to them in the following style are preferred:
     * `// 1 in * chance.`
     *
     * @note
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
     *
     * @param bits Number of consecutive bits to use in the probability test.
     * @return `true` if the probability is met, `false` otherwise.
     */
    #define Rng_TestProbabilityBits(bits) \
        (Rng_Rand16() & ((1 << (bits)) - 1))

    /** @brief Tests if a normalized fixed-point Q19.12, integer range `[0, 4096]` probability is met.
     *
     * @param chance Normalized Q19.12 probability value.
     * @return `true` if the probability is met, `false` otherwise.
     */
    bool Rng_TestProbability(q19_12 chance);

    /** @brief Generates an integer in the range `[low, high]` from a random input.
     *
     * @param rand Random base value.
     * @param low Inclusive lower bound.
     * @param high Inclusive upper bound.
     * @return Random integer in the range `[low, high]`.
    */
    int Rng_GenerateIntFromInput(int rand, int low, int high);

    /** @brief Generates an integer in the range `[low, high]` from an unsigned random input.
     *
     * @param rand Random base value.
     * @param low Inclusive lower bound.
     * @param high Inclusive upper bound.
     * @return Random integer in the range `[low, high]`.
    */
    int Rng_GenerateUIntFromInput(int rand, int low, int high);

    /** @brief Shorthand of `Rng_GenerateIntFromInput(Rng_Rand16(), low, high)` .
     *
     * @param low Inclusive lower bound.
     * @param high Inclusive upper bound.
     * @return Random integer in the range `[low, high]`.
    */
    int Rng_GenerateInt(int low, int high);

    /** @brief Shorthand of `Rng_GenerateUIntFromInput(Rng_Rand16(), low, high)` .
     *
     * @param low Inclusive lower bound.
     * @param high Inclusive upper bound.
     * @return Random integer in the range `[low, high]`.
    */
    int Rng_GenerateUInt(int low, int high);

    /** @brief Adds a generated integer in the range `[low, high]` from an unsigned random input to `base`.
     *
     * @param base Base number to be added to.
     * @param low Inclusive lower bound.
     * @param high Inclusive upper bound.
     * @return Random integer in the range `[base + low, base + high]`.
    */
    int Rng_AddGeneratedUInt(int base, int low, int high);

    /** @brief Generates a random normaalized Q19.12 fixed-point number, integer range `[0, 4096)`.
     * Used for angles and multipliers.
     *
     * @return Random Q19.12 fixed-point number.
     */
    q19_12 Rng_RandQ12();

    /** @brief Generates a new random 32-bit unsigned integer and updates
     * `g_RngSeed`.
     *
     * Implements a Linear Congruential Generator (LCG) Random Number Generator (RNG) algorithm, as outlined in
     * "Numerical Recipes" (Second Edition, Chapter 7.1, An Even Quicker Generator).
     * It produces a pseudo-random 32-bit unsigned integer value.
     *
     * @return New pseudo-random 32-bit unsigned integer.
     */
    uint Rng_Rand32();

    /** @brief Generates a new random 16-bit unsigned integer.
     *
     * Calls `Rng_Rand32` to generate a random number, then shifts the result right to produce a value within the range
     * of `[0, 0x7FFF]`.
     *
     * @return Random positive 16-bit unsigned integer as a 32-bit signed integer.
     */
    int Rng_Rand16();

    /** @brief Returns the current random seed value.
     *
     * Retrieves and returns the current value of the global variable `g_RngSeed`.
     *
     * @return Current random seed as a 32-bit unsigned integer.
     */
    uint Rng_GetSeed();

    /** @brief Sets the random seed to a specified value.
     *
     * Updates the global variable `g_RngSeed` with the given seed value.
     *
     * @param newSeed New seed value to be set, as a 32-bit unsigned integer.
     */
    void Rng_SetSeed(uint newSeed);

    /** @brief Generates a new random 12-bit short integer.
     *
     * Calls `Rng_Rand32` to generate a random number, then * shifts the result right to produce a value within the
     * range of `0` to `0xFFF` (12-bit).
     *
     * @return Random 12-bit short integer in the range `[0, 0xFFF]`.
     */
    ushort Rng_Rand12();
}
