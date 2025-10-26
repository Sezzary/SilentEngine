#include "Framework.h"
#include "Services/Options.h"

#include "Application.h"
#include "Input/Input.h"
#include "Services/Filesystem.h"
#include "Utils/Parallel.h"
#include "Utils/Stream.h"

using namespace Silent::Input;
using namespace Silent::Utils;

namespace Silent::Services
{
    constexpr char OPTIONS_FILENAME[] = "Options";

    constexpr char KEY_GRAPHICS[]     = "Graphics";
    constexpr char KEY_GAMEPLAY[]     = "Gameplay";
    constexpr char KEY_INPUT[]        = "Input";
    constexpr char KEY_ENHANCEMENTS[] = "Enhancements";
    constexpr char KEY_SYSTEM[]       = "System";

    constexpr char KEY_WINDOWED_SIZE_X[]                          = "WindowedSizeX";
    constexpr char KEY_WINDOWED_SIZE_Y[]                          = "WindowedSizeY";
    constexpr char KEY_ENABLE_FULLSCREEN[]                        = "EnableFullscreen";
    constexpr char KEY_ENABLE_MAXIMIZED[]                         = "EnableMaximized";
    constexpr char KEY_BRIGHTNESS_LEVEL[]                         = "BrightnessLevel";
    constexpr char KEY_FRAME_RATE[]                               = "FrameRate";
    constexpr char KEY_RENDER_SCALE[]                             = "RenderScale";
    constexpr char KEY_ASPECT_RATIO[]                             = "AspectRatio";
    constexpr char KEY_TEXTURE_FILTER[]                           = "TextureFilter";
    constexpr char KEY_LIGHTING[]                                 = "Lighting";
    constexpr char KEY_ENABLE_VERTEX_JITTER[]                     = "EnableVertexJitter";
    constexpr char KEY_ENABLE_DITHERING[]                         = "EnableDithering";
    constexpr char KEY_ENABLE_VIGNETTE[]                          = "EnableVignette";
    constexpr char KEY_ENABLE_CRT_FILTER[]                        = "EnableCrtFilter";
    constexpr char KEY_ENABLE_AUTO_LOAD[]                         = "EnableAutoLoad";
    constexpr char KEY_ENABLE_SUBTITLES[]                         = "EnableSubtitles";
    constexpr char KEY_LANGUAGE[]                                 = "Language";
    constexpr char KEY_SOUND[]                                    = "Sound";
    constexpr char KEY_BGM_VOLUME[]                               = "BgmVolume";
    constexpr char KEY_SE_VOLUME[]                                = "SeVolume";
    constexpr char KEY_BLOOD_COLOR[]                              = "BloodColor";
    constexpr char KEY_BULLET_ADJUST[]                            = "BulletAdjust";
    constexpr char KEY_KEYBOARD_MOUSE_BINDINGS[]                  = "KeyboardMouseBindings";
    constexpr char KEY_ACTIVE_KEYBOARD_MOUSE_BINDING_PROFILE_ID[] = "ActiveKeyboardMouseBindingProfileId";
    constexpr char KEY_GAMEPAD_BINDINGS[]                         = "GamepadBindings";
    constexpr char KEY_ACTIVE_GAMEPAD_BINDING_PROFILE_ID[]        = "ActiveGamepadBindingProfileId";
    constexpr char KEY_ENABLE_VIBRATION[]                         = "EnableVibration";
    constexpr char KEY_MOUSE_SENSITIVITY[]                        = "MouseSensitivity";
    constexpr char KEY_WEAPON_CONTROL[]                           = "WeaponControl";
    constexpr char KEY_VIEW_CONTROL[]                             = "ViewControl";
    constexpr char KEY_RETREAT_TURN_CONTROL[]                     = "RetreatTurnControl";
    constexpr char KEY_WALK_RUN_CONTROL[]                         = "WalkRunControl";
    constexpr char KEY_DISABLE_AUTO_AIMING[]                      = "DisableAutoAiming";
    constexpr char KEY_VIEW_MODE[]                                = "ViewMode";
    constexpr char KEY_DIALOG_PAUSE[]                             = "DialogPause";
    constexpr char KEY_ENABLE_TOASTS[]                            = "EnableToasts";
    constexpr char KEY_ENABLE_PARALLELISM[]                       = "EnableParallelism";

