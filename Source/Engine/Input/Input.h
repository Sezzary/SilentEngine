#pragma once

#include "Engine/Input/Action.h"
#include "Engine/Input/Binding.h"
#include "Engine/Input/Event.h"
#include "Engine/Input/Text.h"

namespace Silent::Input
{
    /** @brief Gamepad vendor IDs. */
    enum class GamepadVendorId
    {
        Generic,
        Xbox,
        Nintendo,
        Sony
    };

    /** @brief Analog axis IDs for specialized gameplay and input device axes. */
    enum class AnalogAxisId
    {
        // Gameplay axes

        Move,
        Camera,

        // Input device axes

        Mouse,
        StickLeft,
        StickRight,

        Count
    };

    /** @brief Gamepad rumble modes. */
    enum class RumbleMode
    {
        Low,
        High,
        LowAndHigh
    };

    /** @brief Raw input state data. */
    struct States
    {
        bool               IsUsingGamepad     = false;
        std::vector<float> Events             = {}; /** Index = `EventId`. */
        Vector2            CursorPosition     = {};
        Vector2            PrevCursorPosition = {};
    };

    /** @brief Connected gamepad data. */
    struct Gamepad
    {
        int             Id       = NO_VALUE;
        SDL_Gamepad*    Device   = nullptr;
        GamepadVendorId VendorId = GamepadVendorId::Generic;
    };

    /** @brief Active gamepad rumble data. */
    struct Rumble
    {
        RumbleMode Mode          = RumbleMode::Low;
        float      IntensityFrom = 0.0f;
        float      IntensityTo   = 0.0f;
        uint       DurationTicks = 0;
        uint       Ticks         = 0;
    };

    /** @brief Central input manager. */
    class InputManager
    {
    private:
        // =======
        // Fields
        // =======

        Gamepad                              _gamepad    = {};
        BindingManager                       _bindings   = BindingManager();
        TextManager                          _text       = TextManager();
        States                               _states     = {};
        Rumble                               _rumble     = {};
        std::unordered_map<ActionId, Action> _actions    = {};
        std::vector<Vector2>                 _analogAxes = {}; /** Index = `AnalogAxisId`. */

    public:
        // =============
        // Constructors
        // =============

        InputManager() = default;

        // ========
        // Getters
        // ========

        /** @brief Gets a reference to an input action.
         *
         * @param actionId Input action ID.
         * @return Input action reference.
         */
        const Action& GetAction(ActionId actionId) const;

        /** @brief Gets a reference to an analog axis.
         *
         * @param axisId Analog axis ID.
         * @return Analog axis reference.
         */
        const Vector2& GetAnalogAxis(AnalogAxisId axisId) const;

        /** @brief Gets a reference to the mouse cursor's current screen position.
         *
         * @return Current mouse cursor screeb position.
         */
        const Vector2& GetCursorPosition() const;

        /** @brief Gets a connected gamepad's vendor ID. If no gamepad is connected, it defaults to the generic vendor.
         *
         * @return Gamepad vendor ID.
         */
        GamepadVendorId GetGamepadVendorId() const;

        const std::string& GetText(const std::string& textId) const;

        std::vector<std::string> GetTextLines(const std::string& bufferId, uint low = (uint)NO_VALUE, uint high = (uint)NO_VALUE) const;

        uint GetTextCursorPosition(const std::string& textId) const;

        // ========
        // Setters
        // ========

        /** @brief Sets a new gamepad rumble. If a previously set rumble is currently active, it will be overridden.
         *
         * @param mode Rumble mode.
         * @param intensityFrom Intensity at the start of the rumble's duration.
         * @param intensityTo Intensity at the end of the rumble's duration.
         * @param durationSec Rumble duration in seconds.
         */
        void SetRumble(RumbleMode mode, float intensityFrom, float intensityTo, float durationSec);

        // ==========
        // Inquirers
        // ==========

        /** @brief Checks if a gamepad is currently connected.
         *
         * @return `true` if a gamepad is connected, `false` otherwise.
         */
        bool IsGamepadConnected() const;

        /** @brief Checks if a gamepad is being currently being used. The condition will be `true` if one of the inputs
         * registered for the current tick came from a gamepad.
         *
         * @return `true` if a gamepad is being used, `false` otherwise.
         */
        bool IsUsingGamepad() const;
        
        // ==========
        // Utilities
        // ==========

        /** @brief Initializes the input manager. */
        void Initialize();

        /** @brief Gracefully deinitializes the input manager by disconnecting the connected gamepad. */
        void Deinitialize();

        /** @brief Updates the input system for the current tick.
         *
         * @param window Application window.
         * @param mouseWheelAxis Raw mouse wheel axis data for the current tick.
         */
        void Update(SDL_Window& window, const Vector2& mouseWheelAxis);

        /** @brief Attempts to connect a gamepad if not already connected.
         *
         * @param deviceId Device ID of the gamepad to connect.
         */
        void ConnectGamepad(int deviceId);

        /** @brief Disconnects a gamepad if connected.
         *
         * @param deviceId Device ID of the gamepad to disconnect.
         */
        void DisconnectGamepad(int deviceId);

        void InsertText(const std::string& textId, uint lineWidthMax = 50, uint charCountMax = UINT_MAX);
        void UpdateText(const std::string& textId);
        void RemoveText(const std::string& textId);
        
    private:
        // ========
        // Helpers
        // ========

        /** @brief Gets the name of a given gamepad vendor.
         *
         * @param vendorId Gamepad vendor ID.
         * @return Gamepad vendor name.
         */
        std::string GetGamepadVendorName(GamepadVendorId vendorId) const;

        /** @brief Reads keyboard data and updates raw input states for the current tick.
         * Called first in a sequence of function calls which read device data. Superceded by `ReadMouse`.
         *
         * @param eventStateIdx Reference input event state index. Incremented as keyboard data is read and passed on to `ReadMouse`.
         */
        void ReadKeyboard(int& eventStateIdx);

        /** @brief Reads mouse data and updates raw input states for the current tick.
         * Called second in a sequence of function calls which read device data. Preceeded by `ReadKeyboard`, succeeded by `ReadGamepad`.
         *
         * @param eventStateIdx Reference input event state index. Incremented as mouse data is read and passed on to `ReadGamepad`.
         */
        void ReadMouse(int& eventStateIdx, SDL_Window& window, const Vector2& wheelAxis);

        /** @brief Reads mouse data and updates raw input states for the current tick.
         * Called third in a sequence of function calls which read device data. Preceeded by `ReadMouse`.
         *
         * @param eventStateIdx Reference input event state index. Incremented as gamepad data is read.
         */
        void ReadGamepad(int& eventStateIdx);

        /** @brief Updates rumble data for the current tick if a rumble is active. */
        void UpdateRumble();

        /** @brief Updates input actions for the current tick. */
        void UpdateActions();

        /** @brief Handles hardcoded hotkey actions for the current tick.
         *
         * @note Hotkey actions include:
         * - Screenshot capture
         * - Fullscreen toggle
         * - Debug GUI toggle
         */
        void HandleHotkeyActions();
    };
}
