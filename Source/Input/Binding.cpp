#include "Framework.h"
#include "Input/Binding.h"

#include "Input/Action.h"
#include "Input/Event.h"
#include "Services/Clock.h"
#include "Utils/Utils.h"

using namespace Silent::Services;
using namespace Silent::Utils;

namespace Silent::Input
{
    const BindingProfile USER_KEYBOARD_MOUSE_BINDING_PROFILE_TYPE_1 =
    {
        { In::Up,        { EventId::W, EventId::Up } },
        { In::Down,      { EventId::S, EventId::Down } },
        { In::Left,      { EventId::A, EventId::Left } },
        { In::Right,     { EventId::D, EventId::Right } },

        { In::Enter,     { EventId::Return } },
        { In::Cancel,    { EventId::Escape } },
        { In::Skip,      { EventId::Escape } },

        { In::Action,    { EventId::Ctrl, EventId::MouseClickLeft } },
        { In::Aim,       { EventId::Space } },
        { In::Light,     { EventId::L } },
        { In::Run,       { EventId::J } },
        { In::View,      { EventId::K } },
        { In::StepLeft,  { EventId::Q } },
        { In::StepRight, { EventId::E } },
        { In::Pause,     { EventId::P } },
        { In::Item,      { EventId::I } },
        { In::Map,       { EventId::M } },
        { In::Option,    { EventId::O } }
    };

    const BindingProfile USER_KEYBOARD_MOUSE_BINDING_PROFILE_TYPE_2 =
    {
        { In::Up,        { EventId::W, EventId::Up } },
        { In::Down,      { EventId::S, EventId::Down } },
        { In::Left,      { EventId::A, EventId::Left } },
        { In::Right,     { EventId::D, EventId::Right } },

        { In::Enter,     { EventId::Return } },
        { In::Cancel,    { EventId::Escape } },
        { In::Skip,      { EventId::Escape } },

        { In::Action,    { EventId::MouseClickLeft, EventId::F } },
        { In::Aim,       { EventId::MouseClickRight } },
        { In::Light,     { EventId::L } },
        { In::Run,       { EventId::Shift } },
        { In::View,      { EventId::Ctrl } },
        { In::StepLeft,  { EventId::Q } },
        { In::StepRight, { EventId::E } },
        { In::Pause,     { EventId::P } },
        { In::Item,      { EventId::I } },
        { In::Map,       { EventId::M } },
        { In::Option,    { EventId::O } }
    };

    const BindingProfile USER_KEYBOARD_MOUSE_BINDING_PROFILE_TYPE_3 =
    {
        { In::Up,        { EventId::Up } },
        { In::Down,      { EventId::Down } },
        { In::Left,      { EventId::Left } },
        { In::Right,     { EventId::Right } },

        { In::Enter,     { EventId::Return } },
        { In::Cancel,    { EventId::Escape } },
        { In::Skip,      { EventId::Escape } },

        { In::Action,    { EventId::Ctrl, EventId::MouseClickLeft } },
        { In::Aim,       { EventId::Space } },
        { In::Light,     { EventId::L } },
        { In::Run,       { EventId::Z } },
        { In::View,      { EventId::Pad0 } },
        { In::StepLeft,  { EventId::A } },
        { In::StepRight, { EventId::D } },
        { In::Pause,     { EventId::P } },
        { In::Item,      { EventId::I } },
        { In::Map,       { EventId::M } },
        { In::Option,    { EventId::O } }
    };

    const BindingProfile USER_GAMEPAD_BINDING_PROFILE_TYPE_1 =
    {
        { In::Up,        { EventId::GamepadDpadUp, EventId::GamepadStickLeftUp } },
        { In::Down,      { EventId::GamepadDpadDown, EventId::GamepadStickLeftDown } },
        { In::Left,      { EventId::GamepadDpadLeft, EventId::GamepadStickLeftLeft } },
        { In::Right,     { EventId::GamepadDpadRight, EventId::GamepadStickLeftRight } },

        { In::Enter,     { EventId::GamepadSouth, EventId::GamepadStart } },
        { In::Cancel,    { EventId::GamepadNorth, EventId::GamepadEast, EventId::GamepadWest } },
        { In::Skip,      { EventId::GamepadStart } },

        { In::Action,    { EventId::GamepadSouth } },
        { In::Aim,       { EventId::GamepadTriggerRight } },
        { In::Light,     { EventId::GamepadEast } },
        { In::Run,       { EventId::GamepadWest } },
        { In::View,      { EventId::GamepadTriggerLeft } },
        { In::StepLeft,  { EventId::GamepadShoulderLeft } },
        { In::StepRight, { EventId::GamepadShoulderRight} },
        { In::Pause,     { EventId::GamepadStart } },
        { In::Item,      { EventId::GamepadSelect } },
        { In::Map,       { EventId::GamepadNorth } },
        { In::Option,    { EventId::O } }
    };