    constexpr auto DEFAULT_WINDOWED_SIZE                            = Vector2i(800, 600);
    constexpr bool DEFAULT_ENABLE_MAXIMIZED                         = false;
    constexpr bool DEFAULT_ENABLE_FULLSCREEN                        = false;
    constexpr int  DEFAULT_BRIGHTNESS_LEVEL                         = 3;
    constexpr auto DEFAULT_FRAME_RATE                               = FrameRateType::Fps60;
    constexpr auto DEFAULT_RENDER_SCALE                             = RenderScaleType::Native;
    constexpr auto DEFAULT_ASPECT_RATIO                             = AspectRatioType::Native;
    constexpr auto DEFAULT_TEXTURE_FILTER                           = TextureFilterType::Nearest;
    constexpr auto DEFAULT_LIGHTING                                 = LightingType::PerVertex;
    constexpr bool DEFAULT_ENABLE_DITHERING                         = true;
    constexpr bool DEFAULT_ENABLE_CRT_FILTER                        = false;
    constexpr bool DEFAULT_ENABLE_VIGNETTE                          = false;
    constexpr bool DEFAULT_ENABLE_VERTEX_JITTER                     = false;
    constexpr bool DEFAULT_ENABLE_AUTO_LOAD                         = false;
    constexpr bool DEFAULT_ENABLE_SUBTITLES                         = true;
    constexpr auto DEFAULT_LANGUAGE                                 = LanguageType::English;
    constexpr auto DEFAULT_SOUND                                    = SoundType::Stereo;
    constexpr int  DEFAULT_BGM_VOLUME                               = 16;
    constexpr int  DEFAULT_SE_VOLUME                                = 16;
    constexpr auto DEFAULT_BLOOD_COLOR                              = BloodColorType::Normal;
    constexpr int  DEFAULT_BULLET_ADJUST                            = 1;
    constexpr auto DEFAULT_ACTIVE_KEYBOARD_MOUSE_BINDING_PROFILE_ID = BindingProfileId::KeyboardMouseType1;
    constexpr auto DEFAULT_ACTIVE_GAMEPAD_BINDING_PROFILE_ID        = BindingProfileId::GamepadType1;
    constexpr bool DEFAULT_ENABLE_VIBRATION                         = true;
    constexpr int  DEFAULT_MOUSE_SENSITIVITY                        = 6;
    constexpr auto DEFAULT_WEAPON_CONTROL                           = WeaponControlType::Press;
    constexpr auto DEFAULT_VIEW_CONTROL                             = ControlInversionType::Normal;
    constexpr auto DEFAULT_RETREAT_TURN_CONTROL                     = ControlInversionType::Normal;
    constexpr auto DEFAULT_WALK_RUN_CONTROL                         = ControlInversionType::Normal;
    constexpr bool DEFAULT_DISABLE_AUTO_AIMING                      = false;
    constexpr auto DEFAULT_DIALOG_PAUSE                             = DialogPauseType::Classic;
    constexpr auto DEFAULT_VIEW_MODE                                = ViewMode::Normal;
    constexpr bool DEFAULT_ENABLE_TOASTS                            = true;

    const std::unordered_map<LanguageType, std::string> LOCALE_NAMES =
    {
        { LanguageType::English, "English" }
    };

    void OptionsManager::SetDefaultGraphicsOptions()
    {
        _options.WindowedSize       = DEFAULT_WINDOWED_SIZE;
        _options.EnableMaximized    = DEFAULT_ENABLE_MAXIMIZED;
        _options.EnableFullscreen   = DEFAULT_ENABLE_FULLSCREEN;
        _options.BrightnessLevel    = DEFAULT_BRIGHTNESS_LEVEL;
        _options.FrameRate          = DEFAULT_FRAME_RATE;
        _options.RenderScale        = DEFAULT_RENDER_SCALE;
        _options.AspectRatio        = DEFAULT_ASPECT_RATIO;
        _options.TextureFilter      = DEFAULT_TEXTURE_FILTER;
        _options.Lighting           = DEFAULT_LIGHTING;
        _options.EnableVertexJitter = DEFAULT_ENABLE_VERTEX_JITTER;
        _options.EnableDithering    = DEFAULT_ENABLE_DITHERING;
        _options.EnableVignette     = DEFAULT_ENABLE_VIGNETTE;
        _options.EnableCrtFilter    = DEFAULT_ENABLE_CRT_FILTER;
    }

