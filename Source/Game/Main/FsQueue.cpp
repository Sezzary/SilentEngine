#include "Framework.h"
#include "Psx.h"
#include "Game/Main/FsQueue.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Assets/AssetStreamer.h"
#include "Game/Bodyprog/Chara/Anim.h"
#include "Game/Bodyprog/Sys/CharaFileInfos.h"
#include "Game/Bodyprog/Sys/Demo.h"
#include "Game/Main/FsMem.h"

using namespace Silent::Assets;

namespace Silent::Game
{
    s_FsQueue g_FsQueue;

    bool Fs_QueueIsEntryLoaded(int queueIdx)
    {
        // @todo Need to check equivalent.
        return true;
        //return queueIdx < g_FsQueue.postLoad.idx;
    }

    int Fs_QueueGetLength()
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
            //result = WorldGfx_ChunkInitCheck() != false;
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

    const std::future<void>& Fs_QueueStartReadAnm(int animDataIdx, int charaId)
    {
        auto& assets = g_App.GetAssets();

        int assetIdx = CHARA_FILE_INFOS[charaId].animFileIdx;
        return assets.Load(assetIdx);
    }

    void Fs_QueueReset()
    {
        // @stub
        // @todo Previously cleared the queue, should that translate to clearing all loaded assets?
    }
}