    const BindingProfile USER_GAMEPAD_BINDING_PROFILE_TYPE_2 =
    {
        { In::Up,        { EventId::GamepadDpadUp, EventId::GamepadStickLeftUp } },
        { In::Down,      { EventId::GamepadDpadDown, EventId::GamepadStickLeftDown } },
        { In::Left,      { EventId::GamepadDpadLeft, EventId::GamepadStickLeftLeft } },
        { In::Right,     { EventId::GamepadDpadRight, EventId::GamepadStickLeftRight } },
        
        { In::Enter,     { EventId::GamepadSouth, EventId::GamepadStart } },
        { In::Cancel,    { EventId::GamepadNorth, EventId::GamepadWest, EventId::GamepadEast } },
        { In::Skip,      { EventId::GamepadStart } },

        { In::Action,    { EventId::GamepadSouth } },
        { In::Aim,       { EventId::GamepadShoulderRight } },
        { In::Light,     { EventId::GamepadEast } },
        { In::Run,       { EventId::GamepadWest } },
        { In::View,      { EventId::GamepadShoulderLeft } },
        { In::StepLeft,  { EventId::GamepadTriggerLeft } },
        { In::StepRight, { EventId::GamepadTriggerRight} },
        { In::Pause,     { EventId::GamepadStart } },
        { In::Item,      { EventId::GamepadSelect } },
        { In::Map,       { EventId::GamepadNorth } },
        { In::Option,    { EventId::O } }
    };

    const BindingProfile USER_GAMEPAD_BINDING_PROFILE_TYPE_3 =
    {
        { In::Up,        { EventId::GamepadDpadUp, EventId::GamepadStickLeftUp } },
        { In::Down,      { EventId::GamepadDpadDown, EventId::GamepadStickLeftDown } },
        { In::Left,      { EventId::GamepadDpadLeft, EventId::GamepadStickLeftLeft } },
        { In::Right,     { EventId::GamepadDpadRight, EventId::GamepadStickLeftRight } },
        
        { In::Enter,     { EventId::GamepadSouth, EventId::GamepadStart } },
        { In::Cancel,    { EventId::GamepadNorth, EventId::GamepadWest, EventId::GamepadEast } },
        { In::Skip,      { EventId::GamepadStart } },

        { In::Action,    { EventId::GamepadSouth } },
        { In::Aim,       { EventId::GamepadTriggerRight } },
        { In::Light,     { EventId::GamepadEast } },
        { In::Run,       { EventId::GamepadWest } },
        { In::View,      { EventId::GamepadTriggerLeft } },
        { In::StepLeft,  { EventId::GamepadShoulderLeft } },
        { In::StepRight, { EventId::GamepadShoulderRight} },
        { In::Pause,     { EventId::GamepadStart } },
        { In::Item,      { EventId::GamepadNorth } },
        { In::Map,       { EventId::GamepadSelect } },
        { In::Option,    { EventId::O } }
    };

