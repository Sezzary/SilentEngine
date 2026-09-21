#include "Framework.h"
#include "Psx.h"
#include "GameSysStates.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Assets/TranslationKeys.h"
#include "Game/Bodyprog/Demo.h"
#include "Game/Bodyprog/Events/bodyprog_data_800A99B4.h"
#include "Game/Bodyprog/Events/MapMsg.h"
#include "Game/Bodyprog/Events/EventsMain.h"
#include "Game/Bodyprog/Events/Radio.h"
#include "Game/Bodyprog/Events/Utils.h"
#include "Game/Bodyprog/GameBoot/GameBoot.h"
//#include "Game/Bodyprog/item_screens.h"
//#include "Game/Bodyprog/memcard.h"
#include "Game/Bodyprog/Savegame.h"
#include "Game/Bodyprog/Screen/BackgroundDraw.h"
#include "Game/Bodyprog/Screen/CutsceneBorder.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Bodyprog/Text/TextDraw.h"
#include "Game/Bodyprog/Title.h"
//#include "Game/Bodyprog/player.h"
//#include "Game/Bodyprog/ranking.h"
#include "Game/Bodyprog/Sound/SoundSystem.h"
#include "Game/Main/FsQueue.h"
#include "Game/Main/Rng.h"
#include "Game/Screens/Stream/Stream.h"
#include "Input/Input.h"
#include "Services/Filesystem.h"
#include "Utils/Translator.h"
#include "Utils/Bitfield.h"
#include "Utils/Utils.h"

using namespace Silent::Assets;
using namespace Silent::Input;
using namespace Silent::Services;
using namespace Silent::Utils;

namespace Silent::Game
{
    s_MapOverlayHdr g_MapOverlayHdr; // } @todo Defined elsewhere in decomp.

    // @todo Remove checklist when all are working.
    static void (*g_SysStateFuncs[])() =
    {
        SysState_Gameplay_Update,       // @todo
        SysState_OptionsMenu_Update,    // @todo
        SysState_StatusMenu_Update,     // @todo
        SysState_MapScreen_Update,      // @todo
        SysState_Fmv_Update,            // @todo
        SysState_LoadArea_Update,       // @todo
        SysState_LoadArea_Update,       // @todo
        SysState_ReadMessage_Update,    // @todo
        SysState_SaveMenu_Update,       // @todo
        SysState_SaveMenu_Update,       // @todo
        SysState_EventCallback_Update,  // @todo
        SysState_EventSetFlag_Update,   // @todo
        SysState_EventPlaySound_Update, // @todo
        SysState_GameOver_Update,       // @todo
        SysState_GamePaused_Update      // @todo
    };

    /** Used to store the previous delta time state of the delta timer. There are some instances where 2D backgrounds
     * are drawn using `g_DeltaTimeRaw` while `g_DeltaTime` is stopped.
     */
    static q19_12 g_DeltaTimeCpy;

    s_EventData*   g_ItemTriggerEvents[5]; // Guessed size. Check decomp later.
    s_RadioNpcInfo g_RadioNpcInfos[2];
    s_MapPoint2d   D_800BCDB0;
    s32            g_ItemTriggerItemIds[5];
    u8             D_800BCDD4;
    s_EventData*   g_MapEventData;

