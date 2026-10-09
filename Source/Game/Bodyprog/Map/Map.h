#pragma once

#include "Game/Bodyprog/Anim.h"
#include "Game/Bodyprog/Chara/Chara.h"
#include "Game/Bodyprog/Collision/Collision.h"
#include "Game/Bodyprog/Formats/Anm.h"
#include "Game/Bodyprog/View/Structs.h"

namespace Silent::Game
{
    struct s_PlayerExtra;

    constexpr int CAMERA_PATH_COUNT_MAX       = 100;
    constexpr int COLLISION_TRIGGER_COUNT_MAX = 200;

    /** @brief Map flags. */
    enum e_MapFlags
    {
        MapFlag_FourActiveChunks = 0,      /** Used by exterior maps. */
        MapFlag_OneActiveChunk   = 1 << 0,
        MapFlag_TwoActiveChunks  = 1 << 1,
        MapFlag_Interior         = 1 << 2,
        MapFlag_3                = 1 << 3  /** @unused Unused map type `XXX` has this flag. */
    };

    /** @brief Character spawn flags. */
    enum e_SpawnFlags
    {
        SpawnFlag_None = 0,
        SpawnFlag_0    = 1 << 0,
        SpawnFlag_1    = 1 << 1,
        SpawnFlag_2    = 1 << 2,
        SpawnFlag_3    = 1 << 3,
        SpawnFlag_4    = 1 << 4
    };

    /** @brief Playable map indices. Used for binary overlays. */
    enum e_MapIdx
    {
        MapIdx_MAP0_S00 = 0,
        MapIdx_MAP0_S01 = 1,
        MapIdx_MAP0_S02 = 2,
        MapIdx_MAP1_S00 = 3,
        MapIdx_MAP1_S01 = 4,
        MapIdx_MAP1_S02 = 5,
        MapIdx_MAP1_S03 = 6,
        MapIdx_MAP1_S04 = 7,
        MapIdx_MAP1_S05 = 8,
        MapIdx_MAP1_S06 = 9,
        MapIdx_MAP2_S00 = 10,
        MapIdx_MAP2_S01 = 11,
        MapIdx_MAP2_S02 = 12,
        MapIdx_MAP2_S03 = 13,
        MapIdx_MAP2_S04 = 14,
        MapIdx_MAP3_S00 = 15,
        MapIdx_MAP3_S01 = 16,
        MapIdx_MAP3_S02 = 17,
        MapIdx_MAP3_S03 = 18,
        MapIdx_MAP3_S04 = 19,
        MapIdx_MAP3_S05 = 20,
        MapIdx_MAP3_S06 = 21,
        MapIdx_MAP4_S00 = 22,
        MapIdx_MAP4_S01 = 23,
        MapIdx_MAP4_S02 = 24,
        MapIdx_MAP4_S03 = 25,
        MapIdx_MAP4_S04 = 26,
        MapIdx_MAP4_S05 = 27,
        MapIdx_MAP4_S06 = 28,
        MapIdx_MAP5_S00 = 29,
        MapIdx_MAP5_S01 = 30,
        MapIdx_MAP5_S02 = 31,
        MapIdx_MAP5_S03 = 32,
        MapIdx_MAP6_S00 = 33,
        MapIdx_MAP6_S01 = 34,
        MapIdx_MAP6_S02 = 35,
        MapIdx_MAP6_S03 = 36,
        MapIdx_MAP6_S04 = 37,
        MapIdx_MAP6_S05 = 38,
        MapIdx_MAP7_S00 = 39,
        MapIdx_MAP7_S01 = 40,
        MapIdx_MAP7_S02 = 41,
        MapIdx_MAP7_S03 = 42,
        MapIdx_MAPT_S00 = 43, // } @unused Empty test maps. Only some code references remain and `HB_MTS00.ANM`/`HB_MTX00.ANM` anim files.
        MapIdx_MAPX_S00 = 44  // }
    };

