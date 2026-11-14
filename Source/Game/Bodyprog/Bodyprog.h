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
#include "Game/Bodyprog/Sound/SfxIds.h"
//#include "Game/Bodyprog/Text/MapMsg.h"
#include "Game/Bodyprog/View/VwSystem.h"
#include "Game/Main/FsQueue.h"

namespace Silent::Game
{
    /** Loaded NPC types in memory and their `g_CharaModelAnimsData` indices. */
    extern s8 g_CharaAnimDataIdxs[Chara_Count];

    extern u8 D_800AA604[41][16];

    extern s_EventData* g_ItemEvents[];

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

    extern s16 SQRT[100];
}
