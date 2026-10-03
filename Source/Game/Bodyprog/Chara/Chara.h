#pragma once

#include "Game/Bodyprog/Anim.h"
#include "Game/Bodyprog/Model.h"
#include "Game/Main/FsQueue.h"

namespace Silent::Game
{
    constexpr int  NPC_COUNT_MAX        = 6;
    constexpr int  NPC_BONE_COUNT_MAX   = 10 * NPC_COUNT_MAX;
    constexpr int  CHARA_GROUP_COUNT    = 4;                  /** While up to 6 NPCs and a player can exist in the game world, only 4 different character
                                                               * types (including the player) can be loaded at a time.
                                                               */
    constexpr int  CHARA_FORCE_FREE_ALL = 0xFF;               /** `Chara_Load` can force free already loaded models to make room for new ones. */

    /** @brief Character flags. Used by `s_SubCharacter::flags`. */
    enum e_CharaFlags
    {
        CharaFlag_None          = 0,
        CharaFlag_PadlockBroken = 1 << 0, /** Only used by Padlock character. */
        CharaFlag_Unk2          = 1 << 1, // Targeted by player?
        CharaFlag_Hit           = 1 << 2, // Associated with recoil-causing hit?
        CharaFlag_Unk4          = 1 << 3,
        CharaFlag_Unk5          = 1 << 4, // Camera-related. Only used by Stalker? Maybe only for alley scenario?
        CharaFlag_Damaged       = 1 << 5,
        CharaFlag_Dead          = 1 << 6, // Unure.
        CharaFlag_Unk8          = 1 << 7,
        CharaFlag_NoRadioStatic = 1 << 8
    };

    /** @brief Character group flags. Used by `s_SysWork::charaGroupFlags`. */
    enum e_CharaGroupFlags
    {
        CharaGroupFlag_None = 0,
        CharaGroupFlag_0    = 1 << 0,
        CharaGroupFlag_1    = 1 << 1
    };

    /** @brief Character collision states. */
    enum e_CharaCollisionState
    {
        CharaCollisionState_Ignore = 0,
        CharaCollisionState_Player = 1,
        CharaCollisionState_2      = 2,
        CharaCollisionState_Npc    = 3,
        CharaCollisionState_4      = 4,
        CharaCollisionState_5      = 5
    };

    /** @brief Character IDs.
     *
     * @note The `CHARA_FILE_INFOS` array associates each character ID with asset files. */
    enum e_CharaId
    {
        Chara_None             = 0,

        /** Player characters. */

        Chara_Harry            = 1,

        /** Enemy characters. */

        Chara_AirScreamer      = 2,
        Chara_NightFlutter     = 3,
        Chara_Groaner          = 4,
        Chara_Wormhead         = 5,
        Chara_LarvalStalker    = 6,
        Chara_Stalker          = 7,
        Chara_GreyChild        = 8,
        Chara_Mumbler          = 9,
        Chara_HangedScratcher  = 10,
        Chara_Creeper          = 11,
        Chara_Romper           = 12,
        Chara_Chicken          = 13, /** @unused */
        Chara_SplitHead        = 14,
        Chara_Floatstinger     = 15,
        Chara_PuppetNurse      = 16,
        Chara_DummyNurse       = 17, /** Uses dummy anim file without model/texture, but the same update funcptr as `Chara_PuppetNurse`. */
        Chara_PuppetDoctor     = 18,
        Chara_DummyDoctor      = 19, /** Uses dummy anim file without model/texture, but the same update funcptr as `Chara_PuppetDoctor`. */
        Chara_Twinfeeler       = 20,
        Chara_Bloodsucker      = 21,
        Chara_Incubus          = 22,
        Chara_Unknown23        = 23,
        Chara_MonsterCybil     = 24,

        /** Cutscene characters. */