    /** @brief Used as index into `MAP_INFOS` array. 
     *
     * @note
     * R: "Real"
     * U: "Underworld"
     */
    enum e_MapType
    {
        MapType_THR = 0,  /** Old Silent Hill. */
        MapType_SC  = 1,  /** Real School. */
        MapType_SU  = 2,  /** Alt. School. */
        MapType_SPR = 3,  /** Real Central Town. */
        MapType_SPU = 4,  /** Alt. Central Town */
        MapType_RSR = 5,  /** Real Resort. */
        MapType_RSU = 6,  /** Alt. Resort. */
        MapType_APR = 7,  /** @unused Real Amusement Park. */
        MapType_APU = 8,  /** Alt. Amusement Park. */
        MapType_ER  = 9,  /** Nowhere. */
        MapType_ER2 = 10, /** Bonus Old Town areas. */
        MapType_DR  = 11, /** Real Sewers. */
        MapType_DRU = 12, /** Alt. Sewers. */
        MapType_HP  = 13, /** Real Hospital. */
        MapType_HU  = 14, /** Alt. Hospital. */
        MapType_XXX = 15, /** @unused Test map? */

        MapType_Count
    };

    /** @brief Speed zone types for player movement speed modulation. */
    enum e_SpeedZoneType
    {
        SpeedZoneType_Slow   = 0,
        SpeedZoneType_Normal = 1,
        SpeedZoneType_Fast   = 2
    };

    /** @brief Event trigger activation types. */
    enum e_TriggerActivationType
    {
        TriggerActivationType_None      = 0, /** No activation conditions other than event flag/trigger checks. */
        TriggerActivationType_Exclusive = 1, /** Prevents other events from being triggered while the event is active. */
        TriggerActivationType_Button    = 2, /** Requires a button press. */
        TriggerActivationType_Item      = 3  /** Requires an inventory item. */
    };

    /** @brief Event trigger types. */
    enum e_TriggerType
    {
        TriggerType_EndOfArray     = NO_VALUE,
        TriggerType_Tick           = 0, /** Skips trigger/activation type checks.
                                         * Always activates if required event flags are set.
                                         * Skips processing later events until flags deactivate it.
                                         */ 
        TriggerType_TouchAabb      = 1, /** Player has collided with an AABB. */
        TriggerType_TouchFacing    = 2, /** Player collided with a trigger is facing toward it. */
        TriggerType_TouchObbFacing = 3, /** Player collided with an OBB and is facing toward it. */
        TriggerType_TouchObb       = 4  /** Player collided with an OBB. No facing requirement. */
    };

    /** @brief Area transition flags. Used by some events to indicate specific cutscene behavior. */
    enum e_AreaTransitionFlags
    {
        AreaTransitionFlag_None               = 0,
        AreaTransitionFlag_UnfreezeWorld      = 1 << 0, /** TODO: Dual purpose: `SysState_ReadMessage` unfreezes the world if set, while `GameBoot_WorldStartup` calls `Map_WorldClear`? */
        AreaTransitionFlag_SkipFadeIn         = 1 << 1, /** Skips fade-in when returning to `GameState_InGame`. */
        AreaTransitionFlag_SkipAmbientSfxInit = 1 << 2  /** TODO: Skips the `Sd_AmbientSfxInit` call in `GameBoot_WorldStartup`? */
    };

    enum e_WorldModelLocation
    {
        WorldModelLocation_None   = 0,
        WorldModelLocation_Global = 1,
        WorldModelLocation_Chunk1 = 2,
        WorldModelLocation_Chunk2 = 3,
        WorldModelLocation_Chunk3 = 4,
        WorldModelLocation_Chunk4 = 5
    };

