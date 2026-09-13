#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/Bodyprog.h"
#include "Game/Bodyprog/Demo.h"

#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Bodyprog/Text/TextDraw.h"
#include "Game/Main/FsQueue.h"
#include "Game/Main/Rng.h"

namespace Silent::Game
{
    /** @brief Demo states. */
    enum e_DemoState
    {
        DemoState_Exit = -1,
        DemoState_None = 0,
        DemoState_Step = 1
    };

    /** @brief Initial demo game state data, stored inside `MISC/DEMO****.DAT` files. */
    struct s_DemoWork
    {
        s_OptionsConfig config;
        u8              unk_38[200];
        s_Savegame      savegame;
        u8              unk_37C[1148];
        u32             frameCount;
        u16             randSeed;
    };

    /** @brief Per-frame demo data, stored inside `MISC/PLAY****.DAT` files. */
    struct s_DemoFrameData
    {
        s_AnalogController analogController;
        s8                 gameStateExpected; /** Expected value of `g_GameWork.gameState` before `analogController` is processed.
                                               * If it doesn't match,`Demo_Update` displays `STEP ERROR` and stops reading the demo.
                                               */
        u8                 videoPresentInterval;
        s8                 unk_A[2];
        u32                randSeed;
    };

    /** @brief Associates a demo number/ID with `PLAY****.DAT/DEMO****.DAT` file IDs. */
    struct s_DemoFileInfo
    {
        e_FsFile demoFileId;       /** `MISC/DEMO****.DAT`, initial gamestate for the demo and user config override. */
        e_FsFile playFileId;       /** `MISC/PLAY****.DAT`, data of button presses/randseed for each frame. */
        bool     (*canPlayDemo)(); /** Optional funcptr, returns whether this demo is eligible to be played (unused in retail demos). */
    };

    s_DemoWork       g_DemoWork = {};
    e_FsFile         g_Demo_DemoFileIdx;
    e_FsFile         g_Demo_PlayFileIdx;
    s_OptionsConfig  g_Demo_UserConfigBackup;
    u32              g_Demo_PrevRandSeed;
    u32              g_Demo_RandSeedBackup;
    s_DemoFrameData* g_Demo_CurFrameData;
    s32              g_Demo_DemoStep;
    s32              g_Demo_VideoPresentInterval;
    bool             g_Demo_IsLoadingChunks;
    s32              g_Demo_DemoId   = 0;
    u16              g_Demo_RandSeed = 0;

    static s_DemoFrameData g_Demo_PlayFileBuffer    = {};
    s_DemoFrameData*       g_Demo_PlayFileBufferPtr = &g_Demo_PlayFileBuffer;//(s_DemoFrameData*)0x800F5E00;

