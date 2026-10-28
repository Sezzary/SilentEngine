#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/Bodyprog.h"
#include "Game/Bodyprog/Chara/Spawn.h"

#include "Game/Bodyprog/Screen/ScreenDraw.h"
//#include "Game/Bodyprog/item_screens.h"
#include "Game/Bodyprog/Sound/SoundSystem.h"
#include "Game/Main/FsQueue.h"

namespace Silent::Game
{
    bool Chara_Load(s32 modelIdx, s8 charaId, GsCOORDINATE2* coords, s8 forceFree,
                    s_LmHeader* lmHdr, s_FsImageDesc* tex) // 0x80088C7C
    {
        //Fs_CharaAnimDataAlloc(modelIdx + 1, charaId, nullptr, coords);
        //WorldGfx_CharaLmBufferAssign(forceFree);
        //WorldGfx_CharaLoad(charaId, modelIdx, lmHdr, tex);
        return true;
    }

    bool Chara_ProcessLoads() // 0x80088D0C
    {
        Fs_QueueWaitForEmpty();
        //WorldGfx_CharaModelProcessAllLoads();
        return true;
    }

    void Chara_BonesInit(s32 idx) // 0x80088D34
    {
        idx++;
        //Anim_BonesInit(g_CharaTypeAnimInfo[idx].animFile1_8, g_CharaTypeAnimInfo[idx].npcBoneCoords);
    }

    s32 Chara_Spawn(e_CharaId charaId, s32 spawnFlags, q19_12 posX, q19_12 posZ, q3_12 rotY, u32 stateStep) // 0x80088D78
    {
        // TODO: Weird code, check.
        int activeSpawnFlags = 0;
        if (charaId <= Chara_MonsterCybil && spawnFlags < (1 << 6))
        {
            activeSpawnFlags = CharaSpawnFlag_0 | CharaSpawnFlag_1 | CharaSpawnFlag_2 | CharaSpawnFlag_3 | CharaSpawnFlag_4;
            activeSpawnFlags = spawnFlags & activeSpawnFlags;
        }
        else
        {
            activeSpawnFlags = CharaSpawnFlag_None;
        }

        if (charaId <= Chara_MonsterCybil)
        {
            if (HAS_FLAG(g_SysWork.field_228C, activeSpawnFlags))
            {
                for (int i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
                {
                    if (g_SysWork.npcs[i].field_40 == activeSpawnFlags)
                    {
                        return i;
                    }
                }

                return ARRAY_SIZE(g_SysWork.npcs);
            }

            int npcFlagsId = 0;
            for (int i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
            {
                if (g_SysWork.npcs[i].model.charaId != Chara_None)
                {
                    npcFlagsId++;
                }
            }

            if (npcFlagsId >= g_SysWork.npcFlagsId)
            {
                return 0;
            }
        }

        // Run through NPC slots.
        for (int i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
        {
            // Skip occupied slot.
            if (g_SysWork.npcs[i].model.charaId != Chara_None)
            {
                continue;
            }

            bzero(&g_SysWork.npcs[i], sizeof(s_SubCharacter));

            g_SysWork.npcs[i].model.charaId = charaId;
            g_SysWork.npcs[i].field_40 = activeSpawnFlags;

            if (charaId <= Chara_MonsterCybil && spawnFlags < (1 << 6))
            {
                SET_FLAG(g_SysWork.field_228C, activeSpawnFlags);
            }

            SET_FLAG(&g_SysWork.npcFlags, i);

            g_SysWork.npcs[i].model.controlState = 0;
            g_SysWork.npcs[i].model.stateStep    = stateStep;
            g_SysWork.npcs[i].position.vx        = posX;

            auto surface = s_CollisionSurface{};
            //Collision_SurfaceGet(&surface, posX, posZ);
            g_SysWork.npcs[i].position.vy = surface.floorHeight;
            g_SysWork.npcs[i].position.vz = posZ;
            g_SysWork.npcs[i].rotation.vy = rotY;

            auto& chara             = g_SysWork.npcs[i];
            chara.model.anim.flags |= AnimFlag_Visible;

            return i;
        }

        return ARRAY_SIZE(g_SysWork.npcs);
    }

    void Chara_ModelCharaIdClear(s_SubCharacter* chara, s32 unused0, s32 unused1) // 0x80088F94
    {
        if (chara == nullptr)
        {
            return;
        }

        if (chara->model.charaId <= Chara_MonsterCybil)
        {
            CLEAR_FLAG(g_SysWork.field_228C, chara->field_40);
        }

        chara->model.charaId = Chara_None;
    }

    void Chara_SpawnFlagsSet(s_MapOverlayHdr mapHdr, e_CharaId charaId, s32 spawnIdx, s32 spawnFlags) // 0x80088FF4
    {
        s_SpawnInfo* spawnInfo;

        /*spawnInfo        = &mapHdr.charaSpawnInfos[g_CharaAnimDataIdxs[charaId] - 1][spawnIdx];
        spawnInfo->flags = spawnFlags;*/
    }

    void Chara_SpawnPositionSet(s_MapOverlayHdr mapHdr, e_CharaId charaId, s32 spawnIdx, q19_12 posX, q19_12 posZ) // 0x80089034
    {
        s_SpawnInfo* spawnInfo0;
        s_SpawnInfo* spawnInfo1;

        /*spawnInfo0            = &g_MapOverlayHdr.charaSpawnInfos[g_CharaAnimDataIdxs[charaId] - 1][spawnIdx];
        spawnInfo0->positionX = posX;

        spawnInfo1            = &g_MapOverlayHdr.charaSpawnInfos[g_CharaAnimDataIdxs[charaId] - 1][spawnIdx];
        spawnInfo1->positionZ = posZ;*/
    }
}