        Chara_LockerDeadBody   = 25,
        Chara_Cybil            = 26,
        Chara_EndingCybil      = 27,
        Chara_Cheryl           = 28,
        Chara_Cat              = 29,
        Chara_Dahlia           = 30,
        Chara_EndingDahlia     = 31,
        Chara_Lisa             = 32,
        Chara_BloodyLisa       = 33,
        Chara_Alessa           = 34,
        Chara_GhostChildAlessa = 35,
        Chara_Incubator        = 36,
        Chara_BloodyIncubator  = 37,
        Chara_Kaufmann         = 38,
        Chara_EndingKaufmann   = 39,
        Chara_Flauros          = 40,
        Chara_LittleIncubus    = 41,
        Chara_GhostDoctor      = 42,
        Chara_Parasite         = 43,

        /** Special characters. */

        Chara_Padlock          = 44,

        Chara_Count
    };

    // Collision-related.
    typedef struct
    {
        VECTOR3 position;
        q3_12   field_C; // Set to character Y position. Name it `height`?
        s16     field_E;
        s16     field_10;
        s8      collisionState; /** `e_CharaCollisionState` */
        u8      field_13;
    } s_func_8006CF18;

    /** @brief Character damage info. */
    struct s_CharaDamage
    {
        VECTOR3 position;
        q19_12  amount;
    };

    typedef union
    {
        s32 val32;
        s16 val16[2];
        s8  val8[4];
    } u_Property;

    /** @brief Temporary struct. */
    struct s_PropsDummy
    {
        u_Property properties_E8[16];
    };

    /** @brief Player character properties. */
    struct s_PropsPlayer
    {
        q19_12        afkTimer; // Increments every tick for 10 seconds before AFK anim starts.
        q19_12        groundHeight;
        q19_12        field_F0;
        q19_12        field_F4; // Angle related to X axis flex rotation.
        s32           runStepSfxCount;
        q19_12        exhaustionTimer;
        q19_12        field_100;    // Angle?
        q19_12        field_104;    // Distance?
        q19_12        runDistance;
        u8            field_10C;    // Player SFX pitch?
        u8            field_10D;
        s8            __pad_10E[2];
        q19_12        timer_110; // Increases when `flags & CharaFlag_Unk4` is set, reset when reaches `D_800C45EC`.
        q19_12        gasWeaponPowerTimer; // Timer for the rock drill and chainsaw power.
        s16           field_118; // q3_12?
        s8            __pad_11A[2];
        s32           flags; /** `e_PlayerFlags` */
        q3_12         quickTurnHeadingAngle; /** Target quick turn heading angle. */
        q3_12         field_122; // Specially used when aiming an enemy. Y angle delta to target? 
        q3_12         headingAngle;
        q3_12         moveSpeed; // Used to indicate how much the player should move foward. Seems to be squared.
    };

    // TODO: Unsure if this struct is puppet doctor specific or shared with all characterss. Pointer gets set at puppetDoc+0x124.
    typedef struct
    {
        q19_12      health_0;
        s32         field_4;
        s32         field_8;
        s32         field_C;
        s8          unk_10[8];
        s32         field_18;
        s32         idx_1C;
        s32         field_20;
        s_AnimInfo* animInfo_24;
        s8          unk_28[4];
        q19_12      field_2C;
        s8          unk_30[4];
    } s_800D5710;

    // Used by Alessa, Bloody Incubator, BloodyLisa, Cheryl, Cybil, Dahlia, Ghost Child Alessa, Ghost Doctor, Incubator,
    // Kaufmann, Lisa, Monster Cybil.
    struct s_PropsNpc
    {
        s32    controlState; /** `e_*Control` */
        s16    field_EC;
        s16    field_EE; // Anim index?
        s32    freeze; // `bool`, `q19_12` timer in MonsterCybil.
        s32    field_F4;
        s32    resetControlState; // `bool`, `q19_12` timer in MonsterCybil.
        s8     __pad_FC[2];
        s16    field_FE; // Index.
        q19_12 field_100;
        s16    field_104;
        q3_12  field_106; // Angle or counter (`func_800D8D7C`)? May change usage depending on state step?
        q19_12 field_108; // Timer.
        q19_12 field_10C; // Timer.
        q19_12 distanceToPlayer;
        u8     field_114;
        u8     field_115;
        u8     field_116;
        s8     __pad_117;
        q3_12  field_118; // Angle.
        q3_12  field_11A; // Angle.
        s32    flags; /** `e_*Flags` */
        s16    field_120; // `bool`
        q3_12  field_122; // Angle related to weapon target.
        q3_12  moveDistance_124;
        q3_12  moveSpeed;
    };

