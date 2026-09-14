#pragma once

#include "Game/Game.h"

#include "Game/Bodyprog/Sound/Sfx.h"
#include "Game/Bodyprog/Collision/Collision.h"
#include "Game/Bodyprog/Chara/CharaModel.h"
#include "Game/Bodyprog/Chara/Spawn.h"
#include "Game/Bodyprog/Events/Bgm.h"
#include "Game/Bodyprog/Events/Utils.h"
//#include "Game/Bodyprog/Events/MapMsg.h"
#include "Game/Bodyprog/Formats/Ipd.h"
#include "Game/Bodyprog/Formats/Lm.h"
#include "Game/Bodyprog/Formats/Model.h"
#include "Game/Bodyprog/Formats/Texture.h"
#include "Game/Bodyprog/Formats/Tmd.h"
#include "Game/Bodyprog/Gfx/World.h"
#include "Game/Bodyprog/Gfx/WorldObject.h"
#include "Game/Bodyprog/Map/Map.h"
#include "Game/Bodyprog/Map/Terrain.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Sound/SfxIdEnum.h"
#include "Game/Bodyprog/View/VwSystem.h"
#include "Game/Main/FsQueue.h"

namespace Silent::Game
{
    constexpr int OPT_SOUND_VOLUME_MIN   = 0;
    constexpr int OPT_SOUND_VOLUME_MAX   = 128;
    constexpr int OPT_VIBRATION_DISABLED = 0;
    constexpr int OPT_VIBRATION_ENABLED  = 128;

    /** @brief SFX flags. */
    enum e_SfxFlags
    {
        SfxFlag_None          = 0,
        SfxFlag_Mono          = 1 << 0, /** Monophonic. */
        SfxFlag_NoDistAtten   = 1 << 1, /** No distance attenuation. */
        SfxFlag_UpdateAttribs = 1 << 2  /** @unused Update attributes. */
    };

    // Used by `func_8006E490` and `func_8006E150`.
    enum e_OrientationFlags
    {
        OrientationFlags_None    = 0,
        OrientationFlags_InvertX = 1 << 0,
        OrientationFlags_InvertZ = 1 << 1,
        OrientationFlags_SwapXz  = 1 << 2
    };

    enum e_BoneHierarchy
    {
        BoneHierarchy_MultiModel = -3,
        BoneHierarchy_End        = -2
    };

    enum e_EffectTextureFlags
    {
        EffectTextureFlag_None         = 0,
        EffectTextureFlag_Glass        = 1 << 1, /** Broken glass in the cafe Air Screamer cutscene. */
        EffectTextureFlag_WaterRefract = 1 << 2, /** Water waves and light reflection in the sewer. */
        EffectTextureFlag_Water        = 1 << 3,
        EffectTextureFlag_Fire         = 1 << 4,
        EffectTextureFlag_Ef           = 1 << 5, // TODO: Rename. Looks like stringy flesh?
        EffectTextureFlag_Blood        = 1 << 6,
        EffectTextureFlag_WarmTest     = 1 << 7  /** @unused Test lens flare? */
    };

    enum e_MainMenuEntry
    {
        MainMenuEntry_Load     = 0,
        MainMenuEntry_Continue = 1,
        MainMenuEntry_Start    = 2,
        MainMenuEntry_Option   = 3,
        MainMenuEntry_Quit     = 4, /** @unused The extra options menu may have been accessible via the main menu. */

        MainMenuEntry_Count    = 5
    };

    enum e_MainMenuState
    {
        MenuState_Start              = 0,
        MenuState_Main               = 1,
        MenuState_LoadGame           = 2,
        MenuState_DifficultySelector = 3,
        MenuState_NewGameStart       = 4
    };

    /** Used by `func_8003F654` to cast a specific field to the desired type. */
    enum e_PrimitiveType
    {
        PrimitiveType_None = 0,
        PrimitiveType_S8   = 1,
        PrimitiveType_U8   = 2,
        PrimitiveType_S16  = 3,
        PrimitiveType_U16  = 4,
        PrimitiveType_S32  = 5
    };

