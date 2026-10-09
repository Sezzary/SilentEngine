#pragma once

#include "Game/Bodyprog/Formats/Ipd.h"
#include "Game/Bodyprog/Formats/Lm.h"

namespace Silent::Game
{
    // TODO: Need to decide on clearer terminology. Right now we have "chunk cells", "cells", and "subcells", which are
    // kind of confusing.

    constexpr float CHUNK_SIZE                        = 40.0f;
    constexpr int   ACTIVE_CHUNK_COUNT_MAX            = 4;
    constexpr int   ACTIVE_CHUNK_TEXTURE_COUNT_MAX    = 10;
    constexpr int   HALF_PAGE_CHUNK_TEXTURE_COUNT_MAX = 2;
    constexpr int   FULL_PAGE_CHUNK_TEXTURE_COUNT_MAX = 8;

    /** @brief Map terrain chunk IPD file load states.
     *
     * See `Map_ChunkLoadStateGet`.
     */
    enum e_ChunkLoadState
    {
        ChunkLoadState_Invalid  = 0, /** Entry index is `NO_VALUE`. */
        ChunkLoadState_Unloaded = 1, /** Not currently loaded. */
        ChunkLoadState_Loaded   = 2  /** Currently loaded. */
    };

    /** @brief Map terrain chunk. */
    struct s_Chunk
    {
        s_IpdHeader* ipdHdr;
        s32          queueIdx;
        s16          cellX;
        s16          cellZ;
        q19_12       paddedDistanceToEdge0;
        q19_12       paddedDistanceToEdge1;
        u8           materialCount;
        s8           __pad_15[3];
        s32          outsideCount;
    };

    /** @brief Map terrain chunk column. TODO: Or row? */
    struct s_ChunkColumn
    {
        s16 idx[16];
    };

    struct s_ActiveChunkTextures
    {
        s32        count;
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

    /** @brief Map terrain layout. */
    struct s_MapTerrain
    {
        s_IpdCollisionData collisionData; // Default chunk collision data?
        s32                textureFileIdx;
        s_GlobalLm         globalLm;
        char               mapTag[4];
        s32                mapTagSize;
        s32                ipdFileIdx;
        s_IpdHeader*       chunkBuffer;
        s32                chunkBufferSize;
        s32                activeChunkCount;
        s_Chunk            activeChunks[ACTIVE_CHUNK_COUNT_MAX];
        s_ChunkColumn      chunkGrid[19];
        s_ChunkColumn*     chunkGridCenter;
        s_ChunkTextures    chunkTextures;
        q19_12             positionX;
        q19_12             positionZ;
        s32                cellX;
        s32                cellZ;
        bool               isExterior;
    };
}