    void OptionsManager::SetDefaultGameplayOptions()
    {
        _options.EnableAutoLoad  = DEFAULT_ENABLE_AUTO_LOAD;
        _options.EnableSubtitles = DEFAULT_ENABLE_SUBTITLES;
        _options.Language        = DEFAULT_LANGUAGE;
        _options.Sound           = DEFAULT_SOUND;
        _options.BgmVolume       = DEFAULT_BGM_VOLUME;
        _options.SeVolume        = DEFAULT_SE_VOLUME;
        _options.BloodColor      = DEFAULT_BLOOD_COLOR;
        _options.BulletAdjust    = DEFAULT_BULLET_ADJUST;
    }

    void OptionsManager::SetDefaultInputKmBindingsOptions()
    {
        _options.KeyboardMouseBindings = USER_KEYBOARD_MOUSE_BINDING_PROFILE_TYPE_1;
    }

    void OptionsManager::SetDefaultInputGamepadCustomBindingOptions()
    {
        _options.GamepadBindings = USER_GAMEPAD_BINDING_PROFILE_TYPE_1;
    }

    void OptionsManager::SetDefaultInputControlsOptions()
    {
        _options.EnableVibration    = DEFAULT_ENABLE_VIBRATION;
        _options.MouseSensitivity   = DEFAULT_MOUSE_SENSITIVITY;
        _options.WeaponControl      = DEFAULT_WEAPON_CONTROL;
        _options.ViewControl        = DEFAULT_VIEW_CONTROL;
        _options.RetreatTurnControl = DEFAULT_RETREAT_TURN_CONTROL;
        _options.WalkRunControl     = DEFAULT_WALK_RUN_CONTROL;
        _options.DisableAutoAiming  = DEFAULT_DISABLE_AUTO_AIMING;
        _options.ViewMode           = DEFAULT_VIEW_MODE;
    }

    void OptionsManager::SetDefaultEnhancementsOptions()
    {
        _options.DialogPause = DEFAULT_DIALOG_PAUSE;
    }

    void OptionsManager::SetDefaultSystemOptions()
    {
        _options.EnableToasts      = DEFAULT_ENABLE_TOASTS;
        _options.EnableParallelism = GetCoreCount() > 1;
    }

    void OptionsManager::Initialize()
    {
        _options.EnableDebugMode = Debug::IS_DEBUG_BUILD;
        _options.EnableDebugGui  = false;
        SetDefaultOptions();
    }

    void OptionsManager::Save()
    {
        const auto& fs = g_App.GetFilesystem();

        // Create options JSON.
        auto optionsJson = ToOptionsJson(_options);

        // Write options JSON file.
        auto stream = Stream(fs.GetWorkDirectory() / (std::string(OPTIONS_FILENAME) + JSON_FILE_EXT), false, true);
        stream.WriteJson(optionsJson);
        stream.Close();
    }

    void OptionsManager::Load()
    {
        const auto& fs = g_App.GetFilesystem();
        
        // Open options JSON file.
        auto stream = Stream(fs.GetWorkDirectory() / (std::string(OPTIONS_FILENAME) + JSON_FILE_EXT), true, false);
        if (!stream.IsOpen())
        {
            Debug::Log("No options file found. Creating file.", Debug::LogLevel::Info);

            SetDefaultOptions();
            Save();
            return;
        }

        // Parse file into JSON object.
        auto optionsJson = stream.ReadJson();

        // Read options JSON.
        _options = FromOptionsJson(optionsJson);
    }

    void OptionsManager::SetDefaultOptions()
    {
        SetDefaultGraphicsOptions();
        SetDefaultGameplayOptions();

        SetDefaultInputKmBindingsOptions();
        _options.ActiveKeyboardMouseProfileId = DEFAULT_ACTIVE_KEYBOARD_MOUSE_BINDING_PROFILE_ID;

        SetDefaultInputGamepadCustomBindingOptions();
        _options.ActiveGamepadProfileId = DEFAULT_ACTIVE_GAMEPAD_BINDING_PROFILE_ID;

        SetDefaultInputControlsOptions();
        SetDefaultEnhancementsOptions();
        SetDefaultSystemOptions();
    }

