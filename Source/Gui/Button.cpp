#include "Framework.h"
#include "Gui/Button.h"

#include "Application.h"
#include "Input/Input.h"
#include "Renderer/Common/Enums.h"
#include "Renderer/Common/Utils.h"
#include "Utils/Utils.h"

using namespace Silent::Input;
using namespace Silent::Renderer;
using namespace Silent::Utils;

namespace Silent::Gui
{
    Button::Button(const Vector2& center, const Vector2& extents, ScaleMode scaleMode,
                   Callback onEnter,
                   Callback onInside,
                   Callback onLeave,
                   Callback onOutside,
                   Callback onClick,
                   Callback onHold,
                   Callback onRelease)
    {
        _prevState = false;

        _scaleMode = scaleMode;
        _onEnter   = std::move(onEnter);
        _onInside  = std::move(onInside);
        _onLeave   = std::move(onLeave);
        _onOutside = std::move(onOutside);
        _onClick   = std::move(onClick);
        _onHold    = std::move(onHold);
        _onRelease = std::move(onRelease);

        Center  = center;
        Extents = extents;
    }

    Button::Button(const Vector2i& center, const Vector2i& extents, ScaleMode scaleMode,
                   Callback onEnter,
                   Callback onInside,
                   Callback onLeave,
                   Callback onOutside,
                   Callback onClick,
                   Callback onHold,
                   Callback onRelease)
    {
        *this = Button(ConvertRetroScreenPixelsToPercent(center),
                       ConvertRetroScreenPixelsToPercent(extents),
                       std::move(scaleMode),
                       std::move(onEnter),
                       std::move(onInside),
                       std::move(onLeave),
                       std::move(onOutside),
                       std::move(onClick),
                       std::move(onHold),
                       std::move(onRelease));
    }

    void Button::Update(bool isFocused)
    {
        static const auto SELECT_ACTION_IDS = std::vector<ActionId>
        {
            In::Enter,
            In::Action
        };

        Update(isFocused, SELECT_ACTION_IDS);
    }

    void Button::Update(const Vector2& pos)
    {
        static const auto SELECT_ACTION_IDS = std::vector<ActionId>
        {
            In::MouseClickLeft
        };

        auto correctedExtents = Extents * GetScreenAspectCorrection(_scaleMode);
        auto bounds           = AxisAlignedBoundingRect(Center, correctedExtents);
        Update(bounds.Intersects(pos), SELECT_ACTION_IDS); 
    }

    void Button::SetCenter(const Vector2& center)
    {
        Center = center;
    }

    void Button::SetExtents(const Vector2& extents)
    {
        Extents = extents;
    }

    void Button::Debug() const
    {
        // @todo Visualise button.
        //Debug::CreateRectangle();
    }

    bool Button::IsActionClicked(const std::vector<ActionId>& actionIds) const
    {
        const auto& input = g_App.GetInput();

        for (auto actionId : actionIds)
        {
            if (input.GetAction(actionId).IsClicked(ACTION_HALF_STATE))
            {
                return true;
            }
        }

        return false;
    }

    bool Button::IsActionHeld(const std::vector<ActionId>& actionIds) const
    {
        const auto& input = g_App.GetInput();

        for (auto actionId : actionIds)
        {
            if (input.GetAction(actionId).IsHeld(0.0f, ACTION_HALF_STATE))
            {
                return true;
            }
        }

        return false;
    }

    bool Button::IsActionReleased(const std::vector<ActionId>& actionIds) const
    {
        const auto& input = g_App.GetInput();

        for (auto actionId : actionIds)
        {
            if (input.GetAction(actionId).IsReleased(0.0f, ACTION_HALF_STATE))
            {
                return true;
            }
        }

        return false;
    }

    void Button::Update(bool state, const std::vector<ActionId>& selectActionIds)
    {
        if (state)
        {
            // Execute `_onEnter` callback if previous state was inactive.
            if (!_prevState)
            {
                ExecuteCallback(_onEnter);
            }
            // Execute `_onInside` callback if previous state was also active.
            else
            {
                ExecuteCallback(_onInside);
            }

            // Execute `_onClick`, `_onHold`, or `_onRelease` callback if select action is registered.
            if (IsActionClicked(selectActionIds))
            {
                ExecuteCallback(_onClick);
            }
            else if (IsActionHeld(selectActionIds))
            {
                ExecuteCallback(_onHold);
            }
            else if (IsActionReleased(selectActionIds))
            {
                ExecuteCallback(_onRelease);
            }
        }
        else
        {
            // Execute `_onLeave` callback if previous state was active.
            if (_prevState)
            {
                ExecuteCallback(_onLeave);
            }
            // Execute `_onOutside` callback if previous state was also inactive.
            else
            {
                ExecuteCallback(_onOutside);
            }
        }

        _prevState = state;
    }

    void Button::ExecuteCallback(const Callback& callback) const
    {
        if (callback)
        {
            callback();
        }
    }
}
