#include "Framework.h"
#include "Psx.h"
#include "Game/Screens/Boot/Boot.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Assets/AssetStreamer.h"
#include "Assets/TranslationKeys.h"
#include "Game/Bodyprog/Demo.h"
#include "Game/Bodyprog/MemCard.h"
#include "Game/Bodyprog/Screen/BackgroundDraw.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Bodyprog/Sound/SoundSystem.h"
#include "Game/Bodyprog/Sys/FsScreens.h"
#include "Game/Bodyprog/Sys/Joy.h"
#include "Game/Bodyprog/Text/TextDraw.h"
#include "Game/Main/FsQueue.h"
#include "Game/Screens/Stream/Stream.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"
#include "Services/Clock.h"

using namespace Silent::Assets;
using namespace Silent::Input;
using namespace Silent::Renderer;
using namespace Silent::Services;

namespace Silent::Game
{
    void GameState_Init_Update()
    {
        const auto& options = g_App.GetOptions();

        switch (g_GameWork.gameStateSteps[0])
        {
            case 0:
            {
                // Set background color.
                g_GameWork.background2dColor = { 0, 0, 0 };

                // @todo Init these systems.

                //WorldGfx_HarryCharaLoad();
                //GameFs_BgItemLoad();
                //Map_EffectTexturesLoad(NO_VALUE);

                //SD_Init();
                //if (!Sd_AudioStreamingCheck())
                //{
                //    int vabAudioTaskId = g_BaseVabAudiosTaskId[g_GameWork.gameStateSteps[1]];
                //    if (vabAudioTaskId != 0)
                //    {
                //        SD_Call(vabAudioTaskId);
                //        Game_StateStepIncrement(1);
                //    }
                //    else
                //    {
                //        Game_StateStepIncrement(0);
                //    }
                //}

                // @todo
                //WorldGfx_HarryCharaLoad();
                //GameFs_BgItemLoad();
                //Map_EffectTexturesLoad(NO_VALUE);

                //Settings_RestoreDefaults();
                //GameFs_BgEtcGfxLoad();

                GameFs_TitleGfxLoad();

                //Settings_ScreenAndVolUpdate();
                Demo_SequenceAdvance(0);
                Demo_DemoDataRead();

                //MemCard_Update();
                //MemCard_ElementsUpdate();

                Game_StateStepIncrement(0);
                break;
            }
            case 1:
            {
                if (options.HasCreatedNewFile())
                {
                    Game_StateSetNext(GameState_LanguageScreen);
                }
                else
                {
                    Game_StateSetNext(GameState_SplashScreen);
                }
                break;
            }
        }
    }

    void GameState_LanguageScreen_Update()
    {
        constexpr float FADE_SEC = 1.0f;

        const auto& input      = g_App.GetInput();
        auto&       assets     = g_App.GetAssets();
        auto&       options    = g_App.GetOptions();
        auto&       renderer   = g_App.GetRenderer();
        auto&       translator = g_App.GetTranslator();

        static int  langIdx     = NO_VALUE;
        static auto langLabel   = std::string();
        static auto langComment = std::string();

        switch (g_GameWork.gameStateSteps[0])
        {
            case 0:
            {
                assets.Load("Textures/LanguageIcon.png");

                Game_StateStepIncrement(0);
                return;
            }
            case 1:
            {
                if (!assets.IsBusy())
                {
                    ScreenFade_Start(true, true, false, Q12(FADE_SEC));

                    Game_StateStepIncrement(0);
                    break;
                }
                return;
            }
            case 2:
            {
                const auto& locales = translator.GetLocales();

                // Collect languages.
                auto langItems = std::vector<const char*>{};
                langItems.reserve(locales.size());
                for (int i = 0; i < locales.size(); i++)
                {
                    const auto& locale = locales[i];

                    langItems.push_back(locale.Name.c_str());
                    if (langIdx == NO_VALUE && locale.Name == translator.GetActiveLocaleName())
                    {
                        langIdx = i;
                    }
                }

                // Select language.
                if (input.GetAction(In::Left).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    SD_Call(Sfx_MenuMove);

                    langIdx--;
                    if (langIdx < 0)
                    {
                        langIdx = locales.size() - 1;
                    }
                }
                else if (input.GetAction(In::Right).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    SD_Call(Sfx_MenuMove);

                    langIdx++;
                    if (langIdx >= locales.size())
                    {
                        langIdx = 0;
                    }
                }
                else if (ScreenFade_IsNone() && input.GetAction(In::Enter).IsClicked())
                {
                    SD_Call(Sfx_MenuConfirm);

                    const auto& locale = locales[langIdx];

                    translator.SetActiveLocale(options->Language);
                    options->Language = locale.Name;
                    options.Save();

                    Game_StateStepIncrement(0);
                }

                // Set label and comment.
                const auto& locale = locales[langIdx];
                langLabel          = locale.Label;
                langComment        = locale.Comment;
                break;
            }
            case 3:
            {
                ScreenFade_Start(true, false, false, Q12(FADE_SEC));

                Game_StateStepIncrement(0);
                break;
            }
            case 4:
            {
                if (ScreenFade_IsFinished() && !assets.IsBusy())
                {
                    langIdx     = 0;
                    langLabel   = {};
                    langComment = {};
                    assets.Unload("Textures/LanguageIcon.png");

                    Game_StateSetNext(GameState_SplashScreen);
                    return;
                }
                break;
            }
        }

        const auto* texAsset = assets["Textures/LanguageIcon.png"];
        const auto  texData  = texAsset->GetData<PngAsset>();

        // Submit language label text.
        Gfx_StringPositionSet(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 4) * 3);
        Gfx_StringColorSet(StringColorId_White);
        Gfx_StringDraw("{M}" + langLabel);