    /** @brief Air Screamer or Night Flutter character properties. */
    struct s_PropsAirScreamer
    {
        u32     field_E8_0  : 4; // `AirScreamerHit_None` step.
        u32     field_E8_4  : 4; /** `bool` */
        u32     field_E8_8  : 4;
        u32     __pad_E8_12 : 20;
        s32     field_EC;
        s16     field_F0; // } Maybe 2D offset like in Creeper properties? Must check.
        s16     field_F2; // }
        s32     field_F4;
        VECTOR3 targetPosition; /** Q19.12 */
        VECTOR3 position_104;   /** Q19.12 | Set to either Air Screamer position with slight offset toward player or player position. */
        VECTOR3 position_110;
        s32     flags; /** `e_AirScreamerFlags` */
        q19_12  timer_120;
        q19_12  groundHeight;
    };

    /** @brief Bloodsucker character properties. */
    struct s_PropsBloodsucker
    {
        q19_12 timer_E8;
        q19_12 timer_EC;
        q19_12 timer_F0;
        q19_12 timer_F4;
        s8     unused_F8[36]; /** @unused Probably explicit filler fields originally. */
        s32    flags;         /** `e_BloodsuckerFlags` */
    };

    /** @brief Cat character properties. */
    struct s_PropsCat
    {
        u8 field_E8; // `bool`
    };

    /** @brief Creeper character properties. */
    struct s_PropsCreeper
    {
        u16    flags; /** `e_CreeperFlags` */
        s8     __pad_EA[2];
        q3_12  collisionOffsetX;
        q3_12  collisionOffsetZ;
        q19_12 attackTimer;
        q19_12 targetPositionX;
        q19_12 targetPositionZ;
        q19_12 homePositionX;
        q19_12 homePositionZ;
        q19_12 chirpTimer;
        q3_12  angleToTarget;
        s16    animStatus_10A; // TODO: Purpose unclear.
        q4_12  moveSpeed;
    };

    /** @brief Floatstinger character properties. */
    struct s_PropsFloatstinger
    {
        s16        flags_E8;
        s8         unk_EA[2];
        q3_12      field_EC; // `vy` angle.
        q3_12      field_EE; // Angle.
        q19_12     field_F0; // Timer.
        q3_12      field_F4; // Angle.
        s8         __pad_F6[2];
        u16        field_F8;
        u16        field_FA;
        q4_12      field_FC; // Timer?
        s8         __pad_FE[2];
        q19_12     field_100; // Timer.
        u8         field_104;
        s8         __pad_105;
        s16        field_106;
        q19_12     field_108; // Damage related.
        q3_12      field_10C; // Angle, backup of `field_F4`?
        q3_12      field_10E; // Timer?
        s8         unk_110[24];
    };

    /** @brief Groaner character properties. */
    struct s_PropsGroaner
    {
        u_Property flags; /** `e_GroanerFlags` TODO: One weird exception where it's accessed as `s32`. */
        q3_12      targetHeadingAngle;
        q3_12      flexAngle;
        q3_12      field_F0; // } XZ offset?
        q3_12      field_F2; // }
        q19_12     targetPositionX;
        q19_12     targetPositionZ;
        q3_12      angleToTarget;
        q3_12      field_FE;
        u16        relKeyframeIdx_100;
        s8         __pad_102[2];
        q19_12     timer_104;
        u32        field_108;
        q3_12      timer_10C; // SFX timer?
        u8         field_10E; // } Sound states?
        u8         field_10F; // }
        u8         playLeftFootstepSfx;  /** `bool` */
        u8         playRightFootstepSfx; /** `bool` */
        s8         __pad_112[2];
        q3_12      field_114; // Move speed coefficient?
    };

