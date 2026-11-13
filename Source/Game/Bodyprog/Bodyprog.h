#pragma once

#include "Game/Bodyprog/Game.h"

#include "Game/Bodyprog/Sound/Sfx.h"
#include "Game/Bodyprog/Collision/Collision.h"
#include "Game/Bodyprog/Chara/Model.h"
#include "Game/Bodyprog/Chara/Spawn.h"
#include "Game/Bodyprog/Events/Bgm.h"
#include "Game/Bodyprog/Events/Utils.h"
#include "Game/Bodyprog/Formats/Ipd.h"
#include "Game/Bodyprog/Formats/Lm.h"
#include "Game/Bodyprog/Formats/Model.h"
#include "Game/Bodyprog/Formats/Texture.h"
#include "Game/Bodyprog/Formats/Tmd.h"
#include "Game/Bodyprog/Gfx/World.h"
#include "Game/Bodyprog/Gfx/WorldObject.h"
#include "Game/Bodyprog/Items/WeaponData.h"
#include "Game/Bodyprog/Map/Map.h"
#include "Game/Bodyprog/Map/Terrain.h"
#include "Game/Bodyprog/Screen/Data.h"
#include "Game/Bodyprog/Sound/SfxIdEnum.h"
//#include "Game/Bodyprog/Text/MapMsg.h"
#include "Game/Bodyprog/View/VwSystem.h"
#include "Game/Main/FsQueue.h"

namespace Silent::Game
{
    struct s_RadioNoise
    {
        s8 prevIdx;
        s8 idx;
        s8 closeNpcInfoIdx;
        s8 field_3;
    };

    typedef struct
    {
        u8  unused_0[4];      /** @unused Looks meaningful, but unclear what it could be. */
        s16 startKeyframeIdx;
        s16 endKeyframeIdx;
    } s_800C44F0;

    struct s_MapEnvPresetIdxs
    {
        u8 presetIdx0;
        u8 presetIdx1;
    };

    extern s_FsImageDesc g_PaperMapImg;
    extern s_FsImageDesc g_PaperMapMarkingAtlasImg;
    extern s_FsImageDesc g_ItemInspectionImg;
    extern s_FsImageDesc g_LoadingScreenImg;
    extern s_FsImageDesc g_ControllerButtonAtlasImg;
    extern s_FsImageDesc g_HealthPortraitImg;
    extern s_FsImageDesc g_InventoryKeyItemTextureImg;
    extern s_FsImageDesc g_FirstAidKitItemTextureImg;

    /** Some intentory item texture (`ITEM/TIM07.TIM`). */
    extern s_FsImageDesc D_800A9074;

    extern s_FsImageDesc D_800A907C;

    extern s_FsImageDesc D_800A9084;

    extern s_FsImageDesc D_800A908C;

    extern s_FsImageDesc D_800A9094;

    extern s_WorldEnvPreset MAP_EFFECTS_INFOS[21];

    extern s32 D_800A9A20;

    /** X. */
    extern s32 D_800A9A24;

    /** Z. */
    extern s32 D_800A9A28;

    /** Loaded NPC types in memory and their `g_CharaModelAnimsData` indices. */
    extern s8 g_CharaAnimDataIdxs[Chara_Count];

    extern u8 D_800AA604[41][16];

    extern s_EventData* g_ItemEvents[];

    /** Radio SFX data. */
    extern s_RadioNoise g_RadioNoise[2];

    extern s_MapPoint2d g_MapPoint;

    /** Related to special item interactions. */
    extern s32 g_ItemTriggerItemIds[5];

    extern u8 g_MapAreaLoadCounter;

    extern s_EventData* g_MapEventData;

    extern s_FsImageDesc D_800A90A4;

    extern s_FsImageDesc D_800A90B4;

    extern s_WorldGfxWork g_WorldGfxWork;

    extern u8 D_800AD480[24];

    /** Weapon attacks. */
    extern s_AttackInfo g_Items_WeaponAttackInfo[70];

    extern s_MapOverlayHdr g_MapOverlayHdr;

    extern s16 SQRT[100];
}