    const BindingProfile RAW_KEYBOARD_BINDING_PROFILE
    {
        { In::A,            { EventId::A } },
        { In::B,            { EventId::B } },
        { In::C,            { EventId::C } },
        { In::D,            { EventId::D } },
        { In::E,            { EventId::E } },
        { In::F,            { EventId::F } },
        { In::G,            { EventId::G } },
        { In::H,            { EventId::H } },
        { In::I,            { EventId::I } },
        { In::J,            { EventId::J } },
        { In::K,            { EventId::K } },
        { In::L,            { EventId::L } },
        { In::M,            { EventId::M } },
        { In::N,            { EventId::N } },
        { In::O,            { EventId::O } },
        { In::P,            { EventId::P } },
        { In::Q,            { EventId::Q } },
        { In::R,            { EventId::R } },
        { In::S,            { EventId::S } },
        { In::T,            { EventId::T } },
        { In::U,            { EventId::U } },
        { In::V,            { EventId::V } },
        { In::W,            { EventId::W } },
        { In::X,            { EventId::X } },
        { In::Y,            { EventId::Y } },
        { In::Z,            { EventId::Z } },
        { In::Num1,         { EventId::Row1, EventId::Pad1 } },
        { In::Num2,         { EventId::Row2, EventId::Pad2 } },
        { In::Num3,         { EventId::Row3, EventId::Pad3 } },
        { In::Num4,         { EventId::Row4, EventId::Pad4 } },
        { In::Num5,         { EventId::Row5, EventId::Pad5 } },
        { In::Num6,         { EventId::Row6, EventId::Pad6 } },
        { In::Num7,         { EventId::Row7, EventId::Pad7 } },
        { In::Num8,         { EventId::Row8, EventId::Pad8 } },
        { In::Num9,         { EventId::Row9, EventId::Pad9 } },
        { In::Num0,         { EventId::Row0, EventId::Pad0 } },
        { In::Return,       { EventId::Return, EventId::PadEnter } },
        { In::Escape,       { EventId::Escape } },
        { In::Backspace,    { EventId::Backspace } },
        { In::Tab,          { EventId::Tab } },
        { In::Space,        { EventId::Space } },
        { In::Home,         { EventId::Home } },
        { In::End,          { EventId::End } },
        { In::Delete,       { EventId::Delete } },
        { In::Minus,        { EventId::Minus, EventId::PadMinus } },
        { In::Equals,       { EventId::Equals, EventId::PadPlus } },
        { In::BracketLeft,  { EventId::BracketLeft } },
        { In::BracketRight, { EventId::BracketRight } },
        { In::Backslash,    { EventId::Backslash } },
        { In::Semicolon,    { EventId::Semicolon } },
        { In::Apostrophe,   { EventId::Apostrophe } },
        { In::Comma,        { EventId::Comma } },
        { In::Period,       { EventId::Period, EventId::PadPeriod } },
        { In::Slash,        { EventId::Slash, EventId::PadDivide } },
        { In::ArrowUp,      { EventId::Up } },
        { In::ArrowDown,    { EventId::Down } },
        { In::ArrowLeft,    { EventId::Left } },
        { In::ArrowRight,   { EventId::Right } },
        { In::Ctrl,         { EventId::Ctrl } },
        { In::Shift,        { EventId::Shift } },
        { In::Alt,          { EventId::Alt } }
    };

    const BindingProfile RAW_MOUSE_BINDING_PROFILE
    {
        { In::MouseClickLeft,   { EventId::MouseClickLeft } },
        { In::MouseClickMiddle, { EventId::MouseClickMiddle } },
        { In::MouseClickRight,  { EventId::MouseClickRight } },
        { In::MouseScrollUp,    { EventId::MouseScrollUp } },
        { In::MouseScrollDown,  { EventId::MouseScrollDown } },
        { In::MouseUp,          { EventId::MouseUp } },
        { In::MouseDown,        { EventId::MouseDown } },
        { In::MouseLeft,        { EventId::MouseLeft } },
        { In::MouseRight,       { EventId::MouseRight } }
    };

    const BindingProfile RAW_GAMEPAD_BINDING_PROFILE
    {
        { In::GamepadNorth,           { EventId::GamepadNorth } },
        { In::GamepadSouth,           { EventId::GamepadSouth } },
        { In::GamepadEast,            { EventId::GamepadEast } },
        { In::GamepadWest,            { EventId::GamepadWest } },
        { In::GamepadStart,           { EventId::GamepadStart } },
        { In::GamepadSelect,          { EventId::GamepadSelect } },
        { In::GamepadShoulderLeft,    { EventId::GamepadShoulderLeft } },
        { In::GamepadShoulderRight,   { EventId::GamepadShoulderRight } },
        { In::GamepadTriggerLeft,     { EventId::GamepadTriggerLeft } },
        { In::GamepadTriggerRight,    { EventId::GamepadTriggerRight } },
        { In::GamepadDpadUp,          { EventId::GamepadDpadUp } },
        { In::GamepadDpadDown,        { EventId::GamepadDpadDown } },
        { In::GamepadDpadLeft,        { EventId::GamepadDpadLeft } },
        { In::GamepadDpadRight,       { EventId::GamepadDpadRight } },
        { In::GamepadStickLeftIn,     { EventId::GamepadStickLeft } },
        { In::GamepadStickLeftUp,     { EventId::GamepadStickLeftUp } },
        { In::GamepadStickLeftDown,   { EventId::GamepadStickLeftDown } },
        { In::GamepadStickLeftLeft,   { EventId::GamepadStickLeftLeft } },
        { In::GamepadStickLeftRight,  { EventId::GamepadStickLeftRight } },
        { In::GamepadStickRightIn,    { EventId::GamepadStickRight } },
        { In::GamepadStickRightUp,    { EventId::GamepadStickRightUp } },
        { In::GamepadStickRightDown,  { EventId::GamepadStickRightDown } },
        { In::GamepadStickRightLeft,  { EventId::GamepadStickRightLeft } },
        { In::GamepadStickRightRight, { EventId::GamepadStickRightRight } }
    };