    enum e_LoadingScreenId
    {
        LoadingScreenId_None              = 0,
        LoadingScreenId_PlayerRun         = 1,
        LoadingScreenId_BackgroundTexture = 2,
        LoadingScreenId_StageString       = 3
    };

    enum e_StaticModelLoadState
    {
        StaticModelLoadState_Invalid   = 0,
        StaticModelLoadState_Unloaded  = 1,
        StaticModelLoadState_Corrupted = 2, // Maybe wrong name for this.
        StaticModelLoadState_Loaded    = 3
    };

    // ================
    // UNKNOWN STRUCTS
    // ================
    // TODO: Deobfuscate these and move where they belong afterwardd.

    /** @brief Struct used by many functions involved with GTE. Kept at `PSX_SCRATCH_ADDR` (possibly only temporarily). */
    struct s_GteScratchData
    {
        DVECTOR screenXy_0[90];
        u16     screenZ_168[18];
        s16     field_18C[99]; // The size changed from 150 due to the addition of `field_252`. Not sure if this is correct.
        u8      field_252[102];
        u8      field_2B8[200]; // Size likely incorrect.

        union
        {
            MATRIX field_0;
            struct
            {
                s32     field_0;
                s32     field_4;
                CVECTOR field_8;
                CVECTOR field_C;
                u8      field_10;
                u8      field_11;
                u8      field_12;
                u8      field_13;
                u8      field_14;
                u8      field_15;
                u8      field_16;
                u8      field_17;
                s32     field_18;
                s32     field_1C;
            } s_0;
        } field_380;

        s_Normal field_3A0;
        DVECTOR  screenPos_3A4;
        s32      depthP_3A8;
        SVECTOR  field_3AC;
        u8       unk_3B4[36];
        CVECTOR  field_3D8; // Written by `func_8005A42C`.
        DVECTOR  screenPos_3DC;
        s32      depthP_3E0;
        s16      rotMatrix_3E4[3][3]; // Truncated `MATRIX` without the `long t[3];` transfer vector?
    };

    typedef struct
    {
        s8  field_0;
        s8  field_1;
        s8  field_2;
        s8  field_3;
        s32 field_4[4];
    } s_800382B0;

    typedef struct
    {
        s16 field_0; // Flags?
    } s_8008D850;

    /** Related to weapon attacks. Stats, SFX IDs, damange values, etc.? */
    typedef struct
    {
        q4_12 field_0;
        q3_12 field_2;
        u16   field_4; // Related to damage. Multiplier?
        s8    field_6; // Accessed by `func_8008BF84` as `u16`
        s8    unk_7;
        u8    field_8;   // Accessed by `func_8008BF84` as `u16`
        u8    charaId_9; /** `e_CharaId` */
        u8    field_A;   // Accessed by `func_8008BF84` as `u16`
        u8    field_B;
        q4_12 field_C;
        u8    field_E;  // Keyframe index offset?
        u8    field_F;  // Keyframe index offset?
        u8    field_10; // State.
        u8    field_11;
        u8    field_12; // SFX ID subgroup. Uses values 0-4.
        s8    __pad_13;
        u32*  unk_14; // Some pointer. All entries have the same value `D_800AD4C4`.
    } s_800AD4C8;

    struct s_RadioNpcInfo
    {
        s8 prevIdx;
        s8 idx;
        s8 closeNpcInfoIdx;
        s8 field_3;
    };

    typedef struct
    {
        u8      field_0; // `bool`?
        u8      field_1; // `bool`?
        u8      field_2; // `bool`?
        u8      field_3;
        s8      unk_4[4];
        s16     field_8;
        s16     field_A;
        VECTOR3 field_C; // Q19.12
        s8      unk_18[4];
        s32     field_1C;
        q19_12  field_20;
    } s_800C4818;

    typedef struct
    {
        u8  unk_0[4];
        s16 field_4;
        s16 field_6; // Keyframe index or time.
    } s_800C44F0; // Probable size: 8 bytes.

    struct s_MapEnvPresetIdxs
    {
        u8 presetIdx0;
        u8 presetIdx1;
    };

