#pragma once

namespace Silent::Input
{
    typedef enum class ActionId
    {
        // General

        Up,
        Down,
        Left,
        Right,

        // Menu

        Enter,
        Cancel,
        Skip,

        // Game
        
        Action,
        Aim,
        Light,
        Run,
        View,
        StepLeft,
        StepRight,
        Pause,
        Item,
        Map,
        Option,

        // Raw keyboard

        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, Num0,
        Return, Escape, Backspace, Tab, Space, Home, End, Delete,
        Minus, Equals, BracketLeft, BracketRight, Backslash, Semicolon, Apostrophe, Comma, Period, Slash,
        ArrowUp, ArrowDown, ArrowLeft, ArrowRight,
        Ctrl, Shift, Alt,

        // Raw mouse

        MouseClickLeft,
        MouseClickMiddle,
        MouseClickRight,
        MouseScrollUp,
        MouseScrollDown,
        MouseUp,
        MouseDown,
        MouseLeft,
        MouseRight,

        // Raw gamepad

        GamepadNorth,
        GamepadSouth,
        GamepadEast,
        GamepadWest,
        GamepadStart,
        GamepadSelect,
        GamepadShoulderLeft,
        GamepadShoulderRight,
        GamepadTriggerLeft,
        GamepadTriggerRight,
        GamepadDpadUp,
        GamepadDpadDown,
        GamepadDpadLeft,
        GamepadDpadRight,
        GamepadStickLeftIn,
        GamepadStickLeftUp,
        GamepadStickLeftDown,
        GamepadStickLeftLeft,
        GamepadStickLeftRight,
        GamepadStickRightIn,
        GamepadStickRightUp,
        GamepadStickRightDown,
        GamepadStickRightLeft,
        GamepadStickRightRight,

        Count
    } In;

    enum class ActionGroupId
    {
        General,
        Menu,
        Game,

        Keyboard,
        Mouse,
        Gamepad,

        Printable
    };

    extern const std::unordered_map<ActionGroupId, std::vector<ActionId>> ACTION_ID_GROUPS;
    extern const std::vector<ActionGroupId>                               USER_ACTION_GROUP_IDS;
    extern const std::vector<ActionGroupId>                               RAW_ACTION_GROUP_IDS;

    class Action
    {
    private:
        // Fields
        
        ActionId _id              = In::Up;
        float    _state           = 0.0f;
        float    _prevState       = 0.0f;
        uint     _ticksActive     = 0;
        uint     _prevTicksActive = 0;
        uint     _ticksInactive   = 0;

    public:
        // Constructors

        Action() = default;
        Action(ActionId actionId);

        // Getters

        ActionId GetId() const;
        float    GetState() const;
        uint     GetTicksActive() const;
        uint     GetTicksInactive() const;
        
        // Inquirers
        
        bool IsClicked(float stateMin = 0.0f) const;
        bool IsHeld(float delaySec = 0.0f, float stateMin = 0.0f) const;
        bool IsPulsed(float delaySec, float initialDelaySec = 0.0f, float stateMin = 0.0f) const;
        bool IsReleased(float delaySecMax = INFINITY, float stateMin = 0.0f) const;

        // Utilities

        void Update(float state);
        void Clear();
    };
}