    const std::vector<BindingProfileId> USER_KEYBOARD_MOUSE_BINDING_PROFILE_IDS
    {
        BindingProfileId::KeyboardMouseType1,
        BindingProfileId::KeyboardMouseType2,
        BindingProfileId::KeyboardMouseType3,
        BindingProfileId::KeyboardMouseCustom
    };

    const std::vector<BindingProfileId> USER_GAMEPAD_BINDING_PROFILE_IDS
    {
        BindingProfileId::GamepadType1,
        BindingProfileId::GamepadType2,
        BindingProfileId::GamepadType3,
        BindingProfileId::GamepadCustom
    };

    const std::vector<BindingProfileId> RAW_EVENT_BINDING_PROFILE_IDS
    {
        BindingProfileId::RawKeyboard,
        BindingProfileId::RawMouse,
        BindingProfileId::RawGamepad
    };

    const std::vector<EventId>& BindingManager::GetBoundEventIds(BindingProfileId profileId, ActionId actionId) const
    {
        static const auto EMPTY = std::vector<EventId>{};

        // Find binding profile.
        const auto* profile = Find(_bindings, profileId);
        if (profile == nullptr)
        {
            return EMPTY;
        }

        // Find action-event binding.
        const auto* eventIds = Find(*profile, actionId);
        if (eventIds == nullptr)
        {
            return EMPTY;
        }

        // Return bound event IDs.
        return *eventIds;
    }

    const BindingProfile& BindingManager::GetBindingProfile(BindingProfileId profileId) const
    {
        // Find binding profile.
        auto* profile = Find(_bindings, profileId);
        Debug::Assert(profile != nullptr, "Attempted to get missing binding profile " + std::to_string((int)profileId) + ".");

        // Return binding profile.
        return *profile;
    }

    void BindingManager::Initialize(const BindingProfile& customKeyboardMouseBinds, const BindingProfile& customGamepadBinds)
    {
        _bindings =
        {
            { BindingProfileId::KeyboardMouseType1,  USER_KEYBOARD_MOUSE_BINDING_PROFILE_TYPE_1 },
            { BindingProfileId::KeyboardMouseType2,  USER_KEYBOARD_MOUSE_BINDING_PROFILE_TYPE_2 },
            { BindingProfileId::KeyboardMouseType3,  USER_KEYBOARD_MOUSE_BINDING_PROFILE_TYPE_3 },
            { BindingProfileId::KeyboardMouseCustom, customKeyboardMouseBinds },

            { BindingProfileId::GamepadType1,  USER_GAMEPAD_BINDING_PROFILE_TYPE_1 },
            { BindingProfileId::GamepadType2,  USER_GAMEPAD_BINDING_PROFILE_TYPE_2 },
            { BindingProfileId::GamepadType3,  USER_GAMEPAD_BINDING_PROFILE_TYPE_3 },
            { BindingProfileId::GamepadCustom, customGamepadBinds },

            { BindingProfileId::RawKeyboard, RAW_KEYBOARD_BINDING_PROFILE },
            { BindingProfileId::RawMouse,    RAW_MOUSE_BINDING_PROFILE },
            { BindingProfileId::RawGamepad,  RAW_GAMEPAD_BINDING_PROFILE }
        };
    }

    void BindingManager::BindEventId(BindingProfileId profileId, ActionId actionId, EventId eventId)
    {
        if (Contains(_bindings[profileId][actionId], eventId))
        {
            Debug::Log(fmt::format("Attempted to bind existing event {} to action {} in binding profile {}.", (int)eventId, (int)actionId, (int)profileId),
                       Debug::LogLevel::Warning);
            return;
        }

        _bindings[profileId][actionId].push_back(eventId);
    }

    void BindingManager::UnbindEventIds(BindingProfileId profileId, ActionId actionId)
    {
        _bindings[profileId][actionId].clear();
    }
}