    // Something related to enemies attacks.
    typedef struct
    {
        union
        {
            q19_12 vx_0; // Displacement offset.
            struct
            {
                s16 field_0;
                s16 field_2;
            } s_0;
            struct
            {
                u16 field_0;
                u16 field_2;
            } s_1;
        } field_0;
        union
        {
            q19_12 vz_4;
            struct
            {
                s16 field_0;
                s16 field_2;
            } s_0;
        } field_4;
        q3_12 vy_8;
        u8    field_A;
        u8    field_B; // Flags?
        union
        {
            s32 field_0;
            struct
            {
                q3_12 field_0; // Angle.
                q3_12 field_2; // Angle.
            } s_0;
            struct
            {
                u8 field_0;
                u8 field_1;
                u8 field_2;
                u8 field_3;
            } s_1;
            struct
            {
                q4_12 field_0;
                u16   field_2;
            } s_2;
        } field_C;
        union
        {
            q20_12 field_0; // Timer.
            struct
            {
                s16 field_0;
                s16 field_2;
            } s_0;
            struct
            {
                s16 field_0;
                u8  field_2;
                u8  field_3;
            } s_1;
            struct
            {
                u8 field_0;
                u8 field_1;
                u8 field_2;
                u8 field_3;
            } s_2;
            struct
            {
                u16 field_0;
                u16 field_2;
            } s_3;
        } field_10;
    } s_MapHdr_field_4C;

    /** @brief Speed zone. Defines a volume used for player speed modulation. */
    struct s_SpeedZone
    {
        s8   type; /** `e_SpeedZoneType` */
        // 1 byte of padding.
        q11_4 minX;
        q11_4 maxX;
        q11_4 minZ;
        q11_4 maxZ;
    };

    /** @brief Water zone. Defines a volume used for visual water effects. */
    struct s_WaterZone
    {
        u8    isEnabled; /** `bool` */
        // 1 byte of padding.
        q11_4 illumination;
        q11_4 minX;
        q11_4 maxX;
        q11_4 minZ;
        q11_4 maxZ;
    };

    /** @brief Map GFX info. */
    struct s_MapInfo
    {
        s16                plmFileIdx;
        char               tag[4];
        u8                 flags; /** `e_MapFlags` */
        // 1 byte of padding.
        s_WaterZone* waterZones;
        s_SpeedZone* speedZones;
    };

    typedef struct
    {
        u8    unk_0;
        u8    field_1;
        u8    field_2;
        u8    unk_3;
        u8    field_4;
        u8    field_5;
        u8    field_6;
        u8    unk_7;
        u8    unk_8[2];
        s16   field_A;
        u8    unk_C[2];
        s16   field_E;
        q3_12 field_10;
        u8    unk_12[4];
        s16   field_16;
        u8    unk_18[8];
        s32   field_20;
        s32   field_24;
    } s_MapOverlayHdr_5C;

    typedef struct
    {
        u8    field_0;
        u8    field_1;
        u8    field_2;
        u8    field_3;
        s16   field_4;
        s16   field_6;
        s16   field_8;
        s16   field_A;
        s16   field_C;
        s16   field_E;
        q3_12 field_10;
        s16   field_12;
        s32   field_14;
        s32   field_18;
        s32   field_1C;
    } s_MapOverlayHdr_7C;

    // Related to water particles.
    typedef struct
    {
        s32    field_0[4];
        q3_12  field_10[4];
        q19_12 field_18[4];
        q4_12  field_28[4];
        u8     field_30[4];
        q3_12  field_34[4]; // Y angles.
        q4_12  field_3C[4];
        q4_12  field_44[4];
        q3_12  field_4C[4];
        u16    field_54[4];
        q4_12  field_5C[4];
        q4_12  field_64[4];
        q3_12  field_6c[4];
        s16    field_74; // Particle count?
        s8     __pad_76[2];
        u8     field_78; // Active index for above arrays.
    } s_MapOverlayHdr_94;

    /** @brief Contains X/Z coordinates and optional 4 bytes of data.
     * Map headers include an array of these, into which `s_EventData` includes an index. */
    struct s_MapPoint2d
    {
        q19_12 positionX;
        u32    paperMapIdx     : 5; /** `e_PaperMapIdx` */
        u32    field_4_5       : 4;
        u32    loadingScreenId : 3; /** `e_LoadingScreenId` */
        u32    unused_4_12     : 4; /** @unused Always set to 0. */
        q24_8  triggerParam0   : 8; // Usually a `Q8_ANGLE`.
        u32    triggerParam1   : 8;
        q19_12 positionZ;
    };

