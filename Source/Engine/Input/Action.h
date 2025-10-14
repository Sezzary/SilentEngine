#pragma once

namespace Silent::Input
{
    /** @brief Input action IDs. */
    typedef enum class ActionId
    {
        /** General */

        Up,
        Down,
        Left,
        Right,

        /** Menu */

        Enter,
        Cancel,
        Skip,

        /** Game */
        
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

        /** Raw keyboard */

        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, Num0,
        Return, Escape, Backspace, Tab, Space, Home, End, Delete,
        Minus, Equals, BracketLeft, BracketRight, Backslash, Semicolon, Apostrophe, Comma, Period, Slash,
        ArrowUp, ArrowDown, ArrowLeft, ArrowRight,
        Ctrl, Shift, Alt,

        /** Raw mouse */

        MouseClickLeft,
        MouseClickMiddle,
        MouseClickRight,
        MouseScrollUp,
        MouseScrollDown,
        MouseUp,
        MouseDown,
        MouseLeft,
        MouseRight,

        /** Raw gamepad */

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

    /** @brief Input action group IDs. */
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

    /** @brief Input action. */
    class Action
    {
    private:
        // =======
        // Fields
        // =======
        
        ActionId _id              = In::Up;
        float    _state           = 0.0f;
        float    _prevState       = 0.0f;
        uint     _ticksActive     = 0;
        uint     _prevTicksActive = 0;
        uint     _ticksInactive   = 0;

    public:
        // =============
        // Constructors
        // =============

        /** @brief Constructs an uninitialized default `Action`. */
        Action() = default;

        /** @brief Constructs an `Action` with a given ID.
         *
         * @param actionId Action ID to set.
         */
        Action(ActionId actionId);

        // ========
        // Getters
        // ========

        /** @brief Gets the action's ID.
         *
         * @return Action ID.
         */
        ActionId GetId() const;

        /** @brief Gets the action's analog state value for the current tick.
         *
         * @return Analog state value, range `[0.0f, 1.0f]`.
         */
        float GetState() const;

        /** @brief Gets the number of ticks the action has been active.
         *
         * @return Active tick count.
         */
        uint GetTicksActive() const;

        /** @brief Gets the number of ticks the action has been inactive.
         *
         * @return Inactive tick count.
         */
        uint GetTicksInactive() const;

        // ==========
        // Inquirers
        // ==========

        /** @brief Checks if the action is clicked/debounced for the current tick.
         *
         * @param stateMin Minimum analog state for a click to be registered, range `[0.0f, 1.0f]`.
         * @return `true` if clicked, `false` otherwise.
         */
        bool IsClicked(float stateMin = 0.0f) const;

        /** @brief Checks if the action is held/throttled for the current tick.
         *
         * @param delaySec Seconds to delay while held before a hold can be registered.
         * @param stateMin Minimum analog state for a hold to be registered, range `[0.0f, 1.0f]`
         * @return `true` if held, `false` otherwise.
         */
        bool IsHeld(float delaySec = 0.0f, float stateMin = 0.0f) const;

        /** @brief Checks if the action is pulsed for the current tick.
         *
         * @note To avoid a stutter on the second pulse, `initialDelaySec` must be a multiple of `delaySec`.
         * 
         * @param delaySec Seconds to delay before a pulse can be registered.
         * @param initialDelaySec Initial seconds to delay before a second pulse can be registered.
         * @param stateMin Minimum analog state for a pulse to be registered, range `[0.0f, 1.0f]`
         * @return `true` if pulsed, `false` otherwise.
         */
        bool IsPulsed(float delaySec, float initialDelaySec = 0.0f, float stateMin = 0.0f) const;

        /** @brief Checks if the action is released/debounced for the current tick.
         *
         * @param delaySecMax Maximum seconds allowed to hold before releasing for a release to be registered.
         * @param stateMin Minimum analog state for a release to be registered, range `[0.0f, 1.0f]`
         * @return `true` if released, `false` otherwise.
         */
        bool IsReleased(float delaySecMax = FLT_MAX, float stateMin = 0.0f) const;

        // ==========
        // Utilities
        // ==========

        /** @brief Updates the action state for the current tick.
         *
         * @param state Analog state.
         */
        void Update(float state);

        /** @brief Resets the action state, clearing all values. */
        void Clear();
    };
}
