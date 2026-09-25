#include "Framework.h"
#include "Debug/Menu/Tabs/Options.h"

#include "Application.h"
#include "Renderer/Renderer.h"
#include "Services/Options.h"
#include "Utils/Translator.h"

using namespace Silent::Renderer;
using namespace Silent::Services;
using namespace Silent::Utils;

namespace Silent::Debug
{
    constexpr const char* FRAME_RATE_ITEMS[]        = { "30 FPS", "60 FPS" };
    constexpr const char* ASPECT_RATIO_ITEMS[]      = { "Retro", "Wide", "Native"  };
    constexpr const char* RENDER_SCALE_ITEMS[]      = { "Retro", "Retro 2x", "Native" };
    constexpr const char* TEXTURE_FILTER_ITEMS[]    = { "Nearest", "Linear" };
    constexpr const char* TEXT_QUALITY_ITEMS[]      = { "Retro", "Modern" };
    constexpr const char* LIGHTING_ITEMS[]          = { "Retro", "Modern" };
    constexpr const char* ANTIALIASING_ITEMS[]      = { "None", "Low", "High" };
    constexpr const char* DITHERING_SCALE_ITEMS[]   = { "None", "Retro", "Retro 2x", "Native" };
    constexpr const char* SOUND_ITEMS[]             = { "Stereo", "Monaural" };
    constexpr const char* BLOOD_COLOR_ITEMS[]       = { "Normal", "Green", "Violet", "Black" };
    constexpr const char* CONTROL_INVERSION_ITEMS[] = { "Normal", "Reverse" };
    constexpr const char* WEAPON_CONTROL_ITEMS[]    = { "Switch", "Press" };
    constexpr const char* VIEW_MODE_ITEMS[]         = { "Normal", "Self view" };
    constexpr const char* PAPER_MAP_QUALITY_ITEMS[] = { "Retro", "Modern" };
    constexpr const char* DIALOG_PAUSE_ITEMS[]      = { "Retro", "Refined" };