    typedef struct
    {
        DVECTOR  screenXy_0[90];
        s16      screenZ_168[18];
        s16      field_18C[72];
        s32      field_21C[39]; // Used as `VECTOR3`?
        u8       field_2B8[200];
        MATRIX   field_380;
        s_Normal field_3A0;
        DVECTOR  screenPos_3A4;
        s32      depthP_3A8;
        SVECTOR  field_3AC;
        u8       unk_3B4[36];
        CVECTOR  field_3D8;

        // Different functions access different data at 0x3DC onwards.
        // Union works for it, but also possible those functions just took different `s_GteScratchData` structs.
        union
        {
            struct
            {
                DVECTOR screenPos_3DC;
                s32     depthP_3E0;
                s16     rotMatrix_3E4[3][3]; // Truncated `MATRIX` without the `long t[3];` transfer vector?
            } vertex;

            struct
            {
                s_Normal field_3DC;
                SVECTOR  field_3E0[3];
            } normal;

            struct
            {
                u8  field_0;
                u8  field_1;
                u8  field_2;
                u8  field_3;
                u8  field_4;
                u8  field_5;
                u8  field_6;
                u8  field_7;
                s32 field_8;
            } s_1;
        } u;
    } s_GteScratchData2;

    // Something for inventory items.
    typedef struct
    {
        q3_12   positionY;
        q3_12   field_2; // Move dist?
        q3_12   field_4; // Angle.
        s16     field_6;
        s16     field_8;
        s8      __pad_A[2];
        SVECTOR field_C;  // Q3.12 | Offset?
        SVECTOR position; /** Q3.12 */
    } s_800AE204;

    typedef struct
    {
        s_800AE204* ptr_0;
        u8          count_4;
        u8          unk_5;
        q7_8        field_6;
        u8          field_8;
        u8          field_9;
        u8          field_A;
        u8          field_B;
    } s_800AE4DC;

    typedef struct
    {
        u8 field_0;
        u8 field_1;
        u8 field_2;
        u8 field_3; // Map marking sprite height?
    } s_800AE8A0_0;

    typedef struct
    {
        s8 field_0;
        s8 field_1;
        u8 field_2;
        u8 field_3;
    } s_800AE8A0_4;

    /** Paper map marking graphic data? */
    typedef struct
    {
        POLY_FT4*    field_0;
        s_800AE8A0_0 field_4;
        s_800AE8A0_4 field_8;
        s32          field_C; // Count.
    } s_func_80068E0C;

    typedef struct
    {
        POLY_FT4* field_0;
        SVECTOR   field_4; // Q23.8
        MATRIX    field_C;
        VECTOR3   field_2C; // Q23.8 | Set as grid-aligned player position in `func_80065B94`.
        s32       field_38;
        DVECTOR   field_3C;
        s32       field_40;
        DVECTOR   field_44;
        DVECTOR   field_48;
    } s_func_80065B94;

    typedef struct
    {
        POLY_GT4* field_0;
        MATRIX    field_4;
        SVECTOR   field_24[3];
        VECTOR3   field_3C; // Q19.12
        s32       field_48;
        DVECTOR   field_4C;
        DVECTOR   field_50;
        DVECTOR   field_54;
        DVECTOR   field_58;
        s32       field_5C;
        s32       field_60;
        s32       field_64;
        s32       field_68;
        s32       field_6C;
        s32       field_70;
        s32       field_74;
        s32       field_78;
    } s_func_80066184;

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

    typedef struct
    {
        VECTOR3 field_0; // Q23.8 | Position.
        MATRIX  field_C;
        s32     field_2C;
        DVECTOR field_30;
        q3_12   field_34[24];
        q3_12   field_64[24];
        q3_12   field_94[24];
        s16     field_C4;
        s16     field_C6;
        s16     field_C8;
        s16     field_CA;
        s16     field_CC;
        s16     field_CE;
        s32     field_D0;
        s32     field_D4;
        s32     field_D8;
        s16     field_DC[4];
        s16     field_E4[4];
        union
        {
            DVECTOR field_0[4]; // Q19.12
            s32     raw_0[4];
        } u_field_EC;
        union
        {
            DVECTOR field_0[4];
            s32     raw_0[4];
        } u_field_FC; // Q3.12 | Positions or offsets.
        q19_12  field_10C[4]; // X offsets?
        q19_12  field_11C[4]; // Z offsets?
    } s_func_8005E89C;

