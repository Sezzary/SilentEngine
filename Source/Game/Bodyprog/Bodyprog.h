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
    struct s_RadioNpcInfo
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

    typedef struct
    {
        u8  field_0;
        u8  field_1;
        s16 field_2; // XYZ? X and Y swapped?
        s32 field_4; //
        s32 field_8; //
    } s_800C42E8;

    typedef struct
    {
        s16 field_0;
        s16 field_2;
        s16 field_4;
        s16 field_6;
        u16 field_8;
        u16 field_A;
        u8  field_C;
    } s_800C4418;

    extern s_FsImageDesc g_MainImg0; // 0x80022C74 - TODO: Part of main exe, move to `main/` headers?

    extern char D_80028544[16];

    extern RECT D_80028A20;

    // Something related to keyframes.
    extern const s_800C44F0 D_800294F4[];

    extern const s_AnimInfo* D_800297B8;

    extern u_Filename D_8002B2CC;

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

    extern s_WorldEnvState D_800A952C;

    extern s32 D_800A9A20;

    /** X. */
    extern s32 D_800A9A24;

    /** Z. */
    extern s32 D_800A9A28;

    /** Loaded NPC types in memory and their `g_CharaModelAnimsData` indices. */
    extern s8 g_CharaAnimDataIdxs[Chara_Count];

    extern s32 D_800A9EB0;

    extern s_FsImageDesc D_800A9EB4;

    extern s_FsImageDesc D_800A9EBC;

    extern s_FsImageDesc D_800A9EC4;

    extern s32 D_800A9938;

    extern u8 D_800A9944[];

    /** Player anim info? */
    extern s_AnimInfo D_800A998C;

    /** Current player anim index store? */
    extern u8 D_800A9990;

    // Likely declared as static inside the function that uses it.
    extern s32 D_800A99A0;

    /** Relative file offset for map texture? */
    extern s8 D_800A99B5;

    extern s32 D_800A9A68;

    extern RECT D_800A9A6C; // `RECT<320, 256, 160, 240>`, only used in `SysState_Fmv_Update`?

    extern s32 D_800A9ECC;

    extern s32 D_800A9ED0;

    extern s32 D_800A9ED4;

    extern s32 D_800A9ED8;

    extern s32 D_800A9EDC;

    extern s32 D_800A9EE0;

    extern s32 D_800A9EE4;

    extern s32 D_800A9EE8;

    extern s32 D_800A9EEC;

    extern s32 D_800A9EF0;

    extern s32 D_800A9EF4;

    extern s32 D_800A9EF8;

    extern s32 D_800A9EFC;

    extern s32 D_800A9F00;

    extern s32 D_800A9F04;

    extern s32 D_800A9F08;

    extern s32 D_800A9F0C;

    extern s32 D_800A9F10;

    extern s32 D_800A9F14;

    extern s32 D_800A9F18;

    extern s32 D_800A9F1C;

    extern s32 D_800A9F20;

    extern s32 D_800A9F28;

    extern s32 D_800A9F2C;

    extern s32 D_800A9F30;

    extern s32 D_800A9F34;

    extern s32 D_800A9F38;

    extern s32 D_800A9F3C;

    extern s32 D_800A9F40;

    extern s32 D_800A9F44;

    extern s32 D_800A9F48;

    extern s32 D_800A9F4C;

    extern s32 D_800A9F50;

    extern s32 D_800A9F54;

    extern s32 D_800A9F58;

    extern s32 D_800A9F60;

    extern s32 D_800A9F64;

    extern s32 D_800A9F68;

    extern s32 D_800A9F6C;

    extern s32 D_800A9F74;

    extern s32 D_800A9F78;

    extern s32 D_800A9F7C;

    extern s_MapEnvPresetIdxs D_800A9F80;

    extern s_MapEnvPresetIdxs D_800A9F84;

    extern s_MapEnvPresetIdxs D_800A9F88;

    extern s_MapEnvPresetIdxs D_800A9F8C;

    extern s_MapEnvPresetIdxs D_800A9F98;

    extern u32 D_800A9FB0;

    extern s32 D_800A9FB4[];

    extern u8 D_800AA604[41][16];

    extern u8 D_800AE185;

    extern u8 D_800AE186;

    extern s_EventData* g_ItemEvents[];

    /** Radio SFX data. */
    extern s_RadioNpcInfo g_RadioNpcInfos[2];

    extern s_MapPoint2d g_MapPoint;

    /** Related to special item interactions. */
    extern s32 g_ItemTriggerItemIds[5];

    extern u8 g_MapAreaLoadCounter;

    extern s_EventData* g_MapEventData;

    extern q3_12 g_Player_FlexRotationY;

    extern q3_12 g_Player_FlexRotationX;

    extern const char* INV_ITEM_NAMES[];

    extern const char* g_ItemDescriptions[];

    extern s_FsImageDesc D_800A90A4;

    extern s_FsImageDesc D_800A90B4;

    extern s_SubCharacter D_800BA00C; // Often passed to `Los_NpcToPlayerHitCheck`, might not be full `s_SubCharacter`?

    extern u8 D_800BC74F;

    extern u16 D_800BCCB0;

    extern u32 D_800A999C;

    extern u16 D_800BCCB2;

    extern s32 D_800BCD5C;

    extern s32 D_800BCD84;

    /** Angles. */
    extern s16 D_800BCDE8[8];

    extern s_WorldGfxWork g_WorldGfxWork;

    extern s8* D_800C15B0;

    extern s8 D_800C15B4;

    extern s16 D_800C16A4;

    extern s16 D_800C391E;

    extern s8 D_800C39A0;

    /** Functions from `bodyprog_8003AB28` access `g_WorldEnvRenderWork` as constant, but some functions from `bodyprog_80055028` write to it.
     * It appears that g_WorldEnvRenderWork is intended to be defined inside `bodyprog_80055028` as writable and declared as read-only (`const`) outside of it.
     */
    // extern s_WorldEnvRenderWork g_WorldEnvRenderWork;

    extern GsCOORDINATE2* D_800C42B8; // Set to view coord.

    extern q4_12 g_Player_RotationDeltaToTargetY;
    extern q4_12 g_Player_RotationDeltaToTargetX;

    extern s_800C42E8 D_800C42E8[24];

    extern s16 D_800C4408;

    extern GsCOORDINATE2* D_800C440C;

    extern GsCOORDINATE2* D_800C4410;

    /** Flags. */
    extern s8 D_800C4414;

    extern s_800C4418 D_800C4418;

    extern q3_12 D_800C4454;

    extern s8 D_800C447A;

    extern u8 g_Player_AnimResetRequest; /** `bool` */

    extern s_CollisionSurface D_800C4620;

    /** Q19.12 */
    extern VECTOR3 D_800C4788[];

    extern VECTOR3 D_800C47B8;

    extern VECTOR3 D_800C47C8[];

    extern VECTOR3 D_800C47E8;

    extern s_RayTrace D_800C47F8;

    extern u8 D_800AD480[24];

    /** Weapon attacks. */
    extern s_AttackInfo g_Items_WeaponAttackInfo[70];

    extern s_MapOverlayHdr g_MapOverlayHdr;

    extern s16 SQRT[100];
}