    void GameState_InGame_Update() // 0x80038BD4
    {
        s_SubCharacter* player;

        Demo_DemoRandSeedBackup();

        switch (g_GameWork.gameStateSteps[0])
        {
            case 0:
                ScreenFade_Start(true, true, false);
                g_ScreenFadeTimestep         = Q12(3.0f);
                g_GameWork.gameStateSteps[0] = 1;

            case 1:
                func_80037154();
                //Savegame_MapRoomIdxUpdate();
                //func_800892A4(1);

                g_IntervalVBlanks = 2;
                g_GameWork.gameStateSteps[0]++;
                g_SysWork.bgmStatusFlags |= BgmStatusFlag_6;
                break;
        }

        if (g_SysWork.sysState != SysState_Gameplay && g_SysWork.playerWork.player.health <= Q12(0.0f))
        {
            SysWork_StateSetNext(SysState_Gameplay);
        }

        if (g_DeltaTime != Q12(0.0f))
        {
            g_DeltaTimeCpy = g_DeltaTime;
        }
        else
        {
            g_DeltaTimeCpy = g_DeltaTimeRaw;
        }

        if (g_SysWork.sysState == SysState_Gameplay)
        {
            g_SysWork.isMgsStringSet = false;
            g_SysStateFuncs[SysState_Gameplay]();
        }
        else
        {
            g_DeltaTime = Q12(0.0f);
            g_SysStateFuncs[g_SysWork.sysState]();

            if (g_SysWork.sysState == SysState_Gameplay)
            {
                Event_Update(true);

                if (g_MapEventSysState != SysState_Invalid)
                {
                    SysWork_StateSetNext((e_SysState)g_MapEventSysState);
                }
            }
        }
        Demo_DemoRandSeedRestore();

        g_IsLoadingFinished = ScreenFade_IsFinished() && Fs_QueueChunksLoad();

        if (!(g_SysWork.bgmStatusFlags & BgmStatusFlag_Pause) && g_MapOverlayHdr.updateWorldObjects != nullptr)
        {
            g_MapOverlayHdr.updateWorldObjects();
        }

        Screen_CutsceneCameraStateUpdate();
        //Bgm_TrackUpdate(false);
        Demo_DemoRandSeedRestore();
        Demo_DemoRandSeedRestore();

        if (!(g_SysWork.bgmStatusFlags & BgmStatusFlag_Pause))
        {
            //func_80040014();
            vcMoveAndSetCamera(false, false, false, false, false, false, false, false);

            if (g_MapOverlayHdr.func_44 != nullptr)
            {
                g_MapOverlayHdr.func_44();
            }

            Demo_DemoRandSeedRestore();

            player = &g_SysWork.playerWork.player;
            //Player_Update(player, FS_BUFFER_0, g_SysWork.playerBoneCoords);

            Demo_DemoRandSeedRestore();
            //Gfx_FlashlightUpdate();

            if (g_SavegamePtr->mapIdx != MapIdx_MAP7_S03)
            {
                g_MapOverlayHdr.particlesUpdate(0, g_SavegamePtr->mapIdx, 1);
            }

            Demo_DemoRandSeedRestore();

            if (player->model.anim.flags & AnimFlag_Visible)
            {
                //func_8003DA9C(Chara_Harry, g_SysWork.playerBoneCoords, 1, g_SysWork.playerWork.player.timer_C6, 0);
                //Chara_Flag8Clear(&g_SysWork.playerWork.player);
                //Player_CombatUpdate(&g_SysWork.playerWork, g_SysWork.playerBoneCoords);
                //func_8008A3AC(&g_SysWork.playerWork.player);
            }

            Demo_DemoRandSeedRestore();
            //Game_NpcRoomInitSpawn(true);
            //Game_NpcUpdate();
            //func_8005E89C();
            //Ipd_CloseRangeChunksInit();
            //Gfx_InGameDraw(1);
            Demo_DemoRandSeedAdvance();
        }
    }

    void SysState_Gameplay_Update() // 0x80038BD4
    {
        const auto& input = g_App.GetInput();

        auto* player = &g_SysWork.playerWork.player;

        Event_Update(player->attackReceived != NO_VALUE);
        //Savegame_MapRoomIdxUpdate();

        switch (FP_ROUND_SCALED(player->health, 10, Q12_SHIFT))
        {
            case 0:
                //func_800892A4(17);
                break;

            case 1:
            case 2:
                //func_800892A4(16);
                break;

            case 3:
                //func_800892A4(15);
                break;

            case 4:
                //func_800892A4(14);
                break;

            case 5:
                //func_800892A4(13);
                break;

            case 6:
                //func_800892A4(12);
                break;
        }

        if (g_SysWork.playerWork.player.health <= Q12(0.0f))
        {
            return;
        }

        if (input.GetAction(In::Light).IsClicked() &&
            g_SysWork.field_2388.field_154.effectsInfo.field_0.s_field_0.field_0 & (1 << 1))
        {
            //Game_FlashlightToggle();
        }

        if (g_MapEventSysState != SysState_Invalid)
        {
            SysWork_StateSetNext((e_SysState)g_MapEventSysState);
        }
        else if (input.GetAction(In::Pause).IsClicked())
        {
            SysWork_StateSetNext(SysState_GamePaused);
        }
        /*else if (Player_IsAttacking() == true)
        {
            return;
        }*/
        else if (input.GetAction(In::Item).IsClicked())
        {
            SysWork_StateSetNext(SysState_StatusMenu);
        }
        else if (input.GetAction(In::Map).IsClicked())
        {
            SysWork_StateSetNext(SysState_MapScreen);
            g_SysWork.isMgsStringSet = false;
        }
        else if (input.GetAction(In::Option).IsClicked())
        {
            SysWork_StateSetNext(SysState_OptionsMenu);
        }

        if (g_SysWork.sysState == SysState_OptionsMenu ||
            g_SysWork.sysState == SysState_StatusMenu ||
            g_SysWork.sysState == SysState_MapScreen)
        {
            g_SysWork.sysState |= SysFlag_MenuActive;
        }
        else if (ScreenFade_IsNone())
        {
            g_SysWork.sysState &= ~SysFlag_MenuActive;
        }
    }

