#pragma once

namespace Silent::Utils
{
    class BitField
    {
    public:
        // Aliases

         using ChunkType = uint;

    private:
        // Constants

        static constexpr uint DEFAULT_SIZE = 32;
        static constexpr uint CHUNK_SIZE   = std::numeric_limits<ChunkType>::digits;

        // Fields

        uint                   _size   = 0;
        std::vector<ChunkType> _chunks = {};

    public:
        // Presets

        static const BitField Empty;
        static const BitField Default;

        // Constructors

        BitField();
        BitField(uint size);
        BitField(const std::initializer_list<bool>& bits);
        BitField(const std::vector<ChunkType>& bitChunks, uint size);
        BitField(const std::string& bitString);
        
        // Getters

        uint                          GetSize() const;
        uint                          GetCount() const;
        const std::vector<ChunkType>& GetChunks() const;

        // Setters

        void Set(uint bitIdx);
        void Set(const std::vector<uint>& bitIdxs);
        void SetAll();
        void Clear(uint bitIdx);
        void Clear(const std::vector<uint>& bitIdxs);
        void ClearAll();
        void Flip(uint bitIdx);
        void Flip(const std::vector<uint>& bitIdxs);
        void FlipAll();

        // Inquirers

        bool IsEmpty() const;

        bool Test(const std::vector<uint>& bitIdxs, bool testAny = true) const;
        bool Test(uint bitIdx) const;
        bool TestAny() const;
        bool TestAll() const;

        // Utilities

        void Resize(uint size);

        // Converters

        std::string ToString() const;

        // Operators

        bool      operator==(const BitField& bitField) const;
        bool      operator!=(const BitField& bitField) const;
        BitField& operator=(const BitField& bitField) = default;
        BitField& operator&=(const BitField& bitField);
        BitField& operator|=(const BitField& bitField);
        BitField& operator^=(const BitField& bitField);
        BitField  operator&(const BitField& bitField) const;
        BitField  operator|(const BitField& bitField) const;
        BitField  operator^(const BitField& bitField) const;
        BitField  operator~() const;

    private:
        // Helpers

        void Fill(bool val);
        bool IsBitIdxValid(uint bitIdx) const;
    };
}

namespace std
{
    template<>
    struct hash<Silent::Utils::BitField>
    {
        size_t operator()(const Silent::Utils::BitField& bitField) const
        {
            size_t hashVal = 0;
            for (auto chunk : bitField.GetChunks())
            {
                hashVal ^= hash<Silent::Utils::BitField::ChunkType>{}(chunk) +
                           0x9E3779B9 +
                           (hashVal << 6) +
                           (hashVal >> 2);
            }

            return hashVal;
        }
    };
}