    bool Demo_SequenceAdvance(s32 incrementAmount) // 0x8008EF20
    {
        constexpr int DEMO_FILE_COUNT_MAX = 5;

        static auto DEMO_FILE_INFOS = std::array<s_DemoFileInfo, DEMO_FILE_COUNT_MAX>
        {
            //s_DemoFileInfo{ .demoFileId = FILE_MISC_DEMO0009_DAT, .playFileId = FILE_MISC_PLAY0009_DAT, .canPlayDemo = nullptr },
            //s_DemoFileInfo{ .demoFileId = FILE_MISC_DEMO000A_DAT, .playFileId = FILE_MISC_PLAY000A_DAT, .canPlayDemo = nullptr },
            //s_DemoFileInfo{ .demoFileId = FILE_MISC_DEMO0003_DAT, .playFileId = FILE_MISC_PLAY0003_DAT, .canPlayDemo = nullptr },
            //s_DemoFileInfo{ .demoFileId = FILE_MISC_DEMO000B_DAT, .playFileId = FILE_MISC_PLAY000B_DAT, .canPlayDemo = nullptr },
            //s_DemoFileInfo{ .demoFileId = FILE_MISC_DEMO0005_DAT, .playFileId = FILE_MISC_PLAY0005_DAT, .canPlayDemo = nullptr }
        };

        g_Demo_DemoId += incrementAmount;

        while (true)
        {
            // Cycle demo ID.
            while (g_Demo_DemoId < 0)
            {
                g_Demo_DemoId += DEMO_FILE_COUNT_MAX;
            }
            while ((u32)g_Demo_DemoId >= DEMO_FILE_COUNT_MAX)
            {
                g_Demo_DemoId -= DEMO_FILE_COUNT_MAX;
            }

            // Call optional funcptr associated with this demo.
            // If funcptr is set, return whether demo is eligible to play, possibly based on game progress or other conditions.
            // In retail demos this pointer is always `nullptr`.
            if (DEMO_FILE_INFOS[g_Demo_DemoId].canPlayDemo == nullptr ||
                DEMO_FILE_INFOS[g_Demo_DemoId].canPlayDemo())
            {
                break;
            }

            // If funcptr is set and returned false, skip to next demo.
            // Direction to skip depends on sign of `incrementAmount` (forward or backward).
            if (incrementAmount >= 0)
            {
                g_Demo_DemoId++;
            }
            else
            {
                g_Demo_DemoId--;
            }
        }

        g_Demo_DemoFileIdx = DEMO_FILE_INFOS[g_Demo_DemoId].demoFileId;
        g_Demo_PlayFileIdx = DEMO_FILE_INFOS[g_Demo_DemoId].playFileId;
        return true;
    }

    void Demo_DemoDataRead() // 0x8008F048
    {
        if (g_Demo_DemoFileIdx != NO_VALUE)
        {
            Fs_QueueStartRead(g_Demo_DemoFileIdx, &g_DemoWork);
        }
    }

    void Demo_PlayDataRead() // 0x8008F07C
    {
        Demo_SequenceAdvance(0);

        if (g_Demo_PlayFileIdx != NO_VALUE)
        {
            Fs_QueueStartRead(g_Demo_PlayFileIdx, g_Demo_PlayFileBufferPtr);
        }
    }

    s32 Demo_PlayFileBufferSetup() // 0x8008F0BC
    {
        // @stub
        return true;
    }

    void Demo_DemoFileSavegameUpdate() // 0x8008F13C
    {
        g_GameWork.savegame = g_DemoWork.savegame;
    }

    void Demo_GameGlobalsUpdate() // 0x8008F1A0
    {
        // Backup current user config.
        g_Demo_UserConfigBackup = g_GameWork.config;

        // Update `Demo_RandSeed`.
        g_Demo_RandSeed = g_DemoWork.randSeed;

        // Replace user config with config from demo file.
        g_GameWork.config = g_DemoWork.config;

        // Restore user system settings over demo values.
        g_GameWork.config.screenPositionX  = g_Demo_UserConfigBackup.screenPositionX;
        g_GameWork.config.screenPositionY  = g_Demo_UserConfigBackup.screenPositionY;
        g_GameWork.config.soundType        = g_Demo_UserConfigBackup.soundType;
        g_GameWork.config.volumeBgm        = OPT_SOUND_VOLUME_MIN; // Disable BGM during demo.
        g_GameWork.config.volumeSe         = g_Demo_UserConfigBackup.volumeSe;
        g_GameWork.config.vibrationEnabled = OPT_VIBRATION_DISABLED; // Disable vibration during demo.
        g_GameWork.config.brightness       = g_Demo_UserConfigBackup.brightness;

        //Sd_SetVolume(OPT_SOUND_VOLUME_MIN, OPT_SOUND_VOLUME_MIN, g_GameWork.config.volumeSe);
    }

    void Demo_GameGlobalsRestore() // 0x8008F2BC
    {
        g_GameWork.config = g_Demo_UserConfigBackup;

        //Sd_SetVolume(OPT_SOUND_VOLUME_MAX, g_GameWork.config.volumeBgm, g_GameWork.config.volumeSe);
    }