    /** @brief Hanged Scratcher character properties. */
    struct s_PropsHangedScratcher
    {
        s16    flags; /** `e_HangedScratcherFlags` */
        q4_12  timer_EA;
        q3_12  targetHeadingAngle_EC;
        u8     field_EE;
        s8     __pad_EF;
        q3_12  offsetX_F0; // } Offsets passed to `Chara_MovementUpdate1`
        q3_12  offsetZ_F2; // }
        q19_12 positionX_F4;
        q19_12 positionZ_F8;
        q3_12  field_FC;
        s8     __pad_FE[2];
        q3_12  timer_100;
        u8     field_102;
        u8     field_103;
        s32    field_104;
        q3_12  timer_108;
        s8     __pad_10A[2];
        q4_12  radiusMax_10C; // } Used as `Chara_MoveSpeedUpdate` limit param, TODO: rename?
        q4_12  radiusMin_10E; // }
    };

    /** @brief Incubus character properties, shared with Unknown23? */
    struct s_PropsIncubus
    {
        q19_12 timer_E8;
        s32    field_EC; // Flags?
        s32    someState_F0;
        q19_12 bossFightTimer_F4;
        s8     __pad_F8[48];
    };

    /** @brief Larval Stalker character properties. */
    struct s_PropsLarvalStalker
    {
        u16        flags_E8; /** `e_LarvalStalkerFlags` */
        u8         field_EA;
        s8         __pad_EB;
        q19_12     timer_EC;
        q20_12     timer_F0;
        s16        keyframeIdx_F4; // Relative keyframe?
        s16        keyframeIdx_F6; // Relative keyframe?
        q19_12     targetPositionX;
        q19_12     targetPositionZ;
        q3_12      angle_100;
        q3_12      angle_102;
        q19_12     animTime_104;
        q3_12      angle_108;
        q4_12      timer_10A;
        u_Property field_10C;
        VECTOR3    field_110;
        s32        flags_11C;
        u_Property field_120;
        s16        field_124;
        q3_12      moveSpeed;
    };

    /** @brief Puppet Nurse or Puppet Doctor character properties. */
    struct s_PropsPuppetNurse
    {
        VECTOR3       position_E8; /** Q19.12 */
        s_CharaDamage damage;
        q19_12        field_104; // Timer.
        q19_12        field_108; // `vx` position?
        q19_12        field_10C; // `vz` position?
        q19_12        moveSpeed;
        q19_12        field_114; // Damage related.
        u8            field_118; // Used in switch, values 0 - 2.
        u8            modelVariantIdx;
        q3_12         field_11A; // Timer.
        q3_12         field_11C; // Heading angle.
        s16           field_11E; // `bool`
        q3_12         field_120;
        u16           flags_122; /** `e_PuppetNurseFlags` */
        s_800D5710*   field_124;
    };

    /** @brief Romper character properties. */
    struct s_PropsRomper
    {
        s32    flags; /** `e_RomperFlags` */
        q3_12  angle_EC; // Target heading angle?
        s16    field_EE;
        q3_12  field_F0; // Move speed accumulation for this tick.
        q3_12  rotationY_F2;
        q19_12 field_F4; // Relative anim time?
        q3_12  movementOffsetX;
        q3_12  movementOffsetZ;
        q19_12 targetPositionX_FC;
        q19_12 targetPositionZ_100;
        s32    field_104;
        q19_12 positionX_108;
        u8     field_10C; // Relative keyframe index?
        s8     __pad_10D;
        u16    field_10E;
        q19_12 positionZ_110;
        u8     field_114;
        u8     field_115;
        q3_12  field_116;
        q3_12  timer_118;
        u8     field_11A;
        s8     __pad_11B;
        q3_12  timer_11C;
        s8     __pad_11E[2];
        q19_12 distance_120;
        q19_12 field_124; // Move speed step?
    };

