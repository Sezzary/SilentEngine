#include "Framework.h"
#include "Debug.h"

#include "Application.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"
#include "Services/Time.h"
#include "Utils/Bitfield.h"
#include "Utils/Parallel.h"
#include "Utils/Utils.h"

using namespace Silent::Renderer;
using namespace Silent::Services;
using namespace Silent::Utils;

// Includes and namespace usings required for `Scratchpad` function go here.
#ifdef _DEBUG

#endif

namespace Silent::Debug
{
    constexpr char LOGGER_NAME[]     = "Logger";
    constexpr uint MESSAGE_COUNT_MAX = 128;

    static auto Messages  = std::vector<std::string>{};
    static auto StartTime = std::chrono::high_resolution_clock::time_point{};

    DebugWork g_Work = {};

    void Scratchpad()
    {
        if constexpr (IS_DEBUG_BUILD)
        {
            auto& input = g_App.GetInput();

            bool isInit = true;
            if (isInit)
            {
                input.InsertText("Test");
                isInit = false;
            }

            input.UpdateText("Test");
            Message(input.GetText("Test").c_str());

            // ====================================

            auto& renderer = g_App.GetRenderer();

            // @temp
            auto tri0 = Primitive2d::CreateTriangle(Vector2(0.0f + 0.2f, 0.5f + 0.2f),
                                                    Vector2(-0.5f + 0.2f, -0.5f + 0.2f),
                                                    Vector2(0.5f + 0.2f, -0.5f + 0.2f),
                                                    Color(1.0f, 0.0f, 1.0f, 0.5f),
                                                    Color(1.0f, 1.0f, 1.0f, 0.5f),
                                                    Color(1.0f, 0.0f, 1.0f, 0.5f),
                                                    0);
            auto tri1 = Primitive2d::CreateTriangle(Vector2(0.2f, 0.25f),
                                                    Vector2(-0.25f, -0.25f),
                                                    Vector2(0.25f, -0.25f),
                                                    Color(1.0f, 0.0f, 0.0f, 0.75f),
                                                    Color(0.0f, 1.0f, 1.0f, 0.75f),
                                                    Color(0.0f, 0.0f, 1.0f, 0.75f),
                                                    0);
            auto quad = Primitive2d::CreateQuad(Vector2(40.0f, 40.0f),
                                                Vector2(50.0f, 40.0f),
                                                Vector2(50.0f, 50.0f),
                                                Vector2(40.0f, 50.0f),
                                                Color(0.0f, 0.0f, 0.0f, 0.0f),
                                                Color(0.0f, 1.0f, 0.0f, 1.0f),
                                                Color(0.0f, 1.0f, 0.0f, 1.0f),
                                                Color(0.0f, 0.0f, 0.0f, 0.0f),
                                                0, ScaleMode::Fit, BlendMode::Alpha);
            auto line0 = Primitive2d::CreateLine(Vector2i(10, 10),
                                                Vector2i(50, 10),
                                                Color(1.0f, 1.0f, 0.0f, 1.0f),
                                                Color(0.0f, 0.0f, 0.0f, 0.0f),
                                                0);
            auto line1 = Primitive2d::CreateLine(Vector2i(15, 11),
                                                Vector2i(70, 11),
                                                Color(1.0f, 1.0f, 0.0f, 1.0f),
                                                Color(1.0f, 1.0f, 0.0f, 1.0f),
                                                0);
            auto line2 = Primitive2d::CreateLine(Vector2i(0, 1),
                                                 Vector2i(0, 239),
                                                 Color(1.0f, 1.0f, 0.0f, 1.0f),
                                                 Color(1.0f, 1.0f, 0.0f, 1.0f),
                                                 0);
            auto line3 = Primitive2d::CreateLine(Vector2i(319, 0),
                                                 Vector2i(1, 0),
                                                 Color(1.0f, 1.0f, 0.0f, 1.0f),
                                                 Color(1.0f, 1.0f, 0.0f, 1.0f),
                                                 0);
            //renderer.Submit2dPrimitive(tri0);
            //renderer.Submit2dPrimitive(tri1);
            renderer.Submit2dPrimitive(quad);
            renderer.Submit2dPrimitive(line0);
            renderer.Submit2dPrimitive(line1);
            //renderer.Submit2dPrimitive(line2);
            renderer.Submit2dPrimitive(line3);

            for (int i = 0; i < 11; i++)
            {
                auto line3 = Primitive2d::CreateLine(Vector2i(39,  82 + (i * 16)),
                                                     Vector2i(200, 82 + (i * 16)),
                                                     Color::From8Bit(176, 176, 176),
                                                     Color::From8Bit(160, 128, 64),
                                                     0);
                auto quadB0 = Primitive2d::CreateQuad(
                                                    Vector2i(52, 69 + (i * 16)),
                                                    Vector2i(52, 81 + (i * 16)),
                                                    Vector2i(40, 69 + (i * 16)),
                                                    Vector2i(40, 81 + (i * 16)),
                                                    Color::From8Bit(255, 255, 255),
                                                    Color::From8Bit(160, 128, 64),
                                                    Color::From8Bit(160, 128, 64),
                                                    Color::From8Bit(255, 255, 255),
                                                    0);
                auto quadB1 = Primitive2d::CreateQuad(
                                                    Vector2i(39, 68 + (i * 16)),
                                                    Vector2i(39, 82 + (i * 16)),
                                                    Vector2i(53, 68 + (i * 16)),
                                                    Vector2i(53, 82 + (i * 16)),
                                                    Color::From8Bit(255, 255, 255),
                                                    Color::From8Bit(160, 128, 64),
                                                    Color::From8Bit(160, 128, 64),
                                                    Color::From8Bit(64,  64,  64),
                                                    1);
                renderer.Submit2dPrimitive(line3);
                renderer.Submit2dPrimitive(quadB1);
                renderer.Submit2dPrimitive(quadB0);
            }

            auto tri = Primitive2d::CreateTriangle(
                                                   Vector2i(8, 84),
                                                   Vector2i(16, 76),
                                                   Vector2i(16, 92),
                                                   Color::From8Bit(48, 255, 255),
                                                   Color::From8Bit(48, 255, 128),
                                                   Color::From8Bit(48, 255, 128),
                                                   0);
            renderer.Submit2dPrimitive(tri);
        }
    }

