#include "Framework.h"
#include "Utils/BlockAllocator.h"

namespace Silent::Utils
{
    BlockAllocator::BlockAllocator(int size)
    {
        size = std::max(size, 0);

        _size = size;
        _blocks.push_back(BlockMetadata
        {
            .Offset = 0,
            .Size   = size,
            .IsFree = true
        });
    }

    int BlockAllocator::GetSize() const
    {
        return _size;
    }

    int BlockAllocator::Allocate(int size)
    {
        constexpr int SPLIT_SIZE_MIN = 4;

        size = std::max(size, 0);

        // Find first fit.
        for (int i = 0; i < _blocks.size(); i++)
        {
            auto& block = _blocks[i];

            // Allocate.
            if (block.IsFree && block.Size >= size)
            {
                int allocOffset = block.Offset;
                block.IsFree    = false;

                // Split block if significant space is left over.
                if (block.Size > (size + SPLIT_SIZE_MIN))
                {
                    auto newBlock = BlockMetadata
                    {
                        .Offset = block.Offset + size,
                        .Size   = block.Size - size,
                        .IsFree = true
                    };

                    block.Size = size;
                    _blocks.insert((_blocks.begin() + i) + 1, newBlock);
                }

                return allocOffset;
            }
        }

        // Out of memory.
        Debug::Log(Fmt("Attempted to allocate memory block of size {}: out of memory.", size),
                       Debug::LogLevel::Warning);
        return NO_VALUE;
    }

    void BlockAllocator::Deallocate(int offset)
    {
        offset = std::max(offset, 0);

        for (int i = 0; i < _blocks.size(); i++)
        {
            auto& block = _blocks[i];

            // Set as free and merge free neighbors.
            if (block.Offset == offset)
            {
                block.IsFree = true;
                MergeNeighbors(i);
                break;
            }
        }
    }

    void BlockAllocator::Clear()
    {
        _blocks.clear();
        _blocks.push_back(BlockMetadata
        {
            .Offset = 0,
            .Size   = _size,
            .IsFree = true
        });
    }

    void BlockAllocator::MergeNeighbors(int idx)
    {
        // Check next block.
        if ((idx + 1) < _blocks.size() && _blocks[idx + 1].IsFree)
        {
            _blocks[idx].Size += _blocks[idx + 1].Size;
            _blocks.erase((_blocks.begin() + idx) + 1);
        }

        // Check previous block.
        if (idx > 0 && _blocks[idx - 1].IsFree)
        {
            _blocks[idx - 1].Size += _blocks[idx].Size;
            _blocks.erase(_blocks.begin() + idx);
        }
    }
}