    /** @brief Split Head character properties. */
    struct s_PropsSplitHead
    {
        u16     flags; /** `e_SplitHeadFlags` */
        u8      field_EA;
        s8      __pad_EB;
        q3_12   angle_EC;
        q3_12   field_EE;
        q4_12   angle_F0;
        q4_12   timer_F2;
        q4_12   timer_F4;
        s8      __pad_F8[2];
        q19_12  animTime_F8;
        s32     field_FC;
        s32     field_100;
        s8      unk_104[4];
        u8      field_108[4];
        q19_12  field_10C;
        VECTOR3 field_110;
        s32     flags_11C; /** `e_SplitHeadFlags` */
        s8      unk_120[4];
        s16     field_124;
        q3_12   moveSpeed;
    };

    /** @brief Stalker character properties. */
    struct s_PropsStalker
    {
        s16    flags; /** `e_StalkerFlags` */
        s8     __pad_EA[2];
        q3_12  offset_EC;
        q3_12  offset_EE;
        q19_12 targetPositionX;
        q19_12 targetPositionZ;
        q19_12 timer_F8;
        s16    keyframeIdx_FC;    // Or anim status?? Seems to be used as both.
        s16    relKeyframeIdx_FE; // Unsure.
        q3_12  targetHeadingAngle;
        s16    sfxId_102;
        q19_12 relAnimTime_104;
        q4_12  timer_108;
        u8     field_10A;
        s8     __pad_10B;
        q19_12 timer_10C;
        q19_12 health_110;
        q3_12  angle_114;
        q4_12  timer_116;
    };

    /** @brief Twinfeeler character properties. */
    struct s_PropsTwinfeeler
    {
        // TODO: Weird `field_E8` access.
        u_Property    field_E8;
        //q3_12         sfxTimer_E8;
        //q4_12         field_EA;
        s_CharaDamage damage;
        q19_12        digTimer;
        q19_12        spawnPositionX; /** @unused */
        q19_12        spawnPositionZ; /** @unused */
        q19_12        prevMoveSpeed;
        q19_12        accumulatedDamage;
        s16           field_110; /** @unused */
        s8            __pad_112[2];
        u32           flags;     /** `e_TwinfeelerFlags` */
        u16           field_118; /** `bool` */
        s8            __pad_11A[2];
        q19_12        prevHealth;
        s8            __pad_120[8];
    };

    typedef struct
    {
        s16     field_0; // Something dependent on `CharaFlag_Unk8`.
        u8      field_2; // In player: packed weapon attack. See `WEAPON_ATTACK`.
                         // This is not the same as `attackReceived`, as this value only resets when player is aiming.
                         // In NPCs: Indicates attack performed on player.
        u8      field_3;
        u8      field_4;
        s8      __pad_5[3];
        s32     field_8;  // } `bool`? | Fields used by `func_8008A3E0`.
        s16     field_C;  // } Angle?
        s16     field_E;  // } Angle?
        s16     field_10; // }
        s16     field_12; // }
        s32     field_14; // }
        VECTOR3 field_18; // Q19.12 | Set to player attack position.
        VECTOR3 field_24[3];
        VECTOR3 field_48[3];
    } s_SubCharacter_44;

    /** @brief Character collision box. */
    struct s_CharaBox
    {
        q3_12 top;
        q3_12 bottom;
        q3_12 height;
        q3_12 offsetY;
        q3_12 field_8; // X extent?? Always negative, but why? `s_CharaCylinder::radius` is set to this.
        q3_12 field_A; // Z extent?? `s_CharaCylinder::field_2` is set to this.
    };

    /** @brief Character collision cylinder. */
    struct s_CharaCylinder
    {
        q3_12 radius;  // Map geometry collision radius?
        q3_12 field_2; // Character-to-character collision radius? Sometimes used as box extent?
    };

    /** @brief Character shape offsets for `s_CharaBox` and `s_CharaCylinder`. */
    struct s_CharaShapeOffsets
    {
        DVECTOR_XZ box;
        DVECTOR_XZ cylinder;
    };

    /** @brief Character keyframe collision info. */
    struct s_Keyframe
    {
        s_CharaBox          box;
        s_CharaShapeOffsets shapeOffsets;
    };