        // Submit language comment text.
        Gfx_StringDraw("{L0}" + langComment, INT_MAX, true);

        // @todo Draw arrows.

        // Submit language icon sprite. @todo Use native scale.
        auto sprite = Sprite2d::CreateSprite2d("Textures/LanguageIcon.png", Vector2::Zero, Vector2::One,
                                               Vector2(SCREEN_SPACE_RES.x * 0.5f, SCREEN_SPACE_RES.y * 0.4f),
                                               DEG_TO_RAD(0.0f), (float)texData->Resolution.y / RETRO_SCREEN_SPACE_RES.y,
                                               Color::White, NO_VALUE,
                                               DEPTH_2D_MAX, AlignMode::Center, ScaleMode::Fit, BlendMode::Alpha);
        renderer.SubmitSprite2d(sprite);
    }

    void GameState_SplashScreen_Update()
    {
        constexpr float DISPLAY_SEC = 3.0f;
        constexpr float FADE_SEC    = 1.0f;

        const auto& input      = g_App.GetInput();
        const auto& options    = g_App.GetOptions();
        const auto& translator = g_App.GetTranslator();
        auto&       assets     = g_App.GetAssets();

        switch (g_GameWork.gameStateSteps[0])
        {
            case 0:
            {
                assets.Load("Textures/SplashScreen.png");

                Game_StateStepIncrement(0);
                return;
            }
            case 1:
            {
                if (!assets.IsBusy())
                {
                    ScreenFade_Start(true, true, false, Q12(FADE_SEC));

                    Game_StateStepIncrement(0);
                }
                break;
            }
            case 2:
            {
                if (input.GetAction(In::Enter).IsClicked()  ||
                    input.GetAction(In::Cancel).IsClicked() ||
                    g_SysWork.gameStateCounter >= SEC_TO_TICK(DISPLAY_SEC))
                {
                    ScreenFade_Start(false, false, false, Q12(FADE_SEC));

                    Game_StateStepIncrement(0);
                }
                break;
            }
            case 3:
            {
                if (ScreenFade_IsFinished() && !assets.IsBusy())
                {
                    assets.Unload("Textures/SplashScreen.png");

                    if (options->SkipLogos)
                    {
                        Game_StateSetNext(GameState_MovieIntro);
                    }
                    else
                    {
                        Game_StateSetNext(GameState_LogosScreen);
                    }
                    return;
                }
                break;
            }
        }

        Screen_BackgroundImgDraw("Textures/SplashScreen.png", true, NO_VALUE);
        
        // @todo Style it properly.
        Gfx_StringPositionSet(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 7) * 3);
        Gfx_StringColorSet(StringColorId_White);
        Gfx_StringDraw(translator(KEY_INTRO_WARNING));
    }

    void GameState_LogosScreen_Update()
    {
        constexpr float FADE_SEC    = 1.0f;
        constexpr float DISPLAY_SEC = 3.0f;

        const auto& input  = g_App.GetInput();
        auto&       assets = g_App.GetAssets();

        static auto bgTexName = std::string();
        switch (g_GameWork.gameStateSteps[0])
        {
            case 0:
            {
                assets.Load("Psx/1ST/KONAMI2.TIM");
                assets.Load("Psx/1ST/KONAMI.TIM");

                Game_StateStepIncrement(0);
                return;
            }
            case 1:
            {
                if (!assets.IsBusy())
                {
                    ScreenFade_Start(true, true, false, Q12(FADE_SEC));
                    bgTexName = "Psx/1ST/KONAMI2.TIM";

                    Game_StateStepIncrement(0);
                    break;
                }
                return;
            }
            case 2:
            {
                if (input.GetAction(In::Enter).IsClicked()  ||
                    input.GetAction(In::Cancel).IsClicked() ||
                    g_SysWork.gameStateCounter >= SEC_TO_TICK(DISPLAY_SEC))
                {
                    ScreenFade_Start(false, false, false, Q12(FADE_SEC));
                    g_SysWork.gameStateCounter = 0; // @todo Cleaner way?

                    Game_StateStepIncrement(0);
                }
                break;
            }
            case 3:
            {
                if (ScreenFade_IsFinished())
                {
                    ScreenFade_Start(true, true, false, Q12(FADE_SEC));
                    bgTexName = "Psx/1ST/KONAMI.TIM";

                    Game_StateStepIncrement(0);
                    break;
                }
                break;
            }
            case 4:
            {
                if (input.GetAction(In::Enter).IsClicked()  ||
                    input.GetAction(In::Cancel).IsClicked() ||
                    g_SysWork.gameStateCounter >= SEC_TO_TICK(DISPLAY_SEC))
                {
                    ScreenFade_Start(false, false, false, Q12(FADE_SEC));

                    Game_StateStepIncrement(0);
                }
                break;
            }
            case 5:
                if (ScreenFade_IsFinished() && !assets.IsBusy())
                {
                    assets.Unload("Psx/1ST/KONAMI2.TIM");
                    assets.Unload("Psx/1ST/KONAMI.TIM");
                    bgTexName = {};

                    Game_StateSetNext(GameState_MovieIntro);
                    return;
                }
                break;
        }

        Screen_BackgroundImgDraw(bgTexName, true, 0, Color::From8Bit(248, 248, 248));
    }
}
