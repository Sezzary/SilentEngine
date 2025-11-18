#include "Framework.h"
#include "Input/Action.h"

#include "Services/Clock.h"

using namespace Silent::Services;

namespace Silent::Input
{
    const std::unordered_map<ActionGroupId, std::vector<ActionId>> ACTION_ID_GROUPS
    {
        {
            ActionGroupId::General,
            {
                In::Up,
                In::Down,
                In::Left,
                In::Right
            }
        },
        {
            ActionGroupId::Menu,
            {
                In::Enter,
                In::Cancel,
                In::Skip
            }
        },
        {
            ActionGroupId::Gameplay,
            {
                In::Action,
                In::Aim,
                In::Light,
                In::Run,
                In::View,
                In::StepLeft,
                In::StepRight,
                In::Pause,
                In::Item,
                In::Map,
                In::Option
            }
        },
        {
            ActionGroupId::Keyboard,
            {
                In::A, In::B, In::C, In::D, In::E, In::F, In::G, In::H, In::I, In::J, In::K, In::L, In::M,
                In::N, In::O, In::P, In::Q, In::R, In::S, In::T, In::U, In::V, In::W, In::X, In::Y, In::Z,
                In::Num1, In::Num2, In::Num3, In::Num4, In::Num5, In::Num6, In::Num7, In::Num8, In::Num9, In::Num0,
                In::Return, In::Escape, In::Backspace, In::Tab, In::Space, In::Home, In::End, In::Delete,
                In::Minus, In::Equals, In::BracketLeft, In::BracketRight, In::Backslash, In::Semicolon, In::Apostrophe, In::Comma, In::Period, In::Slash,
                In::ArrowUp, In::ArrowDown, In::ArrowLeft, In::ArrowRight,
                In::Ctrl, In::Shift, In::Alt
            }
        },
        {
            ActionGroupId::Mouse,
            {
                In::MouseClickLeft,
                In::MouseClickMiddle,
                In::MouseClickRight,
                In::MouseScrollUp,
                In::MouseScrollDown,
                In::MouseUp,
                In::MouseDown,
                In::MouseLeft,
                In::MouseRight
            }
        },
        {
            ActionGroupId::Gamepad,
            {
                In::GamepadNorth,
                In::GamepadSouth,
                In::GamepadEast,
                In::GamepadWest,
                In::GamepadStart,
                In::GamepadSelect,
                In::GamepadShoulderLeft,
                In::GamepadTriggerLeft,
                In::GamepadShoulderRight,
                In::GamepadTriggerRight,
                In::GamepadDpadUp,
                In::GamepadDpadDown,
                In::GamepadDpadLeft,
                In::GamepadDpadRight,
                In::GamepadStickLeftIn,
                In::GamepadStickLeftUp,
                In::GamepadStickLeftDown,
                In::GamepadStickLeftLeft,
                In::GamepadStickLeftRight,
                In::GamepadStickRightIn,
                In::GamepadStickRightUp,
                In::GamepadStickRightDown,
                In::GamepadStickRightLeft,
                In::GamepadStickRightRight
            }
        },
        {
            ActionGroupId::Printable,
            {
                In::A, In::B, In::C, In::D, In::E, In::F, In::G, In::H, In::I, In::J, In::K, In::L, In::M,
                In::N, In::O, In::P, In::Q, In::R, In::S, In::T, In::U, In::V, In::W, In::X, In::Y, In::Z,
                In::Num1, In::Num2, In::Num3, In::Num4, In::Num5, In::Num6, In::Num7, In::Num8, In::Num9, In::Num0,
                In::Space,
                In::Minus,
                In::Equals,
                In::BracketLeft,
                In::BracketRight,
                In::Backslash,
                In::Semicolon,
                In::Apostrophe,
                In::Comma,
                In::Period,
                In::Slash
            }
        }
    };

    const std::vector<ActionGroupId> USER_ACTION_GROUP_IDS =
    {
        ActionGroupId::General,
        ActionGroupId::Menu,
        ActionGroupId::Gameplay
    };

    const std::vector<ActionGroupId> RAW_ACTION_GROUP_IDS =
    {
        ActionGroupId::Keyboard,
        ActionGroupId::Mouse,
        ActionGroupId::Gamepad
    };

    Action::Action(ActionId actionId)
    {
        _id = actionId;
    }

    ActionId Action::GetId() const
    {
        return _id;
    }
    
    float Action::GetState() const
    {
        return _state;
    }
    
    uint Action::GetTicksActive() const
    {
        return _ticksActive;
    }
    
    uint Action::GetTicksInactive() const
    {
        return _ticksInactive;
    }
    
    bool Action::IsClicked(float stateMin) const
    {
        return _state > stateMin && _prevState <= stateMin;
    }

    bool Action::IsHeld(float delaySec, float stateMin) const
    {
        if (_state <= stateMin)
        {
            return false;
        }

        uint delayTicks = (delaySec == 0.0f) ? 0 : SEC_TO_TICK(delaySec);
        return _ticksActive >= delayTicks;
    }

    bool Action::IsPulsed(float delaySec, float initialDelaySec, float stateMin) const
    {
        if (IsClicked(stateMin))
        {
            return true;
        }

        if (!IsHeld(stateMin) || _prevTicksActive == 0 || _ticksActive == _prevTicksActive)
        {
            return false;
        }

        float activeDelaySec   = (_ticksActive > SEC_TO_TICK(initialDelaySec)) ? delaySec : initialDelaySec;
        uint  activeDelayTicks = SEC_TO_TICK(activeDelaySec);

        uint delayTicks     = (uint)floor(_ticksActive     / activeDelayTicks) * activeDelayTicks;
        uint prevDelayTicks = (uint)floor(_prevTicksActive / activeDelayTicks) * activeDelayTicks;
        return delayTicks > prevDelayTicks;
    }

    bool Action::IsReleased(float delaySecMax, float stateMin) const
    {
        uint delayTicksMax = (delaySecMax == FLT_MAX) ? UINT_MAX : SEC_TO_TICK(delaySecMax);
        return _state <= stateMin && _prevState > stateMin && _ticksActive <= delayTicksMax;
    }

    void Action::Update(float state)
    {
        _prevState = _state;
        _state     = state;

        if (IsClicked())
        {
            _prevTicksActive = 0;
            _ticksActive     = 0;
            _ticksInactive++;
        }
        else if (IsReleased())
        {
            _prevTicksActive = _ticksActive;
            _ticksInactive   = 0;
            _ticksActive++;
        }
        else if (IsHeld())
        {
            _prevTicksActive = _ticksActive;
            _ticksInactive   = 0;
            _ticksActive++;
        }
        else
        {
            _prevTicksActive = 0;
            _ticksActive     = 0;
            _ticksInactive++;
        }
    }

    void Action::Clear()
    {
        _state           = 0.0f;
        _prevState       = 0.0f;
        _ticksActive     = 0;
        _prevTicksActive = 0;
        _ticksInactive   = 0;
    }
}
