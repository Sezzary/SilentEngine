#include "Framework.h"
#include "Psx.h"
#include "Game/Main/FsQueue.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Assets/AssetStreamer.h"
#include "Game/Bodyprog/Demo.h"
#include "Game/Main/FsMem.h"

using namespace Silent::Assets;

namespace Silent::Game
{
    /** @brief Wraps a filesystem queue entry index to `FS_QUEUE_LENGTH`.
     *
     * @param idx Index to wrap.
     * @return Wrapped `idx`.
     */
    #define FSQUEUE_IDX_WRAP(idx) \
        ((u32)(idx) % FS_QUEUE_LENGTH)

    s_FsQueue g_FsQueue;

    bool Fs_QueueIsEntryLoaded(s32 queueIdx)
    {
        return queueIdx < g_FsQueue.postLoad.idx;
    }

    s32 Fs_QueueGetLength()
    {
        const auto& assets = g_App.GetAssets();

        return assets.GetLoadingCount();
    }

    bool Fs_QueueChunksLoad()
    {
        // @todo
        //D_800C489C = true;

        bool result = false;
        if (Fs_QueueGetLength() == 0)
        {
            // @todo
            //result = Ipd_ChunkInitCheck() != false;
        }

        return result;
    }

    void Fs_QueueWaitForEmpty()
    {
        const auto& assets = g_App.GetAssets();

        while (assets.IsBusy())
        {
            // @todo Blocking behaviours shouldn't occur anymore.
        }
    }

    const std::future<void>& Fs_QueueStartSeek(e_FsFile fileIdx)
    {
        return Fs_QueueStartRead(fileIdx);
    }

    const std::future<void>& Fs_QueueStartRead(e_FsFile fileIdx)
    {
        auto& assets = g_App.GetAssets();

        return assets.Load(fileIdx);
    }

    const std::future<void>& Fs_QueueStartRead(e_FsFile fileIdx, void* dest)
    {
        return Fs_QueueStartRead(fileIdx);
    }

    const std::future<void>& Fs_QueueStartReadTim(e_FsFile fileIdx, void* dest, const s_FsImageDesc* image)
    {
        return Fs_QueueStartRead(fileIdx);
    }

    const std::future<void>& Fs_QueueStartReadAnm(s32 idx, s32 charaId, void* dest, GsCOORDINATE2* coords)
    {
        auto& assets = g_App.GetAssets();

        s32            fileIdx;
        s_FsQueueExtra extra;

        fileIdx             = CHARA_FILE_INFOS[charaId].animFileIdx; // @todo Port over `CHARA_FILE_INFOS`.
        extra.anm.charaId_4 = charaId;
        extra.anm.field_0   = idx;
        extra.anm.coords_8  = coords;
        return assets.Load(fileIdx);
    }

    s32 Fs_QueueEnqueue(e_FsFile fileIdx, u8 op, u8 postLoad, u8 alloc, void* data, u32 unused0, s_FsQueueExtra* extra)
    {
        // @stub
        return 0;
    }

    void Fs_QueueInitialize()
    {
        // @stub
    }

    void Fs_QueueReset()
    {
        // @stub
        // @todo Previously cleared the queue, should that translate to clearing all loaded assets?
    }

    void Fs_QueueUpdate()
    {
        // @stub
        // @todo `Fs_QueueUpdatePostLoad` call seemed important.
    }

    bool Fs_QueueUpdateSeek(s_FsQueueEntry* entry)
    {
        // @stub
        return true;
    }

    bool Fs_QueueUpdateRead(s_FsQueueEntry* entry)
    {
        // @stub
        return true;
    }

    bool Fs_QueueAllocEntryData(s_FsQueueEntry* entry)
    {
        // @stub
        return true;
    }

    bool Fs_QueueCanRead(s_FsQueueEntry* entry)
    {
        // @stub
        return true;
    }

    bool Fs_QueueDoBuffersOverlap(u8* data0, u32 size0, u8* data1, u32 size1)
    {
        // @stub
        return false;
    }

    bool Fs_QueueTickSetLoc(s_FsQueueEntry* entry)
    {
        // @stub
        return true;
    }

    bool Fs_QueueTickRead(s_FsQueueEntry* entry)
    {
        // @stub
        return true;
    }

    bool Fs_QueueResetTick(s_FsQueueEntry* entry)
    {
        // @stub
        return true;
    }

    bool Fs_QueueTickReadPcDrv(s_FsQueueEntry* entry)
    {
        // @stub
        return true;
    }

    bool Fs_QueueUpdatePostLoad(s_FsQueueEntry* entry)
    {
        // @stub
        // @todo `Fs_QueuePostLoadAnm` call seemed important.
        return true;
    }

    bool Fs_QueuePostLoadTim(s_FsQueueEntry* entry)
    {
        // @stub
        return true;
    }

    bool Fs_QueuePostLoadAnm(s_FsQueueEntry* entry)
    {
        // @todo Seems important, not decompiled yet.
        //Fs_CharaAnimInfoUpdate(entry->extra.anm.field_0, entry->extra.anm.charaId_4, entry->externalData,
        //                       entry->extra.anm.coords_8);
        return true;
    }
}