    /** TODO: Known as `Trigger` in SilentHillMapExaminer: https://github.com/ItEndsWithTens/SilentHillMapExaminer/blob/master/src/SHME.ExternalTool.Guts/Trigger.cs */
    struct s_EventData
    {
        s16 requiredEventFlag;  /** `e_EventFlag` | Must be set for an event to trigger (or `EventFlag_None`). */
        s16 completeEventFlag;  /** `e_EventFlag` | Set on event completion. The event is skipped if this flag is already set. */
        s8  triggerType    : 4; /** `e_TriggerType` */
        u8  activationType : 4; /** `e_TriggerActivationType` */
        u8  mapPointIdx;        /** Index into `g_MapOverlayHdr.mapPoints`. */
        u8  requiredItemId;     /** `e_InvItemId` that player must use from item screen. */
        u8  __pad_7;
        u32 sysState        : 5; /** `e_SysState` used by the event. */
        u32 eventParam      : 8; /** Can be an ID of a `MapMsg`, sound effect, index into `mapEventFuncs`, or index into `mapPoints` for `areaLoad` events. */
        u32 transitionFlags : 6; /** `e_AreaTransitionFlags` */
        u32 sfxPairIdx_8_19 : 5; /** `e_SfxPairIdx` | Index into `SFX_PAIRS`. */
        u32 field_8_24      : 1; // `bool` | "Is on camera rail?"
        u32 mapIdx          : 6;
        u32 field_8_31      : 1;
    };

    /** @brief Special map-specific Harry anim data. */
    typedef struct
    {
        s16   status; /** Packed anim status. See `s_ModelAnim::status`. */
        s16   status_2; /** Packed anim status. See `s_ModelAnim::status`. */
        q3_12 time;   /** Fixed-point anim time. */
        s16   keyframeIdx_6;
    } s_UnkStruct3_Mo;

    /** Guessed based on in-debugger observation during gameplay.
     * Everything is inited to 0xFFFF and some data is written when the player is hit by monsters.
     * Might be more generic particles/decals struct.
     */
    struct s_BloodSplat
    {
        s16 field_0; // Maybe an array of `u16`s? Contains indices.
    };

    /** @brief Character spawn info. */
    struct s_SpawnInfo
    {
        q19_12 positionX;
        s8     charaId; /** `e_CharaId` */
        q0_8   rotationY;
        s8     flags;                 /** `e_SpawnFlags` | Copied to `stateStep` in `s_Model`, with `controlState = 0`. */
        s32    gameDifficultyMin : 4; /** `e_GameDifficulty` | Minimum difficulty required for successful spawn. */
        q19_12 positionZ;
    };

    /** TODO: `g_MapOverlayHdr` is part of the map overlay BIN files. Maybe should be moved to `maps/shared.h`.
     * If field has a comment that lists only certain maps, it means all others set this field to 0.
     * func(?) means the signature is unknown and a default void() was selected for now.
     */
    struct s_MapOverlayHdr
    {
        s_MapInfo*             mapInfo;
        u8                     (*mapRoomIdxGet)(q19_12 posX, q19_12 posY); // Called by `Game_MapRoomIdxUpdate`.
        s8                     unused_8;
        s32                    (*func_C)();
        void                   (*bgmEvent)(bool);
        s8                     bgmIdx;
        u8                     ambientAudioIdx; // Ambient file index from `g_AmbientVabTaskLoadCmds`.
        s8                     field_16;        // Set ambient tint and draw distance.
                                                // A value of 3 sets the map to night.
                                                // A value of 2 will sets the map to use a tint used during the hallaway
                                                // intro.
                                                // Giving this and next variable a name will require further investigation
                                                // of `particle.c` code.
        s8                     field_17;        // Set weather to play. Rain, heavy rain, or snow.
        void                   (**loadingScreenFuncs)();
        s_MapPoint2d*          mapPoints;
        void                   (**mapEventFuncs)(); /** Points to array of event functions. */
        s_EventData*           mapEvents;
        GsCOORDINATE2*         npcBoneCoordBuffer; /** Dynamic NPC bone coord buffer used as backup for `s_SysWork::npcBoneCoordBuffer`.
                                                    * In practice, this is always set to `g_SysWork.npcBoneCoordBuffer`,
                                                    * effectively forming a ring buffer. */
        u8*                    loadableItems;
        const char**           mapMessages;
        s_AnimInfo*            harryMapAnimInfos; /** Map-specific anim infos for Harry (for anims 38+). */
        s_UnkStruct3_Mo*       field_38; // Array of 40?
        void                   (*initWorldObjects)(void);
        void                   (*updateWorldObjects)(void);
        void                   (*func_44)();
        void                   (*npcSpawnEvent)();
        s_MapHdr_field_4C*     unkTable1_4C; // Related to collision?
        s16                    unkTable1Count_50;
        s_BloodSplat*          bloodSplats;
        s16                    bloodSplatCount;
        s_MapOverlayHdr_5C*    field_5C;

