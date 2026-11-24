#pragma once

#include "Game/Bodyprog/Chara/Collision.h"
#include "Game/Bodyprog/Chara/Combat.h"
#include "Game/Bodyprog/Chara/Model.h"
#include "Game/Bodyprog/Chara/Properties.h"
#include "Game/Main/FsQueue.h"

namespace Silent::Assets{ struct Asset; }

using namespace Silent::Assets;

namespace Silent::Game
{
    struct s_AnimInfo;
    struct s_CharaKeyframe;

    constexpr int  NPC_COUNT_MAX        = 6;
    constexpr int  NPC_BONE_COUNT_MAX   = 10 * NPC_COUNT_MAX;
    constexpr int  CHARA_GROUP_COUNT    = 4;                  /** While up to 6 NPCs and a player can exist in the game world, only 4 different character
                                                               * types (including the player) can be loaded at a time. */
    constexpr int  CHARA_FORCE_FREE_ALL = 0xFF;               /** `Chara_Load` can force free already loaded models to make room for new ones. */

    /** @brief Returns the `s_SubCharacter` instance for the player character.
     *
     * @note Kept as function-style macro to prevent macro from replacing anything named the same, if we decide to rename these.
     *       e.g. if we had `#define playerChara ...`, that would break things that use `s_SubCharacter* playerChara`
     *       but `#define playerChara() ...` would only replace function-style uses.
    */
    #define PLAYER_CHARA() (g_SysWork.playerWork.player)

    /** @brief Returns the player character properties. */
    #define PLAYER_PROPS() (g_SysWork.playerWork.player.properties.player)

    /** @brief Returns the player character extra data. */
    #define PLAYER_EXTRA() (g_SysWork.playerWork.extra)

    /** @brief Returns the player combat data. */
    #define PLAYER_COMBAT() (g_SysWork.playerCombat)

    /** @brief Returns the `s_SubCharacter` instance for a given NPC index. */
    #define NPC_CHARA(index) (g_SysWork.npcs[index])

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
        Chara_MonsterIncubator = 23,
        Chara_MonsterCybil     = 24,
        CHARA_LAST_ENEMY_ID    = Chara_MonsterCybil,

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

        Chara_Count,

        // @todo Implement this in user options.
        /** Grey Child enemy is replaced by the Mumbler in non-USA releases. */
        Chara_GreyChildOrMumbler = Chara_GreyChild,
        //Chara_GreyChildOrMumbler = Chara_Mumbler,
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
        int               flags;          /** `e_CharaFlags` */
        int               field_40;       // In player: Index of the NPC attacking the player. Spawn index for Air Screamer?
                                          // In NPCs: Unknown. `Game_NpcRoomInitSpawn` sugests it indicates the NPC index in `s_Savegame::mapEnemyStates`.
        int               attackReceived; // Packed weapon attack indicating what attack has been performed to the character. See `WEAPON_ATTACK`.
        s_CharaAttack     attack;
        q19_12            health;
        s_CharaDamage     damage;
        int               deathTimer;     // Part of `shBattleInfo` struct in SH2, may use something similar here.
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

    /** @brief Checks if a character has given flags set.
     *
     * @param chara Character to check.
     * @param flags Flags to check (`e_CharaFlags`).
     * @return `true` if the flags are set, `false` otherwise.
     */
    bool Chara_HasFlag(const s_SubCharacter* chara, int flags);

    /** @brief Clears a character's properties.
     *
     * @param chara Character to update.
     */
    void Chara_PropsClear(s_SubCharacter* chara);

    /** @brief Clears a character's damage parameters.
     *
     * @param chara Character to update.
     */
    void Chara_DamageClear(s_SubCharacter* chara);

    /** @brief Sets a character's received attack type.
     *
     * TODO: Could make this use `WEAPON_ATTACK` macro and take the values needed for that instead.
     * Maybe devs did similar thing, which is why this separate macro/inline is needed for a match?
     *
     * @param chara Character to update.
     * @param attack Packed weapona attack to set. See `WEAPON_ATTACK`.
     */
    void Chara_AttackReceivedSet(s_SubCharacter* chara, int attack);

    /** @brief Gets a character's received attack type.
     *
     * @param chara Character to update.
     */
    int Chara_AttackReceivedGet(s_SubCharacter* chara);

    /** @brief Updates an NPC's animation if the `freeze` flag isn't currently set.
     *
     * @note This is only for NPCs which use the `s_PropsNpc` part of the `s_SubCharacter::properties` union.
     *
     * @param chara Character to update.
     * @param anmAsset ANM asset.
     * @param animInfos Character animation infos.
     */
    void Chara_AnimUpdate(s_SubCharacter* chara, const Asset& anmAsset, const s_AnimInfo* animInfos);

    /** @brief Sets a character's animation.
     *
     * @param chara Character to set the animation for.
     * @param animStatus Packed anim status. See `s_ModelAnim::status`.
     * @param keyframeIdx Active keyframe index.
     */
    void Chara_AnimSet(s_SubCharacter* chara, int animStatus, int keyframeIdx);

    /** @brief Resets an NPC's control state to `*_None` if the control subsystem was flagged for a reset.
     *
     * @note This is only for NPCs which use the `s_PropsNpc` part of the properties union.
     *
     * @param chara Character to update.
     */
    void Chara_AnimStateReset(s_SubCharacter* chara);

    /** @brief Sets the collision shapes of a character from keyframe collision data.
     *
     * @param chara Character to update.
     * @param keyframe Keyframe collision data.
     */
    void Chara_CollisionSet(s_SubCharacter* chara, const s_CharaKeyframe* keyframe);

    /** @brief Reduces a character's move speed toward a target speed without going below it.
     *
     * @param chara Character to update.
     * @param speed Speed change per second.
     * @param limit Lowest move speed.
     */
    void Chara_MoveSpeedDecrease(s_SubCharacter* chara, q19_12 speed, q19_12 limit);

    /** @brief Moves a character's move speed toward a target speed without overshooting.
     *
     * @param chara Character to update.
     * @param speed Speed change per second.
     * @param limit Target move speed.
     */
    void Chara_MoveSpeedUpdate(s_SubCharacter* chara, q19_12 speed, q19_12 limit);
}
