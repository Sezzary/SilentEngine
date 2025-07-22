#pragma once

#include "Engine/Services/Input/Action.h"
#include "Engine/Services/Input/Event.h"

namespace Silent::Input
{
    using BindingProfile = std::unordered_map<ActionId, std::vector<EventId>>; // Key = action ID, value = event IDs.

    extern const BindingProfile USER_KEYBOARD_MOUSE_BINDING_PROFILE_TYPE_1;
    extern const BindingProfile USER_KEYBOARD_MOUSE_BINDING_PROFILE_TYPE_2;
    extern const BindingProfile USER_KEYBOARD_MOUSE_BINDING_PROFILE_TYPE_3;

    extern const BindingProfile USER_GAMEPAD_BINDING_PROFILE_TYPE_1;
    extern const BindingProfile USER_GAMEPAD_BINDING_PROFILE_TYPE_2;
    extern const BindingProfile USER_GAMEPAD_BINDING_PROFILE_TYPE_3;

    extern const BindingProfile RAW_KEYBOARD_BINDING_PROFILE;
    extern const BindingProfile RAW_MOUSE_BINDING_PROFILE;
    extern const BindingProfile RAW_GAMEPAD_BINDING_PROFILE;

    enum class BindingProfileId
    {
        // Keyboard/mouse

        KeyboardMouseType1,
        KeyboardMouseType2,
        KeyboardMouseType3,
        KeyboardMouseCustom,

        // Gamepad

        GamepadType1,
        GamepadType2,
        GamepadType3,
        GamepadCustom,

        // Raw

        RawKeyboard,
        RawMouse,
        RawGamepad,

        Count
    };

    extern const std::vector<BindingProfileId> USER_KEYBOARD_MOUSE_BINDING_PROFILE_IDS;
    extern const std::vector<BindingProfileId> USER_GAMEPAD_BINDING_PROFILE_IDS;
    extern const std::vector<BindingProfileId> RAW_EVENT_BINDING_PROFILE_IDS;

    class BindingManager
    {
    private:
        // Fields

        std::unordered_map<BindingProfileId, BindingProfile> _bindings = {}; // Key = binding profile ID, value = binding profile.

    public:
        // Constructors

        BindingManager() = default;

        // Getters

        const std::vector<EventId>& GetBoundEventIds(BindingProfileId profileId, ActionId actionId) const;
        const BindingProfile&       GetBindingProfile(BindingProfileId profileId) const;

        // Utilities

        void Initialize(const BindingProfile& customKeyboardMouseBinds, const BindingProfile& customGamepadBinds);
        void BindEventId(BindingProfileId profileId, ActionId actionId, EventId eventId);
        void UnbindEventIds(BindingProfileId profileId, ActionId actionId);
    };
}
