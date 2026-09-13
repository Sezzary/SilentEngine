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
    s_FsQueue g_FsQueue;

    bool Fs_QueueIsEntryLoaded(s32 queueIdx)
    {
        // @todo Need to check equivalent.
        return true;
        //return queueIdx < g_FsQueue.postLoad.idx;
    }

    s32 Fs_QueueGetLength()
    {
        const auto& assets = g_App.GetAssets();

        return assets.GetLoadingCount();
    }

    bool Fs_QueueChunksLoad()
    {
        g_Demo_IsLoadingChunks = true;

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

    const std::future<void>& Fs_QueueStartReadTim(e_FsFile fileIdx, void* dest, s_FsImageDesc* image)
    {
        auto& assets = g_App.GetAssets();

        if (image != nullptr)
        {
            image->AssetName = assets.GetName((int)fileIdx);
        }

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

    void Fs_QueueReset()
    {
        // @stub
        // @todo Previously cleared the queue, should that translate to clearing all loaded assets?
    }

    bool Fs_QueueUpdatePostLoad(s_FsQueueEntry* entry)
    {
        // @stub
        // @todo `Fs_QueuePostLoadAnm` call seemed important.
        return true;
    }

    bool Fs_QueuePostLoadAnm(s_FsQueueEntry* entry)
    {
        // @todo Seems important, not decompiled yet.
        //Fs_CharaAnimDataUpdate(entry->extra.anm.field_0, entry->extra.anm.charaId_4, entry->externalData,
        //                       entry->extra.anm.coords_8);
        return true;
    }
}