    void Demo_GameRandSeedUpdate() // 0x8008F33C
    {
        g_Demo_PrevRandSeed = Rng_GetSeed();
        Rng_SetSeed(g_Demo_RandSeed);
    }

    void Demo_GameRandSeedRestore() // 0x8008F370
    {
        Rng_SetSeed(g_Demo_PrevRandSeed);
    }

    bool g_Demo_Play = false;

    void Demo_Start() // 0x8008F398
    {
        g_Demo_Play = true;
        g_SysWork.sysFlags |= SysFlag_DemoActive;

        Demo_GameGlobalsUpdate();
        Demo_GameRandSeedUpdate();

        g_GameWork.field_5A8 = 1;
        g_GameWork.field_5AC = 1;
    }

    void Demo_Stop() // 0x8008f3f0
    {
        g_Demo_Play = false;
    g_SysWork.sysFlags &= ~SysFlag_DemoActive;

        Demo_GameGlobalsRestore();
        Demo_GameRandSeedRestore();
    }

    bool Gfx_ScreenFadeIn_IsInProgress(s32 arg0)
    {
        s32 screenFadeStatus;

        screenFadeStatus = arg0 & ~(1 << 0);

        switch (screenFadeStatus)
        {
            case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutStart, false):
            case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutSteps, false):
            case SCREEN_FADE_STATUS(ScreenFadeState_ResetTimestep, false):
            case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, false):
            case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutStart, true):
            case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutSteps, true):
            case SCREEN_FADE_STATUS(ScreenFadeState_ResetTimestep, true):
            case SCREEN_FADE_STATUS(ScreenFadeState_FadeOutComplete, true):
                return false;
        }

        return true;
    }

    s32 Demo_StateGet(s32 gameState)
    {
        switch (gameState)
        {
            case GameState_InGame:
                if (g_SysWork.sysState == SysState_GameOver)
                {
                    return DemoState_Exit;
                }
                else if (g_GameWork.gameStatePrev == GameState_SaveScreen)
                {
                    return DemoState_Exit;
                }

            case GameState_MapEvent:
            case GameState_ExitMovie:
            case GameState_InventoryScreen:
            case GameState_PaperMapScreen:
                return DemoState_Step;

            case GameState_OptionScreen:
                return DemoState_Step;
        }

        return DemoState_None;
    }

    void Demo_ExitDemo() // 0x8008F4E4
    {
        g_Demo_FrameCount     = 999 * TICKS_PER_SECOND;
        g_Demo_CurFrameData   = nullptr;
        g_Demo_DemoStep       = 0;
        g_SysWork.sysFlags |= SysFlag_DoWarmReset;
    }

    bool func_8008F520() // 0x8008F520
    {
        return false;
    }

    void Demo_DemoRandSeedBackup() // 0x8008F528
    {
        if (g_SysWork.sysFlags & SysFlag_DemoActive)
        {
            g_Demo_RandSeedBackup = Rng_GetSeed();
        }
    }

    void Demo_DemoRandSeedRestore() // 0x8008F560
    {
        if (g_SysWork.sysFlags & SysFlag_DemoActive)
        {
            Rng_SetSeed(g_Demo_RandSeedBackup);
        }
    }

    void Demo_DemoRandSeedAdvance() // 0x8008F598
    {
        #define SEED_OFFSET 0x3C6EF35F

        if (g_SysWork.sysFlags & SysFlag_DemoActive)
        {
            Rng_SetSeed(g_Demo_RandSeedBackup + SEED_OFFSET);
        }
    }

    bool Demo_Update() // 0x8008F5D8
    {
        s32         prevScreenFadeCpy;
        bool        cond;
        u32         demoStep;
        s_GameWork* gameWork;

        static s32 prevScreenFade = SCREEN_FADE_STATUS(ScreenFadeState_Reset, false);

        prevScreenFadeCpy      = prevScreenFade;
        cond                   = g_Demo_IsLoadingChunks;
        g_Demo_IsLoadingChunks = false;
        prevScreenFade         = g_Screen_FadeStatus;

        if (!(g_SysWork.sysFlags & SysFlag_DemoActive))
        {
            g_Demo_CurFrameData = nullptr;
            g_Demo_DemoStep     = 0;
            return true;
        }

        if (g_Demo_PlayFileBufferPtr == nullptr)
        {
            g_Demo_CurFrameData = nullptr;
            return false;
        }

        demoStep = g_Demo_DemoStep;

        if (g_DemoWork.frameCount <= demoStep)
        {
            Demo_ExitDemo();
            return false;
        }

        if (!Gfx_ScreenFadeIn_IsInProgress(prevScreenFadeCpy) || !Gfx_ScreenFadeIn_IsInProgress(g_Screen_FadeStatus) || cond)
        {
            g_Demo_CurFrameData = nullptr;
            return true;
        }

        gameWork = &g_GameWork;

        // Handle demo state.
        switch (Demo_StateGet(gameWork->gameState))
        {
            case DemoState_Step:
                g_Demo_CurFrameData = &g_Demo_PlayFileBufferPtr[g_Demo_DemoStep];

                if (g_Demo_CurFrameData->gameStateExpected != gameWork->gameState)
                {
                    //Text_Debug_PositionSet(8, 80);
                    //Text_Debug_Draw("STEP ERROR:[H:");
                    //Text_Debug_Draw(Text_Debug_IntToString(2, g_Demo_CurFrameData->gameStateExpected_8));
                    //Text_Debug_Draw("]/[M:");
                    //Text_Debug_Draw(Text_Debug_IntToString(2, gameWork->gameState));
                    //Text_Debug_Draw("]");

                    g_Demo_CurFrameData = nullptr;
                }

                g_Demo_DemoStep++;
                return true;

            case DemoState_Exit:
                Demo_ExitDemo();
                return false;

            case DemoState_None:
                break;
        }

        g_Demo_CurFrameData = nullptr;
        return true;
    }

    const s16 unkRodata_8002B2F2 = 0x8008;

    bool Demo_ControllerDataUpdate() // 0x8008F7CC
    {
        u32 btns;

        if (!(g_SysWork.sysFlags & SysFlag_DemoActive))
        {
            return false;
        }

        btns = g_Controller0->analogController.digitalButtons;
        if (btns != 0xFFFF)
        {
            Demo_ExitDemo();
            return true;
        }

        g_Demo_FrameCount = 0;

        if (g_Demo_CurFrameData != nullptr)
        {
            g_Controller0->analogController = g_Demo_CurFrameData->analogController;
            return true;
        }

        *(u16*)&g_Controller0->analogController.status = 0x7300;
        g_Controller0->analogController.digitalButtons = btns;
        g_Controller0->analogController.rightX = 128;
        g_Controller0->analogController.rightY = 128;
        g_Controller0->analogController.leftX  = 128;
        g_Controller0->analogController.leftY  = 128;
        return true;
    }

    bool Demo_PresentIntervalUpdate() // 0x8008F87C
    {
        g_Demo_VideoPresentInterval = 1;

        if (g_Demo_CurFrameData == nullptr)
        {
            return false;
        }

        g_Demo_VideoPresentInterval = g_Demo_CurFrameData->videoPresentInterval;
        return true;
    }

    bool Demo_GameRandSeedSet() // 0x8008F8A8
    {
        if (!(g_SysWork.sysFlags & SysFlag_DemoActive))
        {
            return true;
        }
        else if (g_Demo_CurFrameData == nullptr)
        {
            Rng_SetSeed(g_Demo_RandSeed);
            return false;
        }
        else
        {
            Rng_SetSeed(g_Demo_CurFrameData->randSeed);
            return true;
        }
    }

    bool func_8008F914(q19_12 posX, q19_12 posZ)
    {
        if (g_SysWork.sysFlags & SysFlag_DemoActive)
        {
            //return func_8004393C(posX, posZ);
        }

        return true;
    }
}