        void                   (*func_60)(s32 idx, bool arg1);
        s32                    (*func_64)(POLY_FT4** poly, s32);
        s32                    (*func_68)(POLY_FT4** poly, s32);
        void                   (*func_6C)();
        s32                    (*func_70)(POLY_FT4** poly, s32);
        void                   (*func_74)();
        s32                    (*func_78)(POLY_FT4** poly, s32);
        s_MapOverlayHdr_7C* field_7C; // Only map1_s01, map6_s04.
        void                   (*func_80)();
        s32                    (*func_84)(POLY_FT4** poly, s32);
        s32*                   func_88;                          // Only map0_s01, map7_s01.
        s32                    (*func_8C)(POLY_FT4** poly, s32); // Only map0_s01, map7_s01.
        s32                    (*func_90)(POLY_FT4** poly, s32); // Only map5_s00.
        s_MapOverlayHdr_94* field_94;                         // Only map1_s02, map1_s03.
        s32                    (*func_98)(POLY_FT4** poly, s32); // Only map1_s02, map1_s03.
        void                   (*func_9C)();                     // Only map1_s02, map1_s03.
        void*                  ptr_A0;                           // M1S03 only, pointer to `s_800E3A40` array.
        s32                    (*func_A4)(POLY_FT4** poly, s32); // Only map1_s03.
        void                   (*func_A8)();                     // Only map6_s02.
        s32                    (*func_AC)(POLY_FT4** poly, s32); // Only map4_s03, map4_s05.
        s32                    (*func_B0)(POLY_FT4** poly, s32); // Only map4_s03, map4_s05.
        s32                    (*func_B4)(POLY_FT4** poly, s32); // Only map1_s03, map4_s05, map6_s01, map6_s02, map5_s01.
        void                   (*func_B8)(s_SubCharacter* chara, s_PlayerExtra* extra, GsCOORDINATE2* coords);
        void                   (*func_BC)(s_SubCharacter* chara, s_PlayerExtra* extra, GsCOORDINATE2* coords);
        void                   (*func_C0)();
        void                   (*playerMatchArmAnimDisable)();
        void                   (*playerControlFreeze)();
        void                   (*playerControlUnfreeze)(bool setIdle);
        bool                   (*playerPathWaypointExecute)(s32 playerExtraState, VECTOR3* vec, q3_12 angle, s32 vecCount); // 0x800C964C
        s32                    (*playerAnimStateSet)(s32 playerExtraState); // This only changes the state of player extra data, but gets used by same function that calls `charaAnimStateSet` when player chara is passed.
                                                                            // @hack Points to `Player_AnimStateSet` which has `void` return type, but changing this funcptr causes mismatch in `Event_CharaAnimCmdExecute`.
        void                   (*playerAnimReset)();
        void                   (*playerAnimLock)(void);
        void                   (*playerAnimIsLocked)(void);
        void                   (*playerAnimUnlock)(void);
        s32                    (*playerAnimPlaybackStateGet)(void);
        bool                   (*playerMoveSpeedIsZero)(void);
        void                   (*playerMoveSpeedClear)(void);
        void                   (*playerFallBackward)(void);
        void                   (*playerStub_F8)();
        void                   (*playerDamageFeetFront)(void);
        void                   (*func_100)();
        void                   (*func_104)();
        s32                    (*func_108)();
        void                   (*func_10C)();
        void                   (*func_110)();
        void                   (*func_114)();
        void                   (*func_118)();
        void                   (*func_11C)();
        void                   (*func_120)();
        void                   (*charaAnimStateSet)(s_SubCharacter* chara, q19_12 afkTime);
        s32                    (*charaAnimReset)(s_SubCharacter* player); // TODO: Not only used by the player.
        s32                    (*charaAnimLock)(s_SubCharacter* chara);
        void                   (*charaAnimIsLocked)(s_SubCharacter* chara);
        s32                    (*charaAnimUnlock)(s_SubCharacter* chara);
        s32                    (*charaAnimPlaybackStateGet)(s_SubCharacter* chara);
        bool                   (*charaPathWaypointExecute)(s_SubCharacter* chara, s32 arg1, VECTOR3* arg2In, s32 angleIn, s32 arg4);
        void                   (*charaVisibleSet)(s_SubCharacter* chara);
        void                   (*charaInvisibleSet)(s_SubCharacter* chara);
        bool                   (*func_148)(s32 animStatus, s_SubCharacter* chara, s32 keyframeIdx0, s32 keyframeIdx1, s32 sfxId, s32 pitch);
        bool                   (*func_14C)(s32 animStatus, s_SubCharacter* chara, s32 keyframeIdx, s32 sfxId); // Play SFX?
        s32                    (*charaAnimStartKeyframeIdxGet)(s_SubCharacter* chara);
        void                   (*charaCollisionReset)(s_SubCharacter* chara);
        void                   (*func_158)(q19_12 x, q19_12 z); // Only map1_s05, map1_s06.
        void                   (*func_15C)();                   // Only map5_s01.
        void                   (*func_160)();                   // Only map5_s01.
        void                   (*func_164)();                   // Only map5_s01.
        void                   (*particlesUpdate)(s32 unused, s32 mapId, s32);
        void                   (*enviromentSet)(s8, u32);
        void                   (*func_170)();
        void                   (*func_174)();
        void                   (*particleHyperBlasterBeamDraw)(VECTOR3* vec0, q19_12* rotX, q19_12* rotY);
        void                   (*particleBeamDraw)(const VECTOR3* from, const VECTOR3* to);
        void                   (*particleSoundStop)();