    typedef struct
    {
        s_func_8005E89C field_0;
        PACKET*         field_12C;
        CVECTOR         field_130;
        CVECTOR         field_134;
        SVECTOR         field_138;
        s32             field_140;
        DVECTOR         field_144;
        u16             field_148;
        u8              unk_14A[2];
        s32             field_14C;
        s32             field_150;
        s32             field_154;
        s32             field_158;
        s32             field_15C;
        s32             field_160;
        s32             field_164;
        s32             field_168;
        s32             field_16C;
        s32             field_170;
        s32             field_174;
    } s_func_80060044;

    typedef struct
    {
        s_func_8005E89C field_0;
        CVECTOR         field_12C;
        CVECTOR         field_130;
        SVECTOR         field_134;
        SVECTOR         field_13C;
        SVECTOR         field_144;
        VECTOR3         field_14C;
        s32             field_158;
        s32             field_15C;
        s32             field_160;
        s32             field_164;
        s32             field_168;
        DVECTOR         field_16C;
        s32             field_170;
        s32             field_174;
        q19_12          field_178;
        s32             field_17C;
        s32             field_180;
        s32             field_184;
        s32             field_188;
        s32             field_18C;
        s32             field_190;
        s32             field_194;
        s32             field_198;
        s32             field_19C;
        s32             field_1A0;
    } s_func_800611C0;

    typedef struct
    {
        s_func_8005E89C field_0;
        CVECTOR         field_12C;
        CVECTOR         field_130;
        SVECTOR         field_134[25];
        VECTOR3         field_1FC; // Q19.12
        q20_12          field_208;
        s32             field_20C;
        s32             field_210;
        s32             field_214[25];
        DVECTOR         field_278[25];
        s32             field_2DC;
    } s_func_80062708;

    typedef struct
    {
        s_func_8005E89C field_0;
        SVECTOR         field_12C;
        s8              unk_138[8];
        DVECTOR         field_13C;
        s32             field_140;
        s32             field_144;
    } s_func_80064FC0;

    typedef struct
    {
        POLY_G4*  field_0;
        DR_TPAGE* field_4;
        SVECTOR   field_8[3];
        MATRIX    field_20;
        VECTOR3   field_40; // Q23.8
        s32       field_4C;
        s16       field_50;
        s32       field_54[3];
        s16       field_60[3];
        s32       field_68;
    } s_func_800652F4;

    typedef struct
    {
        s_func_8005E89C field_0;
        MATRIX          field_12C;
        SVECTOR         field_14C[3];
        SVECTOR         field_164;
        VECTOR          field_16C[3];
        VECTOR          field_19C;
        VECTOR          field_1AC; // Q27.4
        s32             field_1BC;
        s32             field_1C0;
        s32             field_1C4;
        s32             field_1C8;
        DVECTOR         field_1CC;
        s16             field_1D0;
        u8              unk_1D2[2];
        s32             field_1D4; // Count.
        s32             field_1D8;
        s32             field_1DC;
        s32             field_1E0;
        s32             field_1E4;
        q19_12          field_1E8;
    } s_func_80063A50;

    typedef struct
    {
        s_func_8005E89C field_0;
        PACKET*         field_12C;
        CVECTOR         field_130;
        CVECTOR         field_134;
        SVECTOR         field_138;
        u8              unk_140[16];
        s32             field_150;
        DVECTOR         field_154;
        u16             field_158;
        u16             field_15A;
        u8              field_15C;
        s32             field_160;
        s32             field_164;
        s32             field_168;
    } s_func_80064334;

    typedef struct
    {
        s_func_8005E89C    field_0;
        s_CollisionSurface surface;
        MATRIX             field_138;
        SVECTOR            field_158[4];
        VECTOR             field_178[4];
        CVECTOR            field_1B8;
        CVECTOR            field_1BC;
        s32                field_1C0;
        s32                field_1C4;
        s32                field_1C8;
        s32                field_1CC;
        s32                field_1D0;
        DVECTOR            field_1D4;
    } s_func_800CD1F8;

    typedef struct
    {
        MATRIX  field_0; // View matrix?
        SVECTOR field_20;
        VECTOR  field_28; // Q27.4
        s32     field_38;
        s32     field_3C[5];
        s32     field_50;
    } s_func_8006342C;

