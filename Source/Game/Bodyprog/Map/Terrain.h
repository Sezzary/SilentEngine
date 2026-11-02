#pragma once

#include "Game/Bodyprog/Formats/Ipd.h"
#include "Game/Bodyprog/Formats/Lm.h"

namespace Silent::Game
{
    constexpr int ACTIVE_CHUNK_COUNT_MAX            = 4;
    constexpr int ACTIVE_CHUNK_TEXTURE_COUNT_MAX    = 10;
    constexpr int HALF_PAGE_CHUNK_TEXTURE_COUNT_MAX = 2;
    constexpr int FULL_PAGE_CHUNK_TEXTURE_COUNT_MAX = 8;

    /** @brief Map model load states. */
    enum e_MapModelLoadState
    {
        MapModelLoadState_Invalid   = 0,
        MapModelLoadState_Unloaded  = 1,
        MapModelLoadState_Corrupted = 2, // Maybe wrong name for this.
        MapModelLoadState_Loaded    = 3
    };

    /** @brief IPD chunk file load states.
     *
     * See `Map_ChunkLoadStateGet`.
     */
    enum e_WorldMapLoadState
    {
        WorldMapLoadState_Invalid  = 0, /** Entry index is `NO_VALUE`. */
        WorldMapLoadState_Unloaded = 1, /** Not currently loaded. */
        WorldMapLoadState_Loaded   = 2  /** Currently loaded. */
    };

    /** @brief Map chunk. */
    struct s_MapChunk
    {
        std::string* ipdHdr;
        int          queueIdx;
        int          cellX;
        int          cellZ;
        q19_12       paddedDistanceToEdge;
        q19_12       projPaddedDistanceToEdge;
        int          materialCount;
        int          outsideCount;
    };

    /** @brief Map terrain chunk column. TODO: Or row? */
    struct s_ChunkColumn
    {
        int idx[16];
    };

    struct s_ActiveChunkTextures
    {
        int        count;
        s_Texture* textures[ACTIVE_CHUNK_TEXTURE_COUNT_MAX];
    };

    /** @brief Texture data associated with map terrain chunks. */
    struct s_ChunkTextures
    {
        s_ActiveChunkTextures fullPage;
        s_ActiveChunkTextures halfPage;
        s_Texture             fullPageTextures[FULL_PAGE_CHUNK_TEXTURE_COUNT_MAX];
        s_Texture             halfPageTextures[HALF_PAGE_CHUNK_TEXTURE_COUNT_MAX];
    };

    /** @brief Global PLM model. */
    struct s_GlobalPlm
    {
        s_LmHeader* lmHdr; // TODO: Should be an array according to `WorldMap_ChunkPropertiesSet` call?
        int         fileIdx;
        int         queueIdx;
    };

    /** @brief World map workspace. */
    struct s_WorldMapWork
    {
        s_IpdCollisionData collisionData; // Default chunk collision data?
        int                textureFileIdx;
        s_GlobalPlm        globalPlm;
        char               mapTag[4];
        int                mapTagSize;
        int                ipdFileIdx;
        s_IpdHeader*       chunkBuffer;
        int                chunkBufferSize;
        int                activeChunkCount;
        s_MapChunk         activeChunks[ACTIVE_CHUNK_COUNT_MAX];
        s_ChunkColumn      chunkGrid[19];
        s_ChunkColumn*     chunkGridCenter; // TODO: All access to this variable relies on hacks.
        s_ChunkTextures    chunkTextures;
        q19_12             positionX;
        q19_12             positionZ;
        int                cellX;
        int                cellZ;
        bool               isExterior;
    };

    /** @brief Packs XZ cell coordinates in a single value.
     *
     * @param x X cell coordinate.
     * @param z Z cell coordinate.
     * @return Packed XZ cell coordinates.
     */
    constexpr int CELL_XZ(uchar x, uchar z)
    {
        return x + (z << 8);
    }
}