    void SysState_GamePaused_Update() // 0x800391E8
    {
        const auto& input      = g_App.GetInput();
        const auto& translator = g_App.GetTranslator();

        static s32 D_800A9A68 = 0;

        D_800A9A68 += g_DeltaTimeRaw;
        if (!((D_800A9A68 >> 11) & (1 << 0)))
        {
            Gfx_StringPositionSet(SCREEN_WIDTH / 2, 104);
            Gfx_StringColorSet(StringColorId_White);
            Gfx_StringDraw(translator(KEY_PAUSE_MENU_HEADING));
        }

        //func_80091380();
        //Game_TimerUpdate();

        if (g_SysWork.sysStateSteps[0] == 0)
        {
            SD_Call(3);
            g_SysWork.sysStateSteps[0]++;
        }

        // Debug button combo to bring up save screen from pause screen.
        if (input.GetAction(In::Left).IsHeld()     &&
            input.GetAction(In::StepLeft).IsHeld() &&
            input.GetAction(In::Action).IsHeld()   &&
            input.GetAction(In::Enter).IsHeld())
        {
            D_800A9A68 = 0;
            SD_Call(4);
            g_MapEventParam = 0;
            SysWork_StateSetNext(SysState_SaveMenu1);
            return;
        }

        if (input.GetAction(In::Pause).IsClicked())
        {
            D_800A9A68 = 0;

            SD_Call(4);
            SysWork_StateSetNext(SysState_Gameplay);
        }
    }

    void SysState_OptionsMenu_Update() // 0x80039344
    {
        switch (g_SysWork.sysStateSteps[0])
        {
            case 0:
                ScreenFade_Start(true, false, false);
                g_ScreenFadeTimestep        = Q12(0.0f);
                g_SysWork.sysStateSteps[0] = 1;

            case 1:
                //if (Ipd_ChunkInitCheck() != 0)
                {
                    SD_Call(19);
                    //GameFs_OptionBinLoad();

                    g_SysWork.sysStateSteps[0]++;
                }
                break;
        }

        if (g_IsLoadingFinished)
        {
            Game_StateSetNext(GameState_OptionScreen);
        }
    }

    void func_8003943C() // 0x8003943C
    {
        s32 roundedVal0;
        s32 roundedVal1;
        s32 val0;
        s32 val1;

        bool isRockDrillAttack = g_SysWork.playerCombat.weaponAttack == WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap);

        //func_8008B3E4(0);

        if (g_SysWork.field_275C > Q12(256.0f))
        {
            val0        = g_SysWork.field_275C - Q12(256.0f);
            roundedVal0 = FP_ROUND_TO_ZERO(val0, Q12_SHIFT);
            //func_8008B438(!isRockDrillAttack, roundedVal0, 0);

            if (isRockDrillAttack)
            {
                val1        = g_SysWork.field_2764 - Q12(256.0f);
                roundedVal1 = FP_ROUND_TO_ZERO(val1, Q12_SHIFT);
                //func_8008B40C(roundedVal1, 0);
            }
        }
        else
        {
            //func_8008B438(!isRockDrillAttack, 0, 0);

            if (isRockDrillAttack)
            {
                //func_8008B40C(0, 0);
            }
        }