    void AddOptionsTab()
    {
        auto& options    = g_App.GetOptions();
        auto& renderer   = g_App.GetRenderer();
        auto& translator = g_App.GetTranslator();

        if (ImGui::BeginTabItem("Options"))
        {
            g_Work.Page       = Page::Options;
            bool isOptChanged = false;

            // `Graphics` section.
            ImGui::SeparatorText("Graphics");
            {
                // `Reset` button.
                if (ImGui::Button("Reset##0"))
                {
                    options.SetDefaultGraphicsOptions();
                    isOptChanged = true;
                }

                // `Toggle fullscreen` button.
                if (ImGui::Button("Toggle fullscreen"))
                {
                    g_App.ToggleFullscreen();
                }

                // `Brightness level` slider.
                if (ImGui::SliderInt("Brightness level", &options->BrightnessLevel, 0, BRIGHTNESS_LEVEL_MAX))
                {
                    isOptChanged = true;
                }

                // `Frame rate` combo.
                int frameRate = (int)options->FrameRate;
                if (ImGui::Combo("Frame rate", &frameRate, FRAME_RATE_ITEMS, IM_ARRAYSIZE(FRAME_RATE_ITEMS)))
                {
                    options->FrameRate = (FrameRateType)frameRate;
                    isOptChanged       = true;
                }

                // `Aspect ratio` combo.
                int aspectRatio = (int)options->AspectRatio;
                if (ImGui::Combo("Aspect ratio", &aspectRatio, ASPECT_RATIO_ITEMS, IM_ARRAYSIZE(ASPECT_RATIO_ITEMS)))
                {
                    options->AspectRatio = (AspectRatioType)aspectRatio;
                    isOptChanged         = true;
                }

                // `Render scale` combo.
                int renderScale = (int)options->RenderScale;
                if (ImGui::Combo("Render scale", &renderScale, RENDER_SCALE_ITEMS, IM_ARRAYSIZE(RENDER_SCALE_ITEMS)))
                {
                    options->RenderScale = (RenderScaleType)renderScale;
                    isOptChanged         = true;

                    if (options->RenderScale    != RenderScaleType::Native &&
                        options->DitheringScale != DitheringScaleType::None)
                    {
                        options->DitheringScale = DitheringScaleType::Native;
                    }
                    renderer.SignalResize();
                }

                // `Texture filter` combo.
                int texFilter = (int)options->TextureFilter;
                if (ImGui::Combo("Texture filter", &texFilter, TEXTURE_FILTER_ITEMS, IM_ARRAYSIZE(TEXTURE_FILTER_ITEMS)))
                {
                    options->TextureFilter = (TextureFilterType)texFilter;
                    isOptChanged           = true;
                }

                // `Text quality` combo.
                int textQuality = (int)options->TextQuality;
                if (ImGui::Combo("Text quality", &textQuality, TEXT_QUALITY_ITEMS, IM_ARRAYSIZE(TEXT_QUALITY_ITEMS)))
                {
                    options->TextQuality = (TextQualityType)textQuality;
                    isOptChanged         = true;
                }

                // `Lighting` combo.
                int lighting = (int)options->Lighting;
                if (ImGui::Combo("Lighting", &lighting, LIGHTING_ITEMS, IM_ARRAYSIZE(LIGHTING_ITEMS)))
                {
                    options->Lighting = (LightingType)lighting;
                    isOptChanged      = true;
                }

                // `Antialiasing` combo.
                int antialiasing = (int)options->Antialiasing;
                if (ImGui::Combo("Antialiasing", &antialiasing, ANTIALIASING_ITEMS, IM_ARRAYSIZE(ANTIALIASING_ITEMS)))
                {
                    options->Antialiasing = (AntialiasingType)antialiasing;
                    isOptChanged          = true;
                }

                // `Dithering scale` combo.
                int ditheringScale = (int)options->DitheringScale;
                if (ImGui::Combo("Dithering scale", &ditheringScale, DITHERING_SCALE_ITEMS, IM_ARRAYSIZE(DITHERING_SCALE_ITEMS)))
                {
                    options->DitheringScale = (DitheringScaleType)ditheringScale;
                    isOptChanged            = true;

                    if (options->RenderScale    != RenderScaleType::Native &&
                        options->DitheringScale != DitheringScaleType::None)
                    {
                        options->DitheringScale = DitheringScaleType::Native;
                    }
                }

                // `Enable ambient occlusion` checkbox.
                if (ImGui::Checkbox("Enable ambient occlusion", &options->EnableAmbientOcclusion))
                {
                    isOptChanged = true;
                }

                // `Enable vertex jitter` checkbox.
                if (ImGui::Checkbox("Enable vertex jitter", &options->EnableVertexJitter))
                {
                    isOptChanged = true;
                }

                // `Enable film grain` checkbox.
                if (ImGui::Checkbox("Enable film grain", &options->EnableFilmGrain))
                {
                    isOptChanged = true;
                }

                // `Enable vignette` checkbox.
                if (ImGui::Checkbox("Enable vignette", &options->EnableVignette))
                {
                    isOptChanged = true;
                }

                // `Enable CRT filter` checkbox.
                if (ImGui::Checkbox("Enable CRT filter", &options->EnableCrtFilter))
                {
                    isOptChanged = true;
                }
            }

            // `Gameplay` section.
            ImGui::SeparatorText("Gameplay");
            {
                // `Reset` button.
                if (ImGui::Button("Reset##1"))
                {
                    options.SetDefaultGameplayOptions();
                    isOptChanged = true;
                }

                // `Enable auto load` checkbox.
                if (ImGui::Checkbox("Enable auto load", &options->EnableAutoLoad))
                {
                    isOptChanged = true;
                }

                // `Enable subtitles` checkbox.
                if (ImGui::Checkbox("Enable subtitles", &options->EnableSubtitles))
                {
                    isOptChanged = true;
                }

                const auto& locales = translator.GetLocales();
                int         langIdx = 0;

                // Collect languages.
                auto langItems = std::vector<const char*>{};
                langItems.reserve(locales.size());
                for (int i = 0; i < locales.size(); i++)
                {
                    const auto& locale = locales[i];

                    langItems.push_back(locale.Name.c_str());
                    if (locale.Name == translator.GetActiveLocaleName())
                    {
                        langIdx = i;
                    }
                }

                // `Language` combo.
                if (ImGui::Combo("Language", &langIdx, langItems.data(), (int)langItems.size()))
                {
                    options->Language = locales[langIdx].Name;
                    translator.SetActiveLocale(options->Language);

                    isOptChanged = true;
                }

                // `Sound type` combo.
                int sound = (int)options->Sound;
                if (ImGui::Combo("Sound", &sound, SOUND_ITEMS, IM_ARRAYSIZE(SOUND_ITEMS)))
                {
                    options->Sound = (SoundType)sound;
                    isOptChanged   = true;
                }

                // `Music volume` slider.
                if (ImGui::SliderInt("Music volume", &options->BgmVolume, 0, SOUND_VOLUME_MAX))
                {
                    isOptChanged = true;
                }

                // `SFX volume` slider.
                if (ImGui::SliderInt("SFX volume", &options->SeVolume, 0, SOUND_VOLUME_MAX))
                {
                    isOptChanged = true;
                }

                // `Blood color type` combo.
                int bloodColor = (int)options->BloodColor;
                if (ImGui::Combo("Blood color", &bloodColor, BLOOD_COLOR_ITEMS, IM_ARRAYSIZE(BLOOD_COLOR_ITEMS)))
                {
                    options->BloodColor = (BloodColorType)bloodColor;
                    isOptChanged        = true;
                }

                // `Bullet adjust` slider.
                if (ImGui::SliderInt("Bullet adjust", &options->BulletAdjust, BULLET_ADJUST_MIN, BULLET_ADJUST_MAX))
                {
                    isOptChanged = true;
                }
            }

            // `Input` section.
            ImGui::SeparatorText("Input");
            {
                // `Reset` button.
                if (ImGui::Button("Reset##2"))
                {
                    options.SetDefaultInputControlsOptions();
                    isOptChanged = true;
                }

                // `Enable vibration` checkbox.
                if (ImGui::Checkbox("Enable vibration", &options->EnableVibration))
                {
                    isOptChanged = true;
                }

                // `Mouse sensitivity` slider.
                if (ImGui::SliderInt("Mouse sensitivity", &options->MouseSensitivity, 1, MOUSE_SENSITIVITY_MAX))
                {
                    isOptChanged = true;
                }

                // `Weapon control` combo.
                int weaponCtrl = (int)options->WeaponControl;
                if (ImGui::Combo("Weapon control", &weaponCtrl, WEAPON_CONTROL_ITEMS, IM_ARRAYSIZE(WEAPON_CONTROL_ITEMS)))
                {
                    options->WeaponControl = (WeaponControlType)weaponCtrl;
                    isOptChanged           = true;
                }

                // `View control` combo.
                int viewCtrl = (int)options->ViewControl;
                if (ImGui::Combo("View control", &viewCtrl, CONTROL_INVERSION_ITEMS, IM_ARRAYSIZE(CONTROL_INVERSION_ITEMS)))
                {
                    options->ViewControl = (ControlInversionType)viewCtrl;
                    isOptChanged         = true;
                }

                // `Retreat turn control` combo.
                int retreatTurnCtrl = (int)options->RetreatTurnControl;
                if (ImGui::Combo("Retreat turn control", &retreatTurnCtrl, CONTROL_INVERSION_ITEMS, IM_ARRAYSIZE(CONTROL_INVERSION_ITEMS)))
                {
                    options->RetreatTurnControl = (ControlInversionType)retreatTurnCtrl;
                    isOptChanged                = true;
                }

                // `Walk/run control` combo.
                int walkRunCtrl = (int)options->WalkRunControl;
                if (ImGui::Combo("Walk/run control", &walkRunCtrl, CONTROL_INVERSION_ITEMS, IM_ARRAYSIZE(CONTROL_INVERSION_ITEMS)))
                {
                    options->WalkRunControl = (ControlInversionType)walkRunCtrl;
                    isOptChanged            = true;
                }

                // `Disable auto aiming` checkbox.
                if (ImGui::Checkbox("Disable auto aiming", &options->DisableAutoAiming))
                {
                    isOptChanged = true;
                }

                // `View mode` combo.
                int viewMode = (int)options->ViewMode;
                if (ImGui::Combo("View mode", &viewMode, VIEW_MODE_ITEMS, IM_ARRAYSIZE(VIEW_MODE_ITEMS)))
                {
                    options->ViewMode = (ViewMode)viewMode;
                    isOptChanged      = true;
                }
            }

            // `Enhancements` section.
            ImGui::SeparatorText("Enhancements");
            {
                // `Reset` button.
                if (ImGui::Button("Reset##3"))
                {
                    options.SetDefaultEnhancementsOptions();
                    isOptChanged = true;
                }

                // `Skip logos` checkbox.
                if (ImGui::Checkbox("Skip logos", &options->SkipLogos))
                {
                    isOptChanged = true;
                }

                // `Paper map quality` combo.
                int paperMapQuality = (int)options->PaperMapQuality;
                if (ImGui::Combo("Paper map quality", &paperMapQuality, PAPER_MAP_QUALITY_ITEMS, IM_ARRAYSIZE(PAPER_MAP_QUALITY_ITEMS)))
                {
                    options->PaperMapQuality = (PaperMapQualityType)paperMapQuality;
                    isOptChanged             = true;
                }

                // `Dialog pause` combo.
                int dialogPause = (int)options->DialogPause;
                if (ImGui::Combo("Dialog pause", &dialogPause, DIALOG_PAUSE_ITEMS, IM_ARRAYSIZE(DIALOG_PAUSE_ITEMS)))
                {
                    options->DialogPause = (DialogPauseType)dialogPause;
                    isOptChanged         = true;
                }
            }

            // `System` section.
            ImGui::SeparatorText("System");
            {
                // `Reset` button.
                if (ImGui::Button("Reset##4"))
                {
                    options.SetDefaultSystemOptions();
                    isOptChanged = true;
                }

                // `Enable toasts` checkbox.
                if (ImGui::Checkbox("Enable toasts", &options->EnableToasts))
                {
                    isOptChanged = true;
                }

                // `Enable parallelism` checkbox.
                if (ImGui::Checkbox("Enable parallelism", &options->EnableParallelism))
                {
                    isOptChanged = true;
                }
            }

            // Save options if changed.
            if (isOptChanged)
            {
                options.Save();
            }

            ImGui::EndTabItem();
        }
    }
}