    void Initialize()
    {
        constexpr char LOG_FILENAME[]   = "Log.txt";
        constexpr char IMGUI_FILENAME[] = "imgui.ini";

        const auto& fs = g_App.GetFilesystem();

        // Set file and console log targets.
        auto path        = fs.GetAppDirectory() / LOG_FILENAME;
        auto fileSink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path.string(), true); // `true` = create new log at launch.
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto logger      = std::make_shared<spdlog::logger>(LOGGER_NAME, spdlog::sinks_init_list{ fileSink, consoleSink });

        // Initialize logger.
        spdlog::initialize_logger(logger);
        logger->set_level(spdlog::level::info);
        logger->flush_on(spdlog::level::info);
        logger->set_pattern("[%Y-%b-%d %T] [%^%l%$] %v");

        // Initialize `ImGui`.
        ImGui::CreateContext();
        auto imguiPath             = (fs.GetWorkDirectory() / IMGUI_FILENAME).string();
        ImGui::GetIO().IniFilename = CopyString(imguiPath.c_str(), imguiPath.size());

        // Reserve `Messages` size.
        Messages.reserve(MESSAGE_COUNT_MAX);
    }

    void Deinitialize()
    {
        spdlog::shutdown();
    }

    void Update()
    {
        Scratchpad();

        // If debug GUI is disabled, return early.
        const auto& options = g_App.GetOptions();
        if (!options->EnableDebugGui)
        {
            //Messages.clear();
            return;
        }

        // Create debug GUI.
        CreateGui([]()
        {
            constexpr const char* FRAME_RATE_ITEMS[]        = { "30", "60", "120", "Unlimited" };
            constexpr const char* RENDER_SCALE_ITEMS[]      = { "Native", "Half", "Quarter", "Retro" };
            constexpr const char* ASPECT_RATIO_ITEMS[]      = { "Native", "Widescreen", "Retro" };
            constexpr const char* TEX_FILTER_ITEMS[]        = { "Nearest", "Linear" };
            constexpr const char* LIGHTING_ITEMS[]          = { "Per vertex", "Per pixel" };
            constexpr const char* LANG_ITEMS[]              = { "English" };
            constexpr const char* SOUND_ITEMS[]             = { "Stereo", "Monaural" };
            constexpr const char* BLOOD_COLOR_ITEMS[]       = { "Normal", "Green", "Violet", "Black" };
            constexpr const char* CONTROL_INVERSION_ITEMS[] = { "Normal", "Reverse" };
            constexpr const char* WEAPON_CONTROL_ITEMS[]    = { "Switch", "Press" };
            constexpr const char* VIEW_MODE_ITEMS[]         = { "Normal", "Self view" };
            constexpr const char* DIALOG_PAUSE_ITEMS[]      = { "Classic", "Short" };

            const auto& assets   = g_App.GetAssets();
            auto&       options  = g_App.GetOptions();
            auto&       renderer = g_App.GetRenderer();

            // Main tabs.
            if (ImGui::BeginTabBar("MainTabs", ImGuiTabBarFlags_FittingPolicyScroll))
            {
                if constexpr (IS_DEBUG_BUILD)
                {
                    // `Scratchpad` tab.
                    if (ImGui::BeginTabItem("Scratchpad"))
                    {
                        // 'Alpha blend' slider.
                        ImGui::SliderFloat("Alpha Blend", &g_Work.BlendAlpha, 0.0f, 1.0f);
    
                        ImGui::EndTabItem();
                    }
                }

                // `Resources` tab.
                if (ImGui::BeginTabItem("Resources"))
                {
                    // `Loaded Assets` section.
                    ImGui::SeparatorText("Loaded Assets");
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
                        if (ImGui::BeginChild("LoadedAssetsBox",
                                              ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8),
                                              ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY))
                        {
                            auto assetNames = assets.GetLoadedAssetNames();
                            for (const auto& assetName : assetNames)
                            {
                                ImGui::Bullet();
                                ImGui::TextWrapped(assetName.c_str());
                            }
                        }
                        ImGui::PopStyleColor();
                        ImGui::EndChild();
                    }

                    ImGui::EndTabItem();
                }

                // `Renderer` tab.
                if (ImGui::BeginTabItem("Renderer"))
                {
                    g_Work.Page = Page::Renderer;

                    // `Draw calls` info.
                    /*ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Draw calls:", 1, 0);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", renderer.GetDrawCallCount(), 1, 1);*/

                    // `Wireframe mode` checkbox.
                    ImGui::Checkbox("Wireframe mode", &g_Work.EnableWireframeMode);

                    ImGui::EndTabItem();
                }
                
                // `Input` tab.
                if (ImGui::BeginTabItem("Input"))
                {
                    g_Work.Page = Page::Input;

                    const auto& input = g_App.GetInput();

                    // `Status` section.
                    ImGui::SeparatorText("Status");
                    {
                        if (ImGui::BeginTable("Status", 2))
                        {
                            // `Gamepad` enabled/disabled info.
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("Gamepad:", 0, 0);
                            ImGui::TableSetColumnIndex(1);
                            input.IsGamepadConnected() ? ImGui::Text("Connected", 0, 1) : ImGui::Text("Not connected", 0, 1);

                            // `Cursor` info.
                            const auto& cursorPos = input.GetCursorPosition();
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("Cursor:", 1, 0);
                            ImGui::TableSetColumnIndex(1);
                            ImGui::Text("(%.2f, %.2f)", cursorPos.x, cursorPos.y, 1, 1);

                            ImGui::EndTable();
                        }
                    }

                    // `Actions` section.
                    ImGui::SeparatorText("Actions");
                    {
                        const auto& input = g_App.GetInput();

                        // Collect action statuses.
                        int  flagCount           = (int)ACTION_ID_GROUPS.at(USER_ACTION_GROUP_IDS.back()).back();
                        auto clickedActionFlags  = Bitfield(flagCount);
                        auto heldActionFlags     = Bitfield(flagCount);
                        auto releasedActionFlags = Bitfield(flagCount);
                        for (int i = 0; i < flagCount; i++)
                        {
                            auto actionId = (ActionId)i;
                            const auto& action = input.GetAction(actionId);

                            if (action.IsClicked())
                            {
                                clickedActionFlags.Set(i);
                            }
                            if (action.IsHeld())
                            {
                                heldActionFlags.Set(i);
                            }
                            if (action.IsReleased())
                            {
                                releasedActionFlags.Set(i);
                            }
                        }

                        // @todo Using `c_str` crashes.

                        // `Clicked` info.
                        /*ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("Clicked:", 0, 0);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text(clickedActionFlags.ToString().c_str(), 0, 1);*/

                        // `Held` info.
                        /*ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("Held:", 1, 0);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text(heldActionFlags.ToString().c_str(), 1, 1);*/

                        // `Released` info.
                        /*ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("Released:", 2, 0);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text(releasedActionFlags.ToString().c_str(), 2, 1);*/
                    }

                    // `Analog Axes` section.
                    ImGui::SeparatorText("Analog Axes");
                    {
                        if (ImGui::BeginTable("Analog Axes", 2))
                        {
                            // `Move axis` info.
                            const auto& moveAxis = input.GetAnalogAxis(AnalogAxisId::Move);
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("Move:", 0, 0);
                            ImGui::TableSetColumnIndex(1);
                            ImGui::Text("(%.2f, %.2f)", moveAxis.x, moveAxis.y, 0, 1);

                            // `Camera axis` info.
                            const auto& camAxis = input.GetAnalogAxis(AnalogAxisId::Camera);
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("Camera:", 1, 0);
                            ImGui::TableSetColumnIndex(1);
                            ImGui::Text("(%.2f, %.2f)", camAxis.x, camAxis.y, 1, 1);

                            // `Mouse axis` info.
                            const auto& mouseAxis = input.GetAnalogAxis(AnalogAxisId::Mouse);
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("Mouse:", 2, 0);
                            ImGui::TableSetColumnIndex(1);
                            ImGui::Text("(%.2f, %.2f)", mouseAxis.x, mouseAxis.y, 2, 1);

                            // `Left stick axis` info.
                            const auto& leftStickAxis = input.GetAnalogAxis(AnalogAxisId::StickLeft);
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("Left stick:", 3, 0);
                            ImGui::TableSetColumnIndex(1);
                            ImGui::Text("(%.2f, %.2f)", leftStickAxis.x, leftStickAxis.y, 3, 1);

                            // `Right stick axis` info.
                            const auto& rightStickAxis = input.GetAnalogAxis(AnalogAxisId::StickRight);
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("Right stick:", 4, 0);
                            ImGui::TableSetColumnIndex(1);
                            ImGui::Text("(%.2f, %.2f)", rightStickAxis.x, rightStickAxis.y, 4, 1);

                            ImGui::EndTable();
                        }
                    }

                    ImGui::EndTabItem();
                }

                // `Cheats` tab.
                if (ImGui::BeginTabItem("Cheats"))
                {
                    g_Work.Page = Page::Cheats;

                    // `Freeze mode` checkbox.
                    ImGui::Checkbox("Freeze mode", &g_Work.EnableFreezeMode);

                    ImGui::EndTabItem();
                }

                // `Options` tab.
                if (ImGui::BeginTabItem("Options"))
                {
                    g_Work.Page  = Page::Options;
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

                        // `Render scale` combo.
                        int renderScale = (int)options->RenderScale;
                        if (ImGui::Combo("Render scale", &renderScale, RENDER_SCALE_ITEMS, IM_ARRAYSIZE(RENDER_SCALE_ITEMS)))
                        {
                            options->RenderScale = (RenderScaleType)renderScale;
                            isOptChanged         = true;
                        }

                        // `Aspect ratio` combo.
                        int aspectRatio = (int)options->AspectRatio;
                        if (ImGui::Combo("Aspect ratio", &aspectRatio, ASPECT_RATIO_ITEMS, IM_ARRAYSIZE(ASPECT_RATIO_ITEMS)))
                        {
                            options->AspectRatio = (AspectRatioType)aspectRatio;
                            isOptChanged         = true;
                        }

                        // `Texture filter` combo.
                        int texFilter = (int)options->TextureFilter;
                        if (ImGui::Combo("Texture filter", &texFilter, TEX_FILTER_ITEMS, IM_ARRAYSIZE(TEX_FILTER_ITEMS)))
                        {
                            options->TextureFilter = (TextureFilterType)texFilter;
                            isOptChanged = true;
                        }

                        // `Lighting type` combo.
                        int lighting = (int)options->Lighting;
                        if (ImGui::Combo("Lighting", &lighting, LIGHTING_ITEMS, IM_ARRAYSIZE(LIGHTING_ITEMS)))
                        {
                            options->Lighting = (LightingType)lighting;
                            isOptChanged      = true;
                        }

                        // `Enable dithering` checkbox.
                        if (ImGui::Checkbox("Enable dithering", &options->EnableDithering))
                        {
                            isOptChanged = true;
                        }

                        // `Enable CRT filter` checkbox.
                        if (ImGui::Checkbox("Enable CRT filter", &options->EnableCrtFilter))
                        {
                            isOptChanged = true;
                        }

                        // `Enable vignette` checkbox.
                        if (ImGui::Checkbox("Enable vignette", &options->EnableVignette))
                        {
                            isOptChanged = true;
                        }

                        // `Enable vertex jitter` checkbox.
                        if (ImGui::Checkbox("Enable vertex jitter", &options->EnableVertexJitter))
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

                        // `Language` combo.
                        int lang = (int)options->Language;
                        if (ImGui::Combo("Language", &lang, LANG_ITEMS, IM_ARRAYSIZE(LANG_ITEMS)))
                        {
                            options->Language = (LanguageType)lang;
                            isOptChanged      = true;
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
                            isOptChanged           = true;
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
                ImGui::EndTabBar();
            }

            // `Messages` section.
            ImGui::SeparatorText("Messages");
            {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
                if (ImGui::BeginChild("MessageBox",
                                      ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8),
                                      ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY))
                {
                    for (const auto& msg : Messages)
                    {
                        ImGui::Bullet();
                        ImGui::TextWrapped(msg.c_str());
                    }

                    // TEMP
                    ImGui::Bullet();
                    ImGui::TextWrapped(g_App.GetInput().GetText("Test").c_str());
                    ImGui::Bullet();
                    ImGui::TextWrapped(std::to_string(g_App.GetInput().GetTextCursorPosition("Test")).c_str());
                }
                ImGui::PopStyleColor();
                ImGui::EndChild();
            }
        });

        // Create demo GUI.
        /*CreateGui([]()
        {
            ImGui::ShowDemoWindow();
        });*/

        Messages.clear();
    }

    // @todo Not working.
    void Message(const char* msg, ...)
    {
        constexpr uint BUFFER_SIZE = 255;

        // Check if debug GUI is enabled.
        const auto& options = g_App.GetOptions();
        if (!options->EnableDebugGui)
        {
            return;
        }

        // Check if `Messages` is full.
        if (Messages.size() >= MESSAGE_COUNT_MAX)
        {
            Log("Attempted to create too many debug messages.", LogLevel::Warning, LogMode::Debug);
            return;
        }

        // Initialize string buffer.
        char buffer[BUFFER_SIZE];
        std::memset(buffer, 0, BUFFER_SIZE);

        // Format string.
        va_list args;
        va_start(args, msg);
        vsnprintf(buffer, BUFFER_SIZE, msg, args);
        va_end(args);

        // @lock Restrict `Messages` access.
        static auto mutex = std::mutex();
        {
            auto lock = std::lock_guard(mutex);

            // Add message.
            Messages.push_back(buffer);
        }
    }

    void Log(const std::string& msg, LogLevel level, LogMode mode, bool repeat)
    {
        // Ignore debug mode logs in release build.
        if constexpr (!IS_DEBUG_BUILD)
        {
            if (mode == LogMode::Debug)
            {
                return;
            }
        }

        // @lock Restrict logger access.
        static auto mutex = std::mutex();
        {
            auto lock = std::lock_guard(mutex);

            static auto prevMsg = std::string();
            if (prevMsg == msg && !repeat)
            {
                return;
            }
            prevMsg = msg;

            // Get logger instance.
            auto logger = spdlog::get(LOGGER_NAME);
            if (logger == nullptr)
            {
                return;
            }

            // Log message.
            switch (level)
            {
                default:
                case LogLevel::Info:
                {
                    logger->info(msg);
                    break;
                }
                case LogLevel::Warning:
                {
                    logger->warn(msg);
                    break;
                }
                case LogLevel::Error:
                {
                    logger->error(msg);
                    break;
                }
                case LogLevel::Critical:
                {
                    logger->critical(msg);
                    break;
                }
            }
            logger->flush();
        }
    }

    void Assert(bool cond, const std::string& msg)
    {
        if constexpr (IS_DEBUG_BUILD)
        {
            if (!cond)
            {
                throw std::runtime_error("Assertion failed. " + msg);
            }
        }
    };

    void StartTimer()
    {
        if constexpr (IS_DEBUG_BUILD)
        {
            StartTime = std::chrono::high_resolution_clock::now();
        }
    }

    void EndTimer()
    {
        if constexpr (IS_DEBUG_BUILD)
        {
            auto endTime  = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - StartTime);
            Message("Execution (μs): %d", duration.count());
        }
    }

    void CreateGui(std::function<void()> drawFunc)
    {
        auto& renderer = g_App.GetRenderer();
        renderer.SubmitDebugGui(drawFunc);
    }

    void CreateLine(const Vector3& from, const Vector3& to, const Color& color, Page page)
    {
        auto& renderer = g_App.GetRenderer();
        renderer.SubmitDebugLine(from, to, color, page);
    }

    void CreateTriangle(const Vector3& vert0, const Vector3& vert1, const Vector3& vert2, const Color& color, Page page)
    {
        auto& renderer = g_App.GetRenderer();
        renderer.SubmitDebugTriangle(vert0, vert1, vert2, color, page);
    }

    void CreateTarget(const Vector3& center, const Quaternion& rot, float radius, const Color& color, Page page)
    {
        auto& renderer = g_App.GetRenderer();
        renderer.SubmitDebugTarget(center, rot, radius, color, page);
    }

    void CreateBox(const OrientedBoundingBox& obb, const Color& color, bool isWireframe, Page page)
    {
        auto& renderer = g_App.GetRenderer();
        renderer.SubmitDebugBox(obb, color, isWireframe, page);
    }

    void CreateSphere(const BoundingSphere& sphere, const Color& color, bool isWireframe, Page page)
    {
        auto& renderer = g_App.GetRenderer();
        renderer.SubmitDebugSphere(sphere, color, isWireframe, page);
    }

    void CreateCylinder(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, Page page)
    {
        auto& renderer = g_App.GetRenderer();
        renderer.SubmitDebugCylinder(center, rot, radius, length, color, isWireframe, page);
    }

    void CreateCone(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, Page page)
    {
        auto& renderer = g_App.GetRenderer();
        renderer.SubmitDebugCone(center, rot, radius, length, color, isWireframe, page);
    }

    void CreateDebugDiamond(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, Page page)
    {
        auto& renderer = g_App.GetRenderer();
        renderer.SubmitDebugDiamond(center, rot, radius, length, color, isWireframe, page);
    }
}
