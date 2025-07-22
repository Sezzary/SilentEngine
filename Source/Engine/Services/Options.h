#pragma once

#include "Engine/Services/Input/Binding.h"

using namespace Silent::Input;

namespace Silent::Services
{
    constexpr int BRIGHTNESS_LEVEL_MAX  = 7;
    constexpr int SOUND_VOLUME_MAX      = 128;
    constexpr int BULLET_ADJUST_MIN     = 1;
    constexpr int BULLET_ADJUST_MAX     = 6;
    constexpr int MOUSE_SENSITIVITY_MAX = 20;

    enum class FrameRateType
    {
        Fps30,
        Fps60,
        Fps120,
        Unlimited
    };

    enum class RenderScaleType
    {
        Native,
        Half,
        Quarter,
        Retro
    };

    enum class AspectRatioType
    {
        Native,
        Widescreen,
        Retro
    };

    enum class TextureFilterType
    {
        Nearest,
        Bilinear
    };

    enum class LightingType
    {
        PerVertex,
        PerPixel
    };

    enum class SoundType
    {
        Stereo,
        Monaural
    };

    enum class BloodColorType
    {
        Normal = 0,
        Green  = 2,
        Violet = 5,
        Black  = 13
    };

    enum class ControlInversionType
    {
        Normal,
        Reverse
    };

    enum class WeaponControlType
    {
        Switch,
        Press
    };

    enum class ViewMode
    {
        Normal,
        SelfView
    };

    struct Options
    {
        // Graphics (internal)

        Vector2i WindowedSize    = Vector2i::Zero;
        bool     EnableMaximized = false;

        // Graphics (user)

        bool              EnableFullscreen   = false;
        int               BrightnessLevel    = 0;
        FrameRateType     FrameRate          = FrameRateType::Fps30;
        RenderScaleType   RenderScale        = RenderScaleType::Native;
        AspectRatioType   AspectRatio        = AspectRatioType::Native;
        TextureFilterType TextureFilter      = TextureFilterType::Nearest;
        LightingType      Lighting           = LightingType::PerVertex;
        bool              EnableDithering    = false;
        bool              EnableCrtFilter    = false;
        bool              EnableVertexJitter = false;

        // Gameplay

        bool           EnableAutoLoad  = false;
        bool           EnableSubtitles = false;
        SoundType      Sound           = SoundType::Stereo;
        int            BgmVolume       = 0;
        int            SeVolume        = 0;
        BloodColorType BloodColor      = BloodColorType::Normal;
        int            BulletAdjust    = 0;

        // TODO: Enhancements section?

        // Input (keyboard/mouse bindings)

        BindingProfile   KeyboardMouseBindings        = {};
        BindingProfileId ActiveKeyboardMouseProfileId = BindingProfileId::KeyboardMouseType1;

        // Input (gamepad bindings)

        BindingProfile   GamepadBindings        = {};
        BindingProfileId ActiveGamepadProfileId = BindingProfileId::GamepadType1;

        // Input (controls)

        bool                 EnableVibration    = false;
        int                  MouseSensitivity   = 0;
        WeaponControlType    WeaponControl      = WeaponControlType::Switch;
        ControlInversionType ViewControl        = ControlInversionType::Normal;
        ControlInversionType RetreatTurnControl = ControlInversionType::Normal;
        ControlInversionType WalkRunControl     = ControlInversionType::Normal;
        bool                 DisableAutoAiming  = false;
        ViewMode             ViewMode           = ViewMode::Normal;

        // System (internal)

        bool EnableDebugMode = true;
        bool EnableDebugGui  = false;

        // System (user)

        bool EnableToasts      = false;
        bool EnableParallelism = false;
    };

    class OptionsManager
    {
    private:
        // Fields

        Options _options = {};

    public:
        // Constructors

        OptionsManager() = default;

        // Setters

        void SetDefaultGraphicsOptions();
        void SetDefaultGameplayOptions();
        void SetDefaultInputKmBindingsOptions();
        void SetDefaultInputGamepadCustomBindingOptions();
        void SetDefaultInputControlsOptions();
        void SetDefaultSystemOptions();

        // Utilities

        void Initialize();
        void Save();
        void Load();

        // Operators

        const Options* operator->() const;
        Options*       operator->();

    private:
        // Helpers

        void SetDefaultOptions();
        
        Options FromOptionsJson(const json& optionsJson) const;
        json    ToOptionsJson(const Options& options) const;
    };
}
