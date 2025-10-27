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
        // Presets

        static const Bitfield Empty;
        static const Bitfield Default;

        // Constructors

        Bitfield();
        Bitfield(uint size);
        Bitfield(const std::initializer_list<bool>& bits);
        Bitfield(const std::vector<ChunkType>& bitChunks, uint size);
        Bitfield(const std::string& bitString);
        
        // ========
        // Getters
        // ========

        /** @brief Gets number of bits.
         *
         * @return Number of bits.
         */
        uint GetSize() const;

        /** @brief Gets the number of set bits.
         *
         * @return Number of set bits.
         */
        uint GetCount() const;

        /** @brief Gets the chunks of the bitfield.
         *
         * @return `std::vector` of chunks.
         */
        const std::vector<ChunkType>& GetChunks() const;

        // ========
        // Setters
        // ========

        /** @brief Sets the state of a specified bit.
         *
         * @param bitIdx Bit index to sset.
         */
        void Set(uint bitIdx);

        /** @brief Sets the states of specified bits.
         *
         * @param bitIdxs Bit indices to set.
         */
        void Set(const std::vector<uint>& bitIdxs);

        /** @brief Sets the state of all bits. */
        void SetAll();

        /** @brief Unsets the state of a specified bit.
         *
         * @param bitIdx Bit index to unset.
         */
        void Clear(uint bitIdx);

        /** @brief Unsets the states of specified bits.
         *
         * @param bitIdxs Bit indices to unset.
         */
        void Clear(const std::vector<uint>& bitIdxs);

        /** @brief Unsets the state of all bits. */
        void ClearAll();

        /** @brief Flips the state of a specified bit.
         *
         * @param bitIdx Bit index to flip.
         */
        void Flip(uint bitIdx);

        /** @brief Flips the states of specified bits.
         *
         * @param bitIdxs Bit indices to flip.
         */
        void Flip(const std::vector<uint>& bitIdxs);

        /** @brief Flips the state of all bits. */
        void FlipAll();

        // ==========
        // Inquirers
        // ==========

        /** @brief Checks if all bits are unset.
         *
         * @return `true` if all bits are unset, `false` otherwise.
         */
        bool IsEmpty() const;

        /** @brief Checks if specified bits are set. If `testAny` is passed as `true`, any set bit will satisfy the condition.
         *
         * @param bitIdxs Bit indices to check.
         * @param testAny If any set bit will satisfy the condition.
         * @return `true` if the bits are set, `false` otherwise.
         */
        bool Test(const std::vector<uint>& bitIdxs, bool testAny = true) const;

        /** @brief Checks if a specified bit is set.
         *
         * @param bitIdx Bit index to check.
         * @param `true` if the bit is set, `false` otherwise.
         */
        bool Test(uint bitIdx) const;

        /** @brief Checks if any bits are set.
         *
         * @return `true` if any bits are set, `false` otherwise.
         */
        bool TestAny() const;

        /** @brief Checks if all bits are set.
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

        /** @brief Converts the bitfield to a string of 1s and 0s.
         *
         * @return Bitfield as a string.
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

        /** @brief Sets or unsets all bits to a specified state.
         *
         * @param state Bit state to set.
         */
        void Fill(bool state);

        /** @brief Checks if a specified bit exists. Only used in the @debug build.
         *
         * @param bitIdx Bit index to check.
         * @return `true` if the bit exists, `false` otherwise.
         */
        bool IsBitIdxValid(uint bitIdx) const;
    };
}

namespace std
{
    template<>
    struct hash<Silent::Utils::Bitfield>
    {
        size_t operator()(const Silent::Utils::Bitfield& bitField) const
        {
            size_t hashVal = 0;
            for (auto chunk : bitField.GetChunks())
            {
                hashVal ^= hash<Silent::Utils::Bitfield::ChunkType>{}(chunk) +
                           0x9E3779B9 +
                           (hashVal << 6) + (hashVal >> 2);
            }

            return hashVal;
        }
    };
}
