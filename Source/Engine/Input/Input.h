#pragma once

#include "Engine/Input/Action.h"
#include "Engine/Input/Binding.h"
#include "Engine/Input/Text.h"

namespace Silent::Input
{
    enum class AnalogAxisId
    {
        Move,
        Camera,

        Mouse,
        StickLeft,
        StickRight,

        Count
    };

    enum class RumbleMode
    {
        Low,
        High,
        LowAndHigh
    };

    struct States
    {
        bool               IsUsingGamepad     = false;
        std::vector<float> Events             = {}; // Index = `EventId`.
        Vector2            CursorPosition     = {};
        Vector2            PrevCursorPosition = {};
    };

    struct Rumble
    {
        RumbleMode Mode          = RumbleMode::Low;
        float      IntensityFrom = 0.0f;
        float      IntensityTo   = 0.0f;
        uint       DurationTicks = 0;
        uint       Ticks         = 0;
    };

    class InputManager
    {
    public:
        // Constants

        static constexpr int GAMEPAD_ID = 0;

    private:
        // Fields

        SDL_Gamepad* _gamepad = nullptr;

        BindingManager                       _bindings   = BindingManager();
        TextManager                          _text       = TextManager();
        States                               _states     = {};
        Rumble                               _rumble     = {};
        std::unordered_map<ActionId, Action> _actions    = {};
        std::vector<Vector2>                 _analogAxes = {}; // Index = `AnalogAxisId`.

    public:
        // Constructors

        InputManager() = default;

        // Getters

        const Action&            GetAction(ActionId actionId) const;
        const Vector2&           GetAnalogAxis(AnalogAxisId axisId) const;
        const Vector2&           GetCursorPosition() const;
        const std::string&       GetText(const std::string& textId) const;
        std::vector<std::string> GetTextLines(const std::string& bufferId, uint low = (uint)NO_VALUE, uint high = (uint)NO_VALUE) const;
        uint                     GetTextCursorPosition(const std::string& textId) const;

        // Setters

        void SetRumble(RumbleMode mode, float intensityFrom, float intensityTo, float durationSec);

        // Inquirers

        bool IsGamepadConnected() const;
        bool IsUsingGamepad() const;

        // Utilities

        void Initialize();
        void Deinitialize();
        void Update(SDL_Window& window, const Vector2& mouseWheelAxis);

        void InsertText(const std::string& textId, uint lineWidthMax = 50, uint charCountMax = UINT_MAX);
        void UpdateText(const std::string& textId);
        void RemoveText(const std::string& textId);

    private:
        // Helpers

        void ReadKeyboard(int& eventStateIdx);
        void ReadMouse(int& eventStateIdx, SDL_Window& window, const Vector2& wheelAxis);
        void ReadGamepad(int& eventStateIdx);

        void UpdateRumble();
        void UpdateActions();

        void HandleHotkeyActions();
        void HandleGamepadDisconnect();
    };
}