        q19_12*                windSpeedX;
        q19_12*                windSpeedZ;
        s32*                   data_18C;
        s32*                   data_190;
        void                   (*charaUpdateFuncs[Chara_Count])(s_SubCharacter* chara, s_AnmHeader* anmHdr, GsCOORDINATE2* coords); /** Guessed params. Funcptrs for each `e_CharaId`, set to 0 for IDs not included in the map overlay. Called by `Game_NpcUpdate`. */
        s8                     charaGroupIds[CHARA_GROUP_COUNT]; /** `e_CharaId` values where if `s_SpawnInfo::charaId == Chara_None`, `charaGroupIds[0]` is used for `charaSpawnInfos[0]` and `charaGroupIds[1]` for `charaSpawnInfos[1]`. */
        s_SpawnInfo            charaSpawnInfos[2][16];           /** Array of character type/position/flags. `flags == SpawnFlags_None` are unused slots? Read by `Game_NpcRoomInitSpawn`. */
        VC_ROAD_DATA           cameraPaths[CAMERA_PATH_COUNT_MAX];
        s_CollisionTrigger     collisionTriggers[COLLISION_TRIGGER_COUNT_MAX];
    };

    extern const s_MapInfo MAP_INFOS[MapType_Count];

    /** @brief Checks if a paper map has been collected.
     *
     * @param paperMapIdx Paper map index.
     * @return `true` if the paper map has been collected, `false` otherwise.
     */
    #define HAS_PAPER_MAP(paperMapIdx) \
        g_SavegamePtr->paperMapFlags.Test(paperMapIdx)
}