    extern s_FsImageDesc g_MainImg0; // 0x80022C74 - TODO: Part of main exe, move to `main/` headers?

    extern char D_80028544[16];

    extern RECT D_80028A20;

    extern const s_AnimInfo HARRY_WEAPON_ANIM_INFOS[];

    extern const s_800C44F0 D_800294F4[];

    extern const s_AnimInfo* D_800297B8;

    extern u_Filename D_8002B2CC;

    extern s_FsImageDesc g_PaperMapImg;
    extern s_FsImageDesc g_PaperMapMarkingAtlasImg;
    extern s_FsImageDesc g_ItemInspectionImg;
    extern s_FsImageDesc g_LoadingScreenImg;
    extern s_FsImageDesc g_ControllerButtonAtlasImg;
    extern s_FsImageDesc g_DeathTipImg;
    extern s_FsImageDesc g_HealthPortraitImg;
    extern s_FsImageDesc g_InventoryKeyItemTextureImg;
    extern s_FsImageDesc g_FirstAidKitItemTextureImg;

    /** Some intentory item texture (`ITEM/TIM07.TIM`). */
    extern s_FsImageDesc D_800A9074;

    extern s_FsImageDesc D_800A907C;

    extern s_FsImageDesc D_800A9084;

    extern s_FsImageDesc D_800A908C;

    extern s_FsImageDesc D_800A9094;

    extern s_MapEffectsInfo MAP_EFFECTS_INFOS[21];

    extern s_StructUnk3 D_800A952C;

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

    /** Counts the amount of times that demos has been play in the current game session. */
    extern s8 g_Demo_ReproducedCount;

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

    extern s_EventData* g_ItemTriggerEvents[];

    /** Radio SFX data. */
    extern s_RadioNpcInfo g_RadioNpcInfos[2];

    extern s_MapPoint2d D_800BCDB0;

    /** Related to special item interactions. */
    extern s32 g_ItemTriggerItemIds[5];

    extern u8 D_800BCDD4;

    extern s_EventData* g_MapEventData;

    extern q3_12 g_Player_FlexRotationY;

    extern q3_12 g_Player_FlexRotationX;

    extern u8 g_Items_GunsMaxLoadAmmo[36]; // Max loaded ammo that a weapon can hold. 0x800AD4A0

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

    /** Related to main menu fog randomization. */
    extern s8* D_800BCDE0; // Type assumed.

    /** Angles. */
    extern s16 D_800BCDE8[8];

    extern s_WorldGfxWork g_WorldGfxWork;

    extern s8* D_800C15B0;

    extern s8 D_800C15B4;

    extern s16 D_800C16A4;

    extern s16 D_800C391E;

    extern s8 D_800C39A0;

    /** Functions from `bodyprog_8003AB28` access `g_WorldEnvWork` as constant, but some functions from `bodyprog_80055028` write to it.
     * It appears that g_WorldEnvWork is intended to be defined inside `bodyprog_80055028` as writable and declared as read-only (`const`) outside of it.
     */
    // extern s_WorldEnvWork g_WorldEnvWork;

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

    extern s16 D_800AEEDC[][2]; // Type assumed.

    extern s16 D_800AEF78[][2]; //  Type assumed.

    extern s16 D_800AF014[];

    extern s16 D_800AF04C[];

    extern q3_12 D_800AF070[];

    extern q3_12 D_800AF1FC[];

    extern u8 g_Player_AnimResetRequest; /** `bool` */

    extern s_CollisionSurface D_800C4620;

    /** Q19.12 */
    extern VECTOR3 D_800C4788[];

    extern VECTOR3 D_800C47B8;

    extern VECTOR3 D_800C47C8[];

    extern VECTOR3 D_800C47E8;

    extern s_RayTrace D_800C47F8;

    extern s_800C4818 D_800C4818;

    extern s32 D_800C48F0;

    extern u8 D_800AD480[24];

    /** Weapon attacks. */
    extern s_800AD4C8 D_800AD4C8[70];

    extern s_MapOverlayHdr g_MapOverlayHdr;

    extern s16 SQRT[100];
}
