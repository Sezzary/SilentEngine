#pragma once

#include "Input/Action.h"
#include "Input/Event.h"

namespace Silent::Input
{
    using BindingProfile = std::unordered_map<ActionId, std::vector<EventId>>; /** Key = action ID, value = event IDs. */

    extern const BindingProfile USER_KEYBOARD_MOUSE_BINDING_PROFILE_TYPE_1;
    extern const BindingProfile USER_KEYBOARD_MOUSE_BINDING_PROFILE_TYPE_2;
    extern const BindingProfile USER_KEYBOARD_MOUSE_BINDING_PROFILE_TYPE_3;

    extern const BindingProfile USER_GAMEPAD_BINDING_PROFILE_TYPE_1;
    extern const BindingProfile USER_GAMEPAD_BINDING_PROFILE_TYPE_2;
    extern const BindingProfile USER_GAMEPAD_BINDING_PROFILE_TYPE_3;

    extern const BindingProfile RAW_KEYBOARD_BINDING_PROFILE;
    extern const BindingProfile RAW_MOUSE_BINDING_PROFILE;
    extern const BindingProfile RAW_GAMEPAD_BINDING_PROFILE;

    /** @brief Input binding profile IDs. */
    enum class BindingProfileId
    {
        /** Keyboard/mouse */

        KeyboardMouseType1,
        KeyboardMouseType2,
        KeyboardMouseType3,
        KeyboardMouseCustom,

        /** Gamepad */

        GamepadType1,
        GamepadType2,
        GamepadType3,
        GamepadCustom,

        /** Raw */

        RawKeyboard,
        RawMouse,
        RawGamepad,

        Count
    };

    extern const std::vector<BindingProfileId> USER_KEYBOARD_MOUSE_BINDING_PROFILE_IDS;
    extern const std::vector<BindingProfileId> USER_GAMEPAD_BINDING_PROFILE_IDS;
    extern const std::vector<BindingProfileId> RAW_EVENT_BINDING_PROFILE_IDS;

    /** @brief Central manager for input bindings. */
    class BindingManager
    {
    private:
        // =======
        // Fields
        // =======

        std::unordered_map<BindingProfileId, BindingProfile> _bindings = {}; /** Key = binding profile ID, value = binding profile. */

    public:
        // =============
        // Constructors
        // =============

        BindingManager() = default;

        // ========
        // Getters
        // ========

        /** @brief Gets the device event IDs bound to an input action within a binding profile.
         *
         * @param profileID Input binding profile ID.
         * @param actionID Input action ID.
         * @return `std::vector` containing the device event IDs bound to the given input action.
         */
        const std::vector<EventId>& GetBoundEventIds(BindingProfileId profileId, ActionId actionId) const;

        /** @brief Gets a reference to an input binding profile containing associations between device events and input actions.
         *
         * @param profileId Input binding profile ID to retrieve.
         * @return Input binding profile reference.
         */
        const BindingProfile& GetBindingProfile(BindingProfileId profileId) const;

        // ==========
        // Utilities
        // ==========

        /** @brief Initializes the input binding manager, setting default bindings and active user bindings.
         *
         * @param customKeyboardMouseBinds Active keyboard/mouse input bindings to set for the user.
         * @param customGamepadBinds Active gamepoad input bindings to set for the user.
         */
        void Initialize(const BindingProfile& customKeyboardMouseBinds, const BindingProfile& customGamepadBinds);

        /** @brief Binds a device event to an input action, storing the new association inside an input binding profile.
         *
         * @param profileId Input binding profile ID to update.
         * @param actionId Input action ID to create a new binding for.
         * @param eventId Device event ID to bind to the input action.
         */
        void BindEventId(BindingProfileId profileId, ActionId actionId, EventId eventId);

        /** @brief Unbinds all device event indings from an input action in an input binding profile.
         *
         * @param profileId Input binding profile ID to update.
         * @param actionId Input action ID to clear all bindings for.
         */
        void UnbindEventIds(BindingProfileId profileId, ActionId actionId);
    };
}