    Options OptionsManager::FromOptionsJson(const json& optionsJson) const
    {
        auto options = Options{};

        // Load graphics options.
        const auto& graphicsJson   = optionsJson[KEY_GRAPHICS];
        options.WindowedSize.x     = graphicsJson.value(KEY_WINDOWED_SIZE_X,      DEFAULT_WINDOWED_SIZE.x);
        options.WindowedSize.y     = graphicsJson.value(KEY_WINDOWED_SIZE_Y,      DEFAULT_WINDOWED_SIZE.y);
        options.EnableMaximized    = graphicsJson.value(KEY_ENABLE_MAXIMIZED,     DEFAULT_ENABLE_MAXIMIZED);
        options.EnableFullscreen   = graphicsJson.value(KEY_ENABLE_FULLSCREEN,    DEFAULT_ENABLE_FULLSCREEN);
        options.BrightnessLevel    = graphicsJson.value(KEY_BRIGHTNESS_LEVEL,     DEFAULT_BRIGHTNESS_LEVEL);
        options.FrameRate          = graphicsJson.value(KEY_FRAME_RATE,           DEFAULT_FRAME_RATE);
        options.RenderScale        = graphicsJson.value(KEY_RENDER_SCALE,         DEFAULT_RENDER_SCALE);
        options.AspectRatio        = graphicsJson.value(KEY_ASPECT_RATIO,         DEFAULT_ASPECT_RATIO);
        options.TextureFilter      = graphicsJson.value(KEY_TEXTURE_FILTER,       DEFAULT_TEXTURE_FILTER);
        options.Lighting           = graphicsJson.value(KEY_LIGHTING,             DEFAULT_LIGHTING);
        options.EnableVertexJitter = graphicsJson.value(KEY_ENABLE_VERTEX_JITTER, DEFAULT_ENABLE_VERTEX_JITTER);
        options.EnableDithering    = graphicsJson.value(KEY_ENABLE_DITHERING,     DEFAULT_ENABLE_DITHERING);
        options.EnableVignette     = graphicsJson.value(KEY_ENABLE_VIGNETTE,      DEFAULT_ENABLE_VIGNETTE);
        options.EnableCrtFilter    = graphicsJson.value(KEY_ENABLE_CRT_FILTER,    DEFAULT_ENABLE_CRT_FILTER);

        // Load gameplay options.
        const auto& gameplayJson = optionsJson[KEY_GAMEPLAY];
        options.EnableAutoLoad   = gameplayJson.value(KEY_ENABLE_AUTO_LOAD, DEFAULT_ENABLE_AUTO_LOAD);
        options.EnableSubtitles  = gameplayJson.value(KEY_ENABLE_SUBTITLES, DEFAULT_ENABLE_SUBTITLES);
        options.Language         = gameplayJson.value(KEY_LANGUAGE,         DEFAULT_LANGUAGE);
        options.Sound            = gameplayJson.value(KEY_SOUND,            DEFAULT_SOUND);
        options.BgmVolume        = gameplayJson.value(KEY_BGM_VOLUME,       DEFAULT_BGM_VOLUME);
        options.SeVolume         = gameplayJson.value(KEY_SE_VOLUME,        DEFAULT_SE_VOLUME);
        options.BloodColor       = gameplayJson.value(KEY_BLOOD_COLOR,      DEFAULT_BLOOD_COLOR);
        options.BulletAdjust     = gameplayJson.value(KEY_BULLET_ADJUST,    DEFAULT_BULLET_ADJUST);

        // Load input options.
        const auto& inputJson                = optionsJson[KEY_INPUT];
        options.ActiveKeyboardMouseProfileId = inputJson.value(KEY_ACTIVE_KEYBOARD_MOUSE_BINDING_PROFILE_ID, DEFAULT_ACTIVE_KEYBOARD_MOUSE_BINDING_PROFILE_ID);
        options.ActiveGamepadProfileId       = inputJson.value(KEY_ACTIVE_GAMEPAD_BINDING_PROFILE_ID,        DEFAULT_ACTIVE_GAMEPAD_BINDING_PROFILE_ID);
        options.EnableVibration              = inputJson.value(KEY_ENABLE_VIBRATION,                         DEFAULT_ENABLE_VIBRATION);
        options.MouseSensitivity             = inputJson.value(KEY_MOUSE_SENSITIVITY,                        DEFAULT_MOUSE_SENSITIVITY);
        options.WeaponControl                = inputJson.value(KEY_WEAPON_CONTROL,                           DEFAULT_WEAPON_CONTROL);
        options.ViewControl                  = inputJson.value(KEY_VIEW_CONTROL,                             DEFAULT_VIEW_CONTROL);
        options.RetreatTurnControl           = inputJson.value(KEY_RETREAT_TURN_CONTROL,                     DEFAULT_RETREAT_TURN_CONTROL);
        options.WalkRunControl               = inputJson.value(KEY_WALK_RUN_CONTROL,                         DEFAULT_WALK_RUN_CONTROL);
        options.DisableAutoAiming            = inputJson.value(KEY_DISABLE_AUTO_AIMING,                      DEFAULT_DISABLE_AUTO_AIMING);
        options.ViewMode                     = inputJson.value(KEY_VIEW_MODE,                                DEFAULT_VIEW_MODE);

        // Load custom action-event bindings.
        const auto& kmBindsJson      = inputJson[KEY_KEYBOARD_MOUSE_BINDINGS];
        const auto& gamepadBindsJson = inputJson[KEY_GAMEPAD_BINDINGS];
        for (auto actionGroupId : USER_ACTION_GROUP_IDS)
        {
            const auto& actionIds = ACTION_ID_GROUPS.at(actionGroupId);
            for (auto actionId : actionIds)
            {
                auto actionStr = std::to_string((int)actionId);

                // Keyboard/mouse.
                if (kmBindsJson.contains(actionStr))
                {
                    const auto& eventsJson = kmBindsJson[actionStr];

                    auto events = std::vector<EventId>{};
                    events.reserve(eventsJson.size());
                    for (const auto& eventJson : eventsJson)
                    {
                        events.push_back((EventId)eventJson.get<int>());
                    }

                    options.KeyboardMouseBindings[actionId] = !events.empty() ? std::move(events) : USER_KEYBOARD_MOUSE_BINDING_PROFILE_TYPE_1.at(actionId);
                }
                else
                {
                    options.KeyboardMouseBindings[actionId] = USER_KEYBOARD_MOUSE_BINDING_PROFILE_TYPE_1.at(actionId);
                }

                // Gamepad.
                if (gamepadBindsJson.contains(actionStr))
                {
                    const auto& eventsJson = gamepadBindsJson[actionStr];

                    auto events = std::vector<EventId>{};
                    events.reserve(eventsJson.size());
                    for (const auto& eventJson : eventsJson)
                    {
                        events.push_back((EventId)eventJson.get<int>());
                    }

                    options.GamepadBindings[actionId] = !events.empty() ? std::move(events) : USER_GAMEPAD_BINDING_PROFILE_TYPE_1.at(actionId);
                }
                else
                {
                    options.GamepadBindings[actionId] = USER_GAMEPAD_BINDING_PROFILE_TYPE_1.at(actionId);
                }
            }
        }

        // Load enhancements options.
        const auto& enhancementsJson = optionsJson[KEY_ENHANCEMENTS];
        options.DialogPause          = enhancementsJson.value(KEY_DIALOG_PAUSE, DEFAULT_DIALOG_PAUSE);

        // Load system options.
        const auto& systemJson    = optionsJson[KEY_SYSTEM];
        options.EnableToasts      = systemJson.value(KEY_ENABLE_TOASTS, DEFAULT_ENABLE_TOASTS);
        options.EnableParallelism = systemJson.value(KEY_ENABLE_PARALLELISM, GetCoreCount() > 1);

        return options;
    }

