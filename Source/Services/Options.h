#pragma once

#include "Input/Input.h"

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
        Linear
    };

    enum class LightingType
    {
        PerVertex,
        PerPixel
    };

    enum class LanguageType
    {
        English
    };

    enum class SoundType
    {
        Stereo,
        Monaural
    };

    enum class BloodColorType
    {
        Normal,
        Green,
        Violet,
        Black
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
        Normal,  /** Third-person. */
        SelfView /** First-person. */
    };

    enum class DialogPauseType
    {
        Classic, /** Original pause length between dialog lines. */
        Short    /** Shorter pause length between dialog lines. */
    };

    /** @brief User options configuration data. */
    struct Options
    {
        /** Graphics (internal) */

        Vector2i WindowedSize    = Vector2i::Zero;
        bool     EnableMaximized = false;

        /** Graphics (user) */

        bool              EnableFullscreen   = false;
        int               BrightnessLevel    = 0;
        FrameRateType     FrameRate          = FrameRateType::Fps30;
        RenderScaleType   RenderScale        = RenderScaleType::Native;
        AspectRatioType   AspectRatio        = AspectRatioType::Native;
        TextureFilterType TextureFilter      = TextureFilterType::Nearest;
        LightingType      Lighting           = LightingType::PerVertex;
        bool              EnableVertexJitter = false;
        bool              EnableDithering    = false;
        bool              EnableVignette     = false;
        bool              EnableCrtFilter    = false;

        /** Gameplay */

        bool           EnableAutoLoad  = false;
        bool           EnableSubtitles = false;
        LanguageType   Language        = LanguageType::English;
        SoundType      Sound           = SoundType::Stereo;
        int            BgmVolume       = 0;
        int            SeVolume        = 0;
        BloodColorType BloodColor      = BloodColorType::Normal;
        int            BulletAdjust    = 0;

        /** Input (keyboard/mouse bindings) */

        BindingProfile   KeyboardMouseBindings        = {};
        BindingProfileId ActiveKeyboardMouseProfileId = BindingProfileId::KeyboardMouseType1;

        /** Input (gamepad bindings) */

        BindingProfile   GamepadBindings        = {};
        BindingProfileId ActiveGamepadProfileId = BindingProfileId::GamepadType1;

        /** Input (controls) */

        bool                 EnableVibration    = false;
        int                  MouseSensitivity   = 0;
        WeaponControlType    WeaponControl      = WeaponControlType::Switch;
        ControlInversionType ViewControl        = ControlInversionType::Normal;
        ControlInversionType RetreatTurnControl = ControlInversionType::Normal;
        ControlInversionType WalkRunControl     = ControlInversionType::Normal;
        bool                 DisableAutoAiming  = false;
        ViewMode             ViewMode           = ViewMode::Normal;

        /** Enhancements */

        DialogPauseType DialogPause = DialogPauseType::Classic;

        /** System (internal) */

        bool EnableDebugMode = true;
        bool EnableDebugGui  = false;
        
        /** System (user) */
        
        bool EnableToasts      = false;
        bool EnableParallelism = false;
    };

    /** @brief Central manager for the user options configuration. */
    class OptionsManager
    {
    private:
        // =======
        // Fields
        // =======

        Options _options = {}; /** Options configuration data. */

    public:
        // =============
        // Constructors
        // =============

        /** @brief Constructs an uninitialized default `OptionsManager`. */
        OptionsManager() = default;

        // ========
        // Setters
        // ========

        /** @brief Sets all graphics options to defaults. */
        void SetDefaultGraphicsOptions();

        /** @brief Sets all gameplay options to defaults. */
        void SetDefaultGameplayOptions();

        /** @brief Sets all keyboard and mouse input binding options to defaults. */
        void SetDefaultInputKmBindingsOptions();

        /** @brief Sets all gamepad input binding options to defaults. */
        void SetDefaultInputGamepadCustomBindingOptions();

        /** @brief Sets all control input options to defaults. */
        void SetDefaultInputControlsOptions();

        /** @brief Sets all enhancements options to defaults. */
        void SetDefaultEnhancementsOptions();

        /** @brief Sets all system options, excluding debug, to defaults. */
        void SetDefaultSystemOptions();

        // ==========
        // Utilities
        // ==========

        /** @brief Initializes the options configurations to startup defaults, taking the build mode into account. */
        void Initialize();

        /** @brief Saves the current options configuration to a JSON file on the user system's game work folder. */
        void Save();

        /** @brief Loads the options configuration from a JSON file on the user system's game work folder. */
        void Load();

        // ==========
        // Operators
        // ==========

        const Options* operator->() const; /** Accesses the internal options object. */
        Options*       operator->();       /** Accesses the internal options object. */

    private:
        // ========
        // Helpers
        // ========

        /** @brief Resets all configuration options to defaults.
         *
         * @note Excludes debug options.
         */
        void SetDefaultOptions();

        /** @brief Parses a JSON containing the options configuration to an internal options object.
         *
         * @param optionsJson Options JSON to parse.
         * @return Internal `Options` object.
         */
        Options FromOptionsJson(const json& optionsJson) const;

        /** @brief Parses an internal options object to a JSON containing the options configuration.
         *
         * @param options Internal `Options` object to parse.
         * @return Options JSON.
         */
        json ToOptionsJson(const Options& options) const;
    };
}