        switch (g_SavegamePtr->mapIdx)
        {
            case MapIdx_MAP0_S01:
            case MapIdx_MAP0_S02:
            case MapIdx_MAP1_S00:
            case MapIdx_MAP1_S01:
            case MapIdx_MAP1_S02:
            case MapIdx_MAP1_S03:
            case MapIdx_MAP1_S04:
            case MapIdx_MAP1_S05:
            case MapIdx_MAP1_S06:
            case MapIdx_MAP2_S00:
            case MapIdx_MAP2_S01:
            case MapIdx_MAP2_S02:
            case MapIdx_MAP2_S03:
            case MapIdx_MAP2_S04:
            case MapIdx_MAP3_S00:
            case MapIdx_MAP3_S01:
            case MapIdx_MAP3_S02:
            case MapIdx_MAP3_S04:
            case MapIdx_MAP3_S05:
            case MapIdx_MAP3_S06:
            case MapIdx_MAP4_S00:
            case MapIdx_MAP4_S01:
            case MapIdx_MAP4_S02:
            case MapIdx_MAP4_S03:
            case MapIdx_MAP4_S04:
            case MapIdx_MAP4_S05:
            case MapIdx_MAP4_S06:
            case MapIdx_MAP5_S00:
            case MapIdx_MAP5_S01:
            case MapIdx_MAP5_S02:
            case MapIdx_MAP5_S03:
            case MapIdx_MAP6_S00:
            case MapIdx_MAP6_S01:
            case MapIdx_MAP6_S02:
            case MapIdx_MAP6_S03:
            case MapIdx_MAP6_S04:
            case MapIdx_MAP6_S05:
            case MapIdx_MAP7_S00:
            case MapIdx_MAP7_S01:
            case MapIdx_MAP7_S02:
                break;

            case MapIdx_MAP3_S03:
                Sd_SfxStop(Sfx_Unk1525);
                Sd_SfxStop(Sfx_Unk1527);
                break;

            case MapIdx_MAP0_S00:
                Sd_SfxStop(Sfx_Unk1358);
                break;
        }
    }

    void SysState_StatusMenu_Update() // 0x80039568
    {
        Game_StateSetNext(GameState_LoadStatusScreen);
    }

    void GameState_LoadStatusScreen_Update() // 0x800395C0
    {
        s_Savegame* save;

        if (g_GameWork.gameStateSteps[0] == 0)
        {
            g_IntervalVBlanks = 1;
            ScreenFade_Reset();

            func_8003943C();

            //if (Sd_AudioStreamingCheck())
            //{
            //    SD_Call(19);
            //}

            save = g_SavegamePtr;
            //func_800540A4(save->mapIdx);
            //GameFs_MapItemsTextureLoad(save->mapIdx);

            g_GameWork.gameStateSteps[0]++;
        }

        //Screen_BackgroundMotionBlur(SyncMode_Wait2);

        if (Fs_QueueChunksLoad())
        {
            Game_StateSetNext(GameState_InventoryScreen);
        }
    }

    void SysState_MapScreen_Update() // 0x800396D4
    {
        const auto& input = g_App.GetInput();

        if (!HAS_PAPER_MAP(g_SavegamePtr->paperMapIdx))
        {
            if (input.GetAction(In::Map).IsClicked() ||
                Gfx_MapMsg_Draw(MapMsgIdx_NoMap) > MapMsgState_Idle)
            {
                SysWork_StateSetNext(SysState_Gameplay);
            }
        }
        else if ((g_SysWork.field_2388.field_154.effectsInfo.field_0.s_field_0.field_0 & (1 << 1)) && !g_SysWork.field_2388.isFlashlightOn &&
                ((g_SysWork.field_2388.field_1C[0].effectsInfo.field_0.s_field_0.field_0 & (1 << 0)) ||
                (g_SysWork.field_2388.field_1C[1].effectsInfo.field_0.s_field_0.field_0 & (1 << 0))))
        {
            if (input.GetAction(In::Map).IsClicked() ||
                Gfx_MapMsg_Draw(MapMsgIdx_TooDarkForMap) > MapMsgState_Idle)
            {
                SysWork_StateSetNext(SysState_Gameplay);
            }
        }
        else
        {
            if (g_SysWork.sysStateSteps[0] == 0)
            {
                if (g_PaperMapMarkingFileIdxs[g_SavegamePtr->paperMapIdx] != NO_VALUE)
                {
                    Fs_QueueStartReadTim((e_FsFile)((int)FILE_TIM_MR_0TOWN_TIM + g_PaperMapMarkingFileIdxs[g_SavegamePtr->paperMapIdx]), FS_BUFFER_1, &g_PaperMapMarkingAtlasImg);
                }

                Fs_QueueStartSeek((e_FsFile)((int)FILE_TIM_MP_0TOWN_TIM + g_PaperMapFileIdxs[g_SavegamePtr->paperMapIdx]));

                ScreenFade_Start(true, false, false);
                g_ScreenFadeTimestep = Q12(0.0f);
                g_SysWork.sysStateSteps[0]++;
            }

            if (g_IsLoadingFinished)
            {
                Game_StateSetNext(GameState_PaperMapScreen);
            }
        }
    }

    void GameState_LoadMapScreen_Update() // 0x8003991C
    {
        if (g_GameWork.gameStateSteps[0] == 0)
        {
            g_IntervalVBlanks = 1;

            func_8003943C();
            //func_80066E40();

            if (g_PaperMapMarkingFileIdxs[g_SavegamePtr->paperMapIdx] != NO_VALUE)
            {
                Fs_QueueStartReadTim((e_FsFile)((int)FILE_TIM_MR_0TOWN_TIM + g_PaperMapMarkingFileIdxs[g_SavegamePtr->paperMapIdx]), FS_BUFFER_1, &g_PaperMapMarkingAtlasImg);
            }

            Fs_QueueStartReadTim((e_FsFile)((int)FILE_TIM_MP_0TOWN_TIM + g_PaperMapFileIdxs[g_SavegamePtr->paperMapIdx]), FS_BUFFER_2, &g_PaperMapImg);
            g_GameWork.gameStateSteps[0]++;
        }

        //Screen_BackgroundMotionBlur(SyncMode_Wait2);

        if (Fs_QueueChunksLoad())
        {
            Game_StateSetNext(GameState_PaperMapScreen);
        }
    }

    void SysState_Fmv_Update()
    {
        constexpr auto BASE_AUDIO_FILE_IDX = FILE_XA_ZC_14392;

        const auto& assets = g_App.GetAssets();
        const auto& fs     = g_App.GetFilesystem();

        static auto videoName = std::string();
        
        // Handle FMV state step.
        static int  fmvStateStep = 0;
        switch (fmvStateStep)
        {
            case 0:
            {
                switch (g_SysWork.sysStateSteps[0])
                {
                    case 0:
                        ScreenFade_Start(false, false, false);
                        g_IsLoadingFinished        = false;
                        g_SysWork.sysStateSteps[0] = 1;

                    case 1:
                        /*if (Ipd_ChunkInitCheck() != 0)
                        {
                            g_SysWork.sysStateSteps[0]++;
                        }*/
                        break;
                }

                if (!g_IsLoadingFinished)
                {
                    break;
                }

                //func_800892A4(0);
                //func_80089128();

                fmvStateStep++;
                break;
            }
            case 1:
            {
                // Collect files sorted alphabetically.
                auto videosPath = fs.GetAssetsDirectory() / ASSETS_VIDEO_DIR_NAME;
                auto videoFiles = std::vector<stdfs::path>{};
                for (auto& entry : stdfs::recursive_directory_iterator(videosPath))
                {
                    if (entry.is_regular_file())
                    {
                        videoFiles.push_back(entry.path().generic_string());
                    }
                }
                Sort(videoFiles);

                // Set FMV video name.
                videoName = videoFiles[assets.GetIdx("Psx/XA/ZC_14392.STR") - g_MapEventParam].filename();

                fmvStateStep++;
                break;
            }
            case 2:
            {
                if (!PlayFmv(videoName))
                {
                    Game_StateSetNext(GameState_MainMenu);
                    g_ScreenFadeTimestep = Q12(1.0f);

                    fmvStateStep++;
                }
                break;
            }
            case 3:
            {
                //func_800892A4(1);

                // Set savegame flag based on `g_MapEventData->completeEventFlag` flag ID.
                Savegame_EventFlagSetAlt(g_MapEventData->completeEventFlag);

                // Return to game.
                Game_StateSetNext(GameState_InGame);

                // If flag is set, returns to `GameState_InGame` with `gameStateStep[0]` = 1.
                if (g_MapEventData->transitionFlags & AreaTransitionFlag_SkipFadeIn)
                {
                    g_GameWork.gameStateSteps[0] = 1;
                }

                fmvStateStep = 0;
                videoName    = {};
                break;
            }
        }
    }

    void SysState_LoadArea_Update() // 0x80039C40
    {
        u32           offsetZ;
        s_MapPoint2d* mapPoint;

        g_SysWork.unused_229C         = 0;
        g_SysWork.loadingScreenIdx    = D_800BCDB0.loadingScreenId;
        g_SysWork.sfxPairIdx          = g_MapEventData->sfxPairIdx_8_19;
        g_SysWork.areaTransitionFlags = g_MapEventData->transitionFlags;

        SD_Call(SFX_PAIRS[g_SysWork.sfxPairIdx].sfx_0);

        if (g_SysWork.sfxPairIdx == SfxPairIdx_7)
        {
            D_800BCDD4          = 0;
            g_SysWork.sysState |= SysFlag_LoadActive;
        }

        D_800BCDB0 = g_MapOverlayHdr.mapPoints[g_MapEventData->eventParam];

        if (D_800BCDB0.triggerParam1 == 1)
        {
            mapPoint              = &g_MapOverlayHdr.mapPoints[g_MapEventData->mapPointIdx];
            offsetZ               = g_SysWork.playerWork.player.position.vz - mapPoint->positionZ;
            D_800BCDB0.positionX += g_SysWork.playerWork.player.position.vx - mapPoint->positionX;
            D_800BCDB0.positionZ += offsetZ;
        }

        if (g_SysWork.sysState == SysState_LoadOverlay)
        {
            g_SysWork.processFlags = ProcessFlag_OverlayTransition;
            g_SavegamePtr->mapIdx  = g_MapEventData->mapIdx;
            GameBoot_MapLoad(g_SavegamePtr->mapIdx);
        }
        else
        {
            g_SysWork.processFlags = ProcessFlag_RoomTransition;
            //Bgm_TrackChange(g_MapEventData->mapIdx);

            if (g_MapOverlayHdr.mapPoints[g_MapEventData->eventParam].field_4_5 != 0)
            {
                g_SysWork.field_2349 = g_MapOverlayHdr.mapPoints[g_MapEventData->eventParam].field_4_5 - 1;
            }
        }

        Savegame_EventFlagSetAlt(g_MapEventData->completeEventFlag);

        if (g_MapEventData->field_8_24)
        {
            g_SysWork.sysState |= SysFlag_OnCameraRail;
        }
        else
        {
            g_SysWork.sysState &= ~SysFlag_OnCameraRail;
        }

        g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;
        Game_StateSetNext(GameState_MainLoadScreen);
        //Screen_BackgroundMotionBlur(SyncMode_Immediate);
    }

    void AreaLoad_UpdatePlayerPosition() // 0x80039F30
    {
        //Chara_PositionSet(&D_800BCDB0);
    }

    void AreaLoad_TransitionSound() // 0x80039F54
    {
        SD_Call(SFX_PAIRS[g_SysWork.sfxPairIdx].sfx_2);
    }

    s8 AreaLoad_TransitionFlags() // 0x80039F90
    {
        if (g_SysWork.processFlags & (ProcessFlag_RoomTransition | ProcessFlag_OverlayTransition))
        {
            return g_SysWork.areaTransitionFlags;
        }

        return 0;
    }

    void SysState_ReadMessage_Update() // 0x80039FB8
    {
        // When `SysState_ReadMessage_Update` is called, the game world freezes.
        // The following conditions unfreeze:
        // - A specific event related flag is disabled.
        // - A specific camera related flag is disabled.
        // - All enemies are dead.
        if (!(g_MapEventData->transitionFlags & AreaTransitionFlag_UnfreezeWorld) &&
            !(g_SysWork.sysState & SysFlag_5))
        {
            for (int i = 0; i < ARRAY_SIZE(g_SysWork.npcs); i++)
            {
                if (g_SysWork.npcs[i].model.charaId >= Chara_Harry        &&
                    g_SysWork.npcs[i].model.charaId <= Chara_MonsterCybil &&
                    g_SysWork.npcs[i].health > Q12(0.0f))
                {
                    if (i == ARRAY_SIZE(g_SysWork.npcs))
                    {
                        g_DeltaTime = g_DeltaTimeCpy;
                    }
                    break;
                }
            }
        }
        else
        {
            g_DeltaTime = g_DeltaTimeCpy;
        }

        if (!g_SysWork.isMgsStringSet)
        {
            g_MapOverlayHdr.playerControlFreeze();
        }

        switch (Gfx_MapMsg_Draw(g_MapEventParam))
        {
            case MapMsgState_Finish:
                break;

            case MapMsgState_Idle:
                break;

            case MapMsgState_SelectEntry1:
                Savegame_EventFlagSetAlt(g_MapEventData->completeEventFlag);
                SysWork_StateSetNext(SysState_Gameplay);
                g_MapOverlayHdr.playerControlUnfreeze(false);
                break;
        }
    }

    void SysWork_SavegameUpdatePlayer() // 0x8003A120
    {
        s_Savegame* save;

        save = g_SavegamePtr;

        save->locationId      = g_MapEventParam;
        save->playerPositionX = g_SysWork.playerWork.player.position.vx;
        save->playerPositionZ = g_SysWork.playerWork.player.position.vz;
        save->playerRotationY = g_SysWork.playerWork.player.rotation.vy;
        save->playerHealth    = g_SysWork.playerWork.player.health;
    }

    void func_8003A16C() // 0x8003A16C
    {
        if (!(g_SysWork.sysState & SysFlag_DemoActive))
        {
            // Update `savegame` with player info.
            SysWork_SavegameUpdatePlayer();

            g_GameWork.autosave = g_GameWork.savegame;
        }
    }

    void SysWork_SavegameReadPlayer() // 0x8003A1F4
    {
        g_SysWork.playerWork.player.position.vx = g_SavegamePtr->playerPositionX;
        g_SysWork.playerWork.player.position.vz = g_SavegamePtr->playerPositionZ;
        g_SysWork.playerWork.player.rotation.vy = g_SavegamePtr->playerRotationY;
        g_SysWork.playerWork.player.health      = g_SavegamePtr->playerHealth;
    }

    void SysState_SaveMenu_Update() // 0x8003A230
    {
        //MemCard_ElementsUpdate();

        switch (g_SysWork.sysStateSteps[0])
        {
            case 0:
                SysWork_SavegameUpdatePlayer();

                if (Savegame_EventFlagGet(EventFlag_SeenSaveScreen) /*||
                    g_SavegamePtr->locationId == SaveLocationId_NextFear || g_MapEventParam == 0*/)
                {
                    //GameFs_SaveLoadBinLoad();

                    ScreenFade_Start(true, false, false);
                    SysWork_StateStepIncrement(0);
                }
                /*else if (Gfx_MapMsg_Draw(MapMsgIdx_SaveGame) == MapMsgState_SelectEntry1)
                {
                    Savegame_EventFlagSet(EventFlag_SeenSaveScreen);

                    //GameFs_SaveLoadBinLoad();

                    ScreenFade_Start(true, false, false);
                    SysWork_StateStepIncrement(0);
                }*/
                break;

            case 1:
                if (g_IsLoadingFinished)
                {
                    ScreenFade_Start(true, true, false);
                    func_8003943C();
                    Game_StateSetNext(GameState_SaveScreen);
                }
                break;
        }
    }

    void SysState_EventCallback_Update() // 0x8003A3C8
    {
        if (g_MapEventData->transitionFlags != AreaTransitionFlag_None)
        {
            Savegame_EventFlagSetAlt(g_MapEventData->completeEventFlag);
        }

        g_DeltaTime = g_DeltaTimeCpy;
        g_MapOverlayHdr.mapEventFuncs[g_MapEventParam]();
    }

    void SysState_EventSetFlag_Update() // 0x8003A460
    {
        g_DeltaTime = g_DeltaTimeCpy;
        Savegame_EventFlagSetAlt(g_MapEventData->completeEventFlag);
        g_SysWork.sysState = SysState_Gameplay;
    }

    void SysState_EventPlaySound_Update() // 0x8003A4B4
    {
        g_DeltaTime = g_DeltaTimeCpy;

        SD_Call(((u16)g_MapEventParam + Sfx_Base) & 0xFFFF);

        Savegame_EventFlagSetAlt(g_MapEventData->completeEventFlag);
        g_SysWork.sysState = SysState_Gameplay;
    }

    void SysState_GameOver_Update() // 0x8003A52C
    {
        constexpr int  TIP_COUNT    = 15;
        constexpr auto TIP_STR_KEYS = std::array<const char*, TIP_COUNT>
        {
            KEY_GAME_OVER_TIP_1,
            KEY_GAME_OVER_TIP_2,
            KEY_GAME_OVER_TIP_3,
            KEY_GAME_OVER_TIP_4,
            KEY_GAME_OVER_TIP_5,
            KEY_GAME_OVER_TIP_6,
            KEY_GAME_OVER_TIP_7,
            KEY_GAME_OVER_TIP_8,
            KEY_GAME_OVER_TIP_9,
            KEY_GAME_OVER_TIP_10,
            KEY_GAME_OVER_TIP_11,
            KEY_GAME_OVER_TIP_12,
            KEY_GAME_OVER_TIP_13,
            KEY_GAME_OVER_TIP_14,
            KEY_GAME_OVER_TIP_15
        };

        const auto& input      = g_App.GetInput();
        const auto& translator = g_App.GetTranslator();

        static int prevTipIdx;

        auto seenTipIdxs = Bitfield(TIP_COUNT);
        int  tipIdx;
        int  randTipVal;

        switch (g_SysWork.sysStateSteps[0])
        {
            case 0:
                g_MapOverlayHdr.playerControlFreeze();
                g_SysWork.sysStateStepData[0] = Q12(0.0f);

                if (g_GameWork.autosave.continueCount < 99)
                {
                    g_GameWork.autosave.continueCount++;
                }

                MainMenu_SelectedOptionIdxReset();

                // If every game over tip has been seen, reset flag bits.
                if (g_GameWork.config.seenGameOverTips.TestAll())
                {
                    g_GameWork.config.seenGameOverTips.ClearAll();
                }

                randTipVal = 0;

                seenTipIdxs = g_GameWork.config.seenGameOverTips;
                for (tipIdx = 0; tipIdx < TIP_COUNT; tipIdx++)
                {
                    if (!seenTipIdxs.Test(tipIdx))
                    {
                        if ((!(g_SysWork.field_2388.field_154.effectsInfo.field_0.field_0 & 0x3) && (tipIdx - 13) >= 2u) ||
                            ( (g_SysWork.field_2388.field_154.effectsInfo.field_0.field_0 & 0x3) && (tipIdx - 13) <  2u))
                        {
                            randTipVal += 3;
                        }
                        else
                        {
                            randTipVal++;
                        }
                    }
                }

                randTipVal = Rng_GenerateInt(0, randTipVal - 1);

                // `randTipVal` seems to go unused after loop, gets checked during loop and can cause early exit,
                // thereby affecting what `tipIdx` will contain.
                for (tipIdx = 0; tipIdx < TIP_COUNT; tipIdx++)
                {
                    if (!seenTipIdxs.Test(tipIdx))
                    {
                        if ((!(g_SysWork.field_2388.field_154.effectsInfo.field_0.field_0 & 0x3) && (tipIdx - 13) >= 2u) ||
                            ( (g_SysWork.field_2388.field_154.effectsInfo.field_0.field_0 & 0x3) && (tipIdx - 13) <  2u))
                        {
                            if (randTipVal < 3)
                            {
                                break;
                            }

                            randTipVal -= 3;
                        }
                        else
                        {
                            if (randTipVal <= 0)
                            {
                                break;
                            }

                            randTipVal--;
                        }
                    }
                }

                // Store current shown `tipIdx`, later `sysStateSteps == 7` will set it inside `seenGameOverTips`.
                prevTipIdx = tipIdx;
                SysWork_StateStepIncrement(0);

            case 1:
                //(ScreenFadeCmd_Auto, true, ScreenFadeType_Black, Q12(0.5f), false);
                break;

            case 2:
                //Event_ScreenFadeCmd(ScreenFadeCmd_Start, false, ScreenFadeType_Black, Q12(0.5f), false);
                SysWork_StateStepIncrement(0);

            case 3:
                Gfx_StringPositionSet(SCREEN_WIDTH / 2, 104);
                Gfx_StringColorSet(StringColorId_White);
                Gfx_StringDraw(translator(KEY_GAME_OVER_HEADING));

                g_SysWork.sysStateStepData[0]++;
                if (input.GetAction(In::Enter).IsClicked()  ||
                    input.GetAction(In::Cancel).IsClicked() ||
                    g_SysWork.sysStateStepData[0] > SECONDS_60_FPS(4))
                {
                    SysWork_StateStepIncrement(0);
                }
                break;

            case 4:
                Gfx_StringPositionSet(SCREEN_WIDTH / 2, 104);
                Gfx_StringColorSet(StringColorId_White);
                Gfx_StringDraw(translator(KEY_GAME_OVER_HEADING));

                //Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, ScreenFadeType_Black, Q12(2.0f), false);
                break;

            case 5:
                if (g_SavegamePtr->gameDifficulty == GameDifficulty_Hard)
                {
                    SysWork_StateStepReset();
                    break;
                }
                else
                {
                    Fs_QueueWaitForEmpty();
                    Game_RadioSoundStop();
                    SysWork_StateStepIncrement(0);
                }

            case 6:
                Gfx_StringPositionSet(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 3) * 2);
                Gfx_StringColorSet(StringColorId_White);
                Gfx_StringDraw(translator(TIP_STR_KEYS[prevTipIdx]));

                //Event_ScreenFadeCmd(ScreenFadeCmd_Auto, false, ScreenFadeType_Black, Q12(2.0f), false);
                g_SysWork.sysStateStepData[0] = Q12(0.0f);
                break;

            case 7:
                Gfx_StringPositionSet(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 3) * 2);
                Gfx_StringColorSet(StringColorId_White);
                Gfx_StringDraw(translator(TIP_STR_KEYS[prevTipIdx]));

                g_SysWork.sysStateStepData[0]++;
                if (!input.GetAction(In::Enter).IsClicked() &&
                    !input.GetAction(In::Cancel).IsClicked())
                {
                    if (g_SysWork.sysStateStepData[0] <= SECONDS_60_FPS(8))
                    {
                        break;
                    }
                }

                g_GameWork.config.seenGameOverTips.Set(prevTipIdx);

                SysWork_StateStepIncrement(0);
                break;

            case 8:
                Gfx_StringPositionSet(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 3) * 2);
                Gfx_StringColorSet(StringColorId_White);
                Gfx_StringDraw(translator(TIP_STR_KEYS[prevTipIdx]));

                //Event_ScreenFadeCmd(ScreenFadeCmd_Auto, true, ScreenFadeType_Black, Q12(2.0f), false);
                break;

            default:
                g_MapOverlayHdr.playerControlUnfreeze(0);
                SysWork_StateSetNext(SysState_Gameplay);
                //Game_WarmBoot();
                break;
        }

        if (g_SysWork.sysStateSteps[0] >= 2 || g_GameWork.gameState != GameState_InGame)
        {
            g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;
        }
    }

    void GameState_MapEvent_Update() // 0x8003AA4C
    {
        if (g_GameWork.gameStateSteps[0] == 0)
        {
            g_IntervalVBlanks            = 1;
            ScreenFade_Start(true, true, false);
            g_GameWork.gameStateSteps[0] = 1;
        }

        g_IsLoadingFinished = ScreenFade_IsFinished() && Fs_QueueChunksLoad();

        Savegame_EventFlagSetAlt(g_MapEventData->completeEventFlag);
        g_MapOverlayHdr.mapEventFuncs[g_MapEventParam]();
        Screen_BackgroundImgDraw(&g_ItemInspectionImg);
    }
}