    const Options* OptionsManager::operator->() const
    {
        return &_options;
    }

    Options* OptionsManager::operator->()
    {
        return &_options;
    }

    json OptionsManager::ToOptionsJson(const Options& options) const
    {
        // Create keyboard/mouse action-event bindings JSON.
        auto kmBindsJson = json();
        for (const auto& [actionId, eventIds] : options.KeyboardMouseBindings)
        {
            auto eventsJson = json::array();
            for (const auto& eventId : eventIds)
            {
                eventsJson.push_back(eventId);
            }

            kmBindsJson[std::to_string((int)actionId)] = eventsJson;
        }

        // Create gamepad action-event bindings JSON.
        auto gamepadBindsJson = json();
        for (const auto& [actionId, eventIds] : options.KeyboardMouseBindings)
        {
            auto eventsJson = json::array();
            for (const auto& eventId : eventIds)
            {
                eventsJson.push_back(eventId);
            }

            gamepadBindsJson[std::to_string((int)actionId)] = eventsJson;
        }

        // Create options JSON.
        return json
        {
            {
                KEY_GRAPHICS,
                {
                    { KEY_WINDOWED_SIZE_X,      options.WindowedSize.x     },
                    { KEY_WINDOWED_SIZE_Y,      options.WindowedSize.y     },
                    { KEY_ENABLE_MAXIMIZED,     options.EnableMaximized    },
                    { KEY_ENABLE_FULLSCREEN,    options.EnableFullscreen   },
                    { KEY_BRIGHTNESS_LEVEL,     options.BrightnessLevel    },
                    { KEY_FRAME_RATE,           options.FrameRate          },
                    { KEY_RENDER_SCALE,         options.RenderScale        },
                    { KEY_ASPECT_RATIO,         options.AspectRatio        },
                    { KEY_TEXTURE_FILTER,       options.TextureFilter      },
                    { KEY_LIGHTING,             options.Lighting           },
                    { KEY_ENABLE_VERTEX_JITTER, options.EnableVertexJitter },
                    { KEY_ENABLE_DITHERING,     options.EnableDithering    },
                    { KEY_ENABLE_VIGNETTE,      options.EnableVignette     },
                    { KEY_ENABLE_CRT_FILTER,    options.EnableCrtFilter    }
                }
            },
            {
                KEY_GAMEPLAY,
                {
                    { KEY_ENABLE_AUTO_LOAD, options.EnableAutoLoad  },
                    { KEY_ENABLE_SUBTITLES, options.EnableSubtitles },
                    { KEY_LANGUAGE,         options.Language        },
                    { KEY_SOUND,            options.Sound           },
                    { KEY_BGM_VOLUME,       options.BgmVolume       },
                    { KEY_SE_VOLUME,        options.SeVolume        },
                    { KEY_BLOOD_COLOR,      options.BloodColor      },
                    { KEY_BULLET_ADJUST,    options.BulletAdjust    }
                }
            },
            {
                KEY_INPUT,
                {
                    { KEY_KEYBOARD_MOUSE_BINDINGS,                  kmBindsJson                          },
                    { KEY_ACTIVE_KEYBOARD_MOUSE_BINDING_PROFILE_ID, options.ActiveKeyboardMouseProfileId },
                    { KEY_GAMEPAD_BINDINGS,                         gamepadBindsJson                     },
                    { KEY_ACTIVE_GAMEPAD_BINDING_PROFILE_ID,        options.ActiveGamepadProfileId       },
                    { KEY_ENABLE_VIBRATION,                         options.EnableVibration              },
                    { KEY_MOUSE_SENSITIVITY,                        options.MouseSensitivity             },
                    { KEY_WEAPON_CONTROL,                           options.WeaponControl                },
                    { KEY_VIEW_CONTROL,                             options.ViewControl                  },
                    { KEY_RETREAT_TURN_CONTROL,                     options.RetreatTurnControl           },
                    { KEY_WALK_RUN_CONTROL,                         options.WalkRunControl               },
                    { KEY_DISABLE_AUTO_AIMING,                      options.DisableAutoAiming            },
                    { KEY_VIEW_MODE,                                options.ViewMode                     }
                }
            },
            {
                KEY_ENHANCEMENTS,
                {
                    { KEY_DIALOG_PAUSE, options.DialogPause }
                }
            },
            {
                KEY_SYSTEM,
                {
                    { KEY_ENABLE_TOASTS, options.EnableToasts }
                }
            }
        };
    }
}