    /** @brief Character collision info for the active animation frame. */
    struct s_CharaCollision
    {
        s_CharaBox          box;
        s_CharaCylinder     cylinder;
        s_CharaShapeOffsets shapeOffsets;   // Translation data?
        u8                  field_E0;       // Related to collision. If the player collides with the only enemy in memory and the enemy is knocked down, this is set to 1.
        s8                  state      : 4; /** `e_CharaCollisionState` */
        u8                  field_E1_4 : 4; // Index for array of `s_func_8006CF18`.
        s_func_8006CF18*    field_E4;
    };

    /** @brief Character info. */
    struct s_SubCharacter
    {
        s_Model           model;    // In player: Manage the half lower part of Harry's body animations (legs and feet).
        VECTOR3           position; /** Q19.12 */
        SVECTOR3          rotation; /** Q3.12 */
        q3_12             angleToTarget;
        SVECTOR3          rotationSpeed;              /** Q3.12 | Rotation speed for `rotation`. */
        q3_12             angleToTargetRotationSpeed; /** Rotation speed for `angleToTarget`. */
        q19_12            fallSpeed;
        q19_12            moveSpeed;
        q3_12             headingAngle;
        s16               flags;          /** `e_CharaFlags` */
        s8                field_40;       // In player: Index of the NPC attacking the player. Spawn index for Air Screamer?
                                          // In NPCs: Unknown. `Game_NpcRoomInitSpawn` sugests it indicates the NPC index in `s_Savegame::mapEnemyStates`.
        s8                attackReceived; // Packed weapon attack indicating what attack has been performed to the character. See `WEAPON_ATTACK`.
        s_SubCharacter_44 field_44;
        q19_12            health;
        s_CharaDamage     damage;
        u16               deathTimer;     // Part of `shBattleInfo` struct in SH2, may use something similar here.
        q3_12             timer_C6;       // Some sort of timer. Written to by `LarvalStalker_Update`.
        s_CharaCollision  collision;

        s_SubCharacter() { memset(this, 0, sizeof(*this)); }

        union _u
        {
            s_PropsDummy           dummy;
            s_PropsPlayer          player;
            s_PropsNpc             npc;
        
            s_PropsAirScreamer     airScreamer;
            s_PropsBloodsucker     bloodsucker;
            s_PropsCat             cat;
            s_PropsCreeper         creeper;
            s_PropsFloatstinger    floatstinger;
            s_PropsGroaner         groaner;
            s_PropsHangedScratcher hangedScratcher;
            s_PropsIncubus         incubus;
            s_PropsLarvalStalker   larvalStalker;
            s_PropsPuppetNurse     puppetNurse;
            s_PropsRomper          romper;
            s_PropsSplitHead       splitHead;
            s_PropsStalker         stalker;
            s_PropsTwinfeeler      twinfeeler;

            _u() {}
        } properties;
    };

    /** @brief Character file info.
     * Holds file IDs of anim/model/texture for each `e_CharaId` along with some data used in VC camera code.
     */
    struct s_CharaFileInfo
    {
        s16            animFileIdx;
        s16            modelFileIdx;
        s16            textureFileIdx    : 16;
        q8_8           field_6           : 10;
        u16            materialBlendMode : 6; /** `e_BlendMode` */
        s_FsImageDesc* field_8;               // TODO: Extra texture pointer? Usually `NULL` in `CHARA_FILE_INFOS`.
        u16            cameraAnchor  : 2;     /** `e_CameraAnchor` */
        q19_12         cameraOffsetY : 14;
        // 2 bytes of padding.
    };

    /** Array containg file IDs used for each `e_CharaId`, used in `Fs_QueueStartReadAnm`. */
    extern s_CharaFileInfo CHARA_FILE_INFOS[Chara_Count]; // 0x800A90FC

