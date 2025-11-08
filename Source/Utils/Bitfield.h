#pragma once

namespace Silent::Utils
{
    class Bitfield
    {
    public:
        // ========
        // Aliases
        // ========

        using ChunkType = uint;

    private:
        // ==========
        // Constants
        // ==========

        static constexpr uint DEFAULT_SIZE = 32;
        static constexpr uint CHUNK_SIZE   = std::numeric_limits<ChunkType>::digits;

        // =======
        // Fields
        // =======

        uint                   _size   = 0;  /** Number of bits. */
        std::vector<ChunkType> _chunks = {}; /** Integers containing the bits. */

    public:
        // ========
        // Presets
        // ========

        static const Bitfield Empty;
        static const Bitfield Default;

        // =============
        // Constructors
        // =============

        /** @brief Constructs a default `Bitfield` with a size of 32. */
        Bitfield();

        /** @brief Constructs a `Bitfield` of the specified size.
         *
         * @param size Number of bits.
         */
        Bitfield(uint size);

        /** @brief Constructs a `Bitfield` with the specified bits.
         *
         * @param bits Consecutive bit states.
         */
        Bitfield(const std::initializer_list<bool>& bits);

        /** @brief Constructs a `Bitfield` from specified chunks with a size.
         *
         * @param bitChunks Chunks containing bits.
         * @param size Number of bits.
         */
        Bitfield(const std::vector<ChunkType>& bitChunks, uint size);

        /** @brief Constructs a `Bitfield` from a string of `1`s (`true`) and `0`s (`false`).
         *
         * @param bitStr Bits encoded in a string.
         */
        Bitfield(const std::string& bitStr);

        // ========
        // Getters
        // ========

        /** @brief Gets number of bits.
         *
         * @return Number of bits.
         */
        uint GetSize() const;

        /** @brief Gets the number of bits set to `true`.
         *
         * @return Number of set bits.
         */
        uint GetCount() const;

        /** @brief Gets the chunks of the bitfield.
         *
         * @return Bit chunks.
         */
        const std::vector<ChunkType>& GetChunks() const;

        // ========
        // Setters
        // ========

        /** @brief Sets the state of a specified bit to `true`.
         *
         * @param bitIdx Index of the bit to set.
         */
        void Set(uint bitIdx);

        /** @brief Sets the states of specified bits to `true`.
         *
         * @param bitIdxs Indices of the bits to set.
         */
        void Set(const std::vector<uint>& bitIdxs);

        /** @brief Sets the states of specified bits to `true`.
         *
         * @param bitIdxs Indices of the bits to set.
         */
        template <typename... Args>
        requires (std::is_integral_v<Args> && ...)
        void Set(Args... bitIdxs);

        /** @brief Sets the states of all bits to `true`. */
        void SetAll();

        /** @brief Sets the state of a specified bit to `false`.
         *
         * @param bitIdx Index of the bit to clear.
         */
        void Clear(uint bitIdx);

        /** @brief Sets the states of specified bits to `false`.
         *
         * @param bitIdxs Indices of the bits to clear.
         */
        void Clear(const std::vector<uint>& bitIdxs);

        /** @brief Sets the states of specified bits to `false`.
         *
         * @param bitIdxs Indices of the bits to clear.
         */
        template <typename... Args>
        requires (std::is_integral_v<Args> && ...)
        void Clear(Args... bitIdxs);

        /** @brief Sets the states of all bits to `false`. */
        void ClearAll();

        /** @brief Flips the state of a specified bit.
         *
         * @param bitIdx Index of the bit to flip.
         */
        void Flip(uint bitIdx);

        /** @brief Flips the states of specified bits.
         *
         * @param bitIdxs Indices of the bits to flip.
         */
        void Flip(const std::vector<uint>& bitIdxs);

        /** @brief Flips the states of specified bits.
         *
         * @param bitIdxs Indices of the bits to flip.
         */
        template <typename... Args>
        requires (std::is_integral_v<Args> && ...)
        void Flip(Args... bitIdxs);

        /** @brief Flips the states of all bits. */
        void FlipAll();

        // ==========
        // Inquirers
        // ==========

        /** @brief Checks if all bits are set to `false`.
         *
         * @return `true` if all bits are `false`, `false` otherwise.
         */
        bool IsEmpty() const;

        /** @brief Checks if specified bits are set to `true`. If `testAny` is passed as `true`, any set bit will satisfy the condition.
         *
         * @param bitIdxs Indices of the bits to check.
         * @param testAny If any set bit will satisfy the condition.
         * @return `true` if the bits are set, `false` otherwise.
         */
        bool Test(const std::vector<uint>& bitIdxs, bool testAny = true) const;

        /** @brief Checks if a specified bit is set to `true`.
         *
         * @param bitIdxs Index of the bit to check.
         * @param `true` if the bit is set, `false` otherwise.
         */
        bool Test(uint bitIdx) const;

        /** @brief Checks if any bits are set to `true`.
         *
         * @return `true` if any bits are set, `false` otherwise.
         */
        bool TestAny() const;

        /** @brief Checks if all bits are set to `true`.
         *
         * @return `true` if all bits are set, `false` otherwise.
         */
        bool TestAll() const;

        // ==========
        // Utilities
        // ==========

        /** @brief Resizes the bitfield.
         *
         * @param size New size.
         */
        void Resize(uint size);

        // ===========
        // Converters
        // ===========

        /** @brief Converts the bitfield to a string of `1`s (`true`) and `0`s (`false`).
         *
         * @return String representing the bits.
         */
        std::string ToString() const;

        // ==========
        // Operators
        // ==========

        bool      operator==(const Bitfield& bitField) const;
        bool      operator!=(const Bitfield& bitField) const;
        Bitfield& operator=(const Bitfield& bitField) = default;
        Bitfield& operator&=(const Bitfield& bitField);
        Bitfield& operator|=(const Bitfield& bitField);
        Bitfield& operator^=(const Bitfield& bitField);
        Bitfield  operator&(const Bitfield& bitField) const;
        Bitfield  operator|(const Bitfield& bitField) const;
        Bitfield  operator^(const Bitfield& bitField) const;
        Bitfield  operator~() const;

    private:
        // ========
        // Helpers
        // ========

        /** @brief Fills all bits with a specified state.
         *
         * @param state Bit state to fill.
         */
        void Fill(bool state);

        /** @brief Checks if a specified bit exists.
         *
         * @param bitIdx Index of the bit to check.
         * @return `true` if the bit exists, `false` otherwise.
         */
        bool IsBitIdxValid(uint bitIdx) const;
    };

    template <typename... Args>
    requires (std::is_integral_v<Args> && ...)
    void Bitfield::Set(Args... bitIdxs)
    {
        (Set(bitIdxs), ...);
    }

    template <typename... Args>
    requires (std::is_integral_v<Args> && ...)
    void Bitfield::Clear(Args... bitIdxs)
    {
        (Clear(bitIdxs), ...);
    }

    template <typename... Args>
    requires (std::is_integral_v<Args> && ...)
    void Bitfield::Flip(Args... bitIdxs)
    {
        (Flip(bitIdxs), ...);
    }
}

namespace std
{
    template<>
    struct hash<Silent::Utils::Bitfield>
    {
        size_t operator()(const Silent::Utils::Bitfield& bitField) const
        {
            size_t seed = 0;
            for (auto chunk : bitField.GetChunks())
            {
                seed ^= hash<Silent::Utils::Bitfield::ChunkType>{}(chunk) +
                        Silent::Math::GOLDEN_RATIO +
                        (seed << 6) + (seed >> 2);
            }

            return seed;
        }
    };
}