    /** @brief Sets the collision shapes of a character from keyframe collision data.
     *
     * @param chara Character to update.
     * @param keyframe Keyframe collision data.
     */
    static inline void Chara_CollisionSet(s_SubCharacter* chara, const s_Keyframe* keyframe)
    {
        chara->collision.box.top                  = keyframe->box.top;
        chara->collision.box.bottom               = keyframe->box.bottom;
        chara->collision.box.height               = keyframe->box.height;
        chara->collision.box.offsetY              = keyframe->box.offsetY;
        chara->collision.shapeOffsets.cylinder.vx = keyframe->shapeOffsets.cylinder.vx;
        chara->collision.shapeOffsets.cylinder.vz = keyframe->shapeOffsets.cylinder.vz;
        chara->collision.cylinder.radius          = keyframe->box.field_8;
        chara->collision.shapeOffsets.box.vx      = keyframe->shapeOffsets.box.vx;
        chara->collision.shapeOffsets.box.vz      = keyframe->shapeOffsets.box.vz;
        chara->collision.cylinder.field_2         = keyframe->box.field_A;
    }

    /** @brief Checks if the `s_SubCharacter*` has the given `flags` value set. */
    #define Chara_HasFlag(chara, flag) \
        ((chara)->flags & (flag))

    /** @brief Clears a character's properties. TODO: Declare `i` here.
     *
     * @param chara Character to update.
     */
    #define Chara_PropsClear(chara)                             \
        for (i = 0; i < 16; i++)                                \
        {                                                       \
            chara->properties.dummy.properties_E8[i].val32 = 0; \
        }

    /** @brief Clears a character's damage field.
     *
     * @param chara Character to update.
     */
    #define Chara_DamageClear(chara)             \
        (chara)->damage.amount      = Q12(0.0f); \
        (chara)->damage.position.vz = Q12(0.0f); \
        (chara)->damage.position.vy = Q12(0.0f); \
        (chara)->damage.position.vx = Q12(0.0f)

    /** @brief Sets a character's received attack type.
     *
     * TODO: Could make this use `WEAPON_ATTACK` macro and take the values needed for that instead.
     * Maybe devs did similar thing, which is why this separate macro/inline is needed for a match?
     *
     * @param chara Character to update.
     * @param attack Attack type to set.
     */
    #define Chara_AttackReceivedSet(chara, attack) \
        (chara)->attackReceived = (attack)

    /** @brief Gets a character's received attack type.
     *
     * @param chara Character to update.
     */
    #define Chara_AttackReceivedGet(chara) \
        (chara)->attackReceived

    /** @brief Updates an NPC's animation if the `freeze` flag isn't currently set.
     *
     * @note This is only for NPCs which use the `s_PropsNpc` part of the `s_SubCharacter::properties` union.
     *
     * @param chara Character to update.
     * @param anmHdr ANM file header.
     * @param boneCoords Character model bone coords.
     * @param animInfos Character animation infos.
     */
    #define Chara_AnimUpdate(chara, anmHdr, boneCoords, animInfos)                     \
    {                                                                                  \
        if (!(chara)->properties.npc.freeze)                                           \
        {                                                                              \
            s_AnimInfo* __animInfo = &(animInfos)[(chara)->model.anim.status];         \
                                                                                       \
            __animInfo->playbackFunc(&(chara)->model, anmHdr, boneCoords, __animInfo); \
        }                                                                              \
    }

    /** @brief Sets the animation of a character.
     *
     * @param chara Character to set animation for. TODO: Maybe should take `s_ModelAnim` instead? If fits better, also rename to `Anim_Set`.
     * @param animStatus Packed anim status. See `s_ModelAnim::status`.
     * @param keyframeIdx Active keyframe index.
     */
    static inline void Chara_AnimSet(s_SubCharacter* chara, s32 animStatus, s32 keyframeIdx)
    {
        chara->model.anim.status      = animStatus;
        chara->model.anim.time        = Q12(keyframeIdx);
        chara->model.anim.keyframeIdx = keyframeIdx;
    }

    /** @brief Resets an NPC's control state to `*_None` if the control subsystem was flagged for a reset.
     *
     * @note This is only for NPCs which use the `s_PropsNpc` part of the properties union.
     *
     * @param chara Character to update.
     */
    static inline void Chara_AnimStateReset(s_SubCharacter* chara)
    {
        if (chara->properties.npc.resetControlState)
        {
            chara->properties.npc.controlState      = 0;
            chara->model.stateStep                  = 0;
            chara->properties.npc.resetControlState = false;
        }
    }
}
