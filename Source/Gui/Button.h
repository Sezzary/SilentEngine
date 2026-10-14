#pragma once

#include "Input/Input.h"
#include "Renderer/Common/Enums.h"

using namespace Silent::Input;
using namespace Silent::Renderer;

namespace Silent::Gui
{
    /** @brief Interactable screen space region. */
    class Button
    {
    private:
        // ========
        // Aliases
        // ========

        using Callback = std::function<void()>;

        // =======
        // Fields
        // =======

        bool      _prevState = false;
        ScaleMode _scaleMode = ScaleMode::VerticalEdge;

        Callback _onEnter   = nullptr;
        Callback _onInside  = nullptr;
        Callback _onLeave   = nullptr;
        Callback _onOutside = nullptr;
        Callback _onClick   = nullptr;
        Callback _onHold    = nullptr;
        Callback _onRelease = nullptr;

    public:
        Vector2 Center  = Vector2::Zero;
        Vector2 Extents = Vector2::Zero;

        // =============
        // Constructors
        // =============

        /** @brief Constructs and uninitialized default instance. */
        Button() = default;

        /** @brief Creates an instance from a center and extents in screen percent with callbacks.
         *
         * @param center Center in screen percent.
         * @param extents Extents in screen percent.
         * @param scaleMode Extents scale mode relative to screen aspect ratio.
         * @param onEnter Interaction callback to execute when entering.
         * @param onInside Interaction callback to execute when inside.
         * @param onLeave Interaction callback to execute when leaving.
         * @param onOutside Interaction callback to execute when outside.
         * @param onClick Interaction callback to execute when input is clicked.
         * @param onHold Interaction callback to execute when input is held.
         * @param onRelease Interaction callback to execute when input is released.
         */
        Button(const Vector2& center, const Vector2& extents, ScaleMode scaleMode,
               Callback onEnter,
               Callback onInside,
               Callback onLeave,
               Callback onOutside,
               Callback onClick,
               Callback onHold,
               Callback onRelease);

        /** @brief Creates an instance from a center and extents in retro screen coordinates with callbacks.
         *
         * @param center Screen space center in retro screen coordinates (320x240).
         * @param extents Screen space extents in retro screen coordinates (320x240).
         * @param scaleMode Extents scale mode relative to screen aspect ratio.
         * @param onEnter Interaction callback to execute when entering.
         * @param onInside Interaction callback to execute when inside.
         * @param onLeave Interaction callback to execute when leaving.
         * @param onOutside Interaction callback to execute when outside.
         * @param onClick Interaction callback to execute when input is clicked.
         * @param onHold Interaction callback to execute when input is held.
         * @param onRelease Interaction callback to execute when input is released.
         */
        Button(const Vector2i& center, const Vector2i& extents, ScaleMode scaleMode,
               Callback onEnter,
               Callback onInside,
               Callback onLeave,
               Callback onOutside,
               Callback onClick,
               Callback onHold,
               Callback onRelease);

        // ========
        // Setters
        // ========

        /** @brief Sets the button center.
         *
         * @param center New center.
         */
        void SetCenter(const Vector2& center);

        /** @brief Sets the button extents.
         *
         * @param extents New extents.
         */
        void SetExtents(const Vector2& extents);

        // ==========
        // Utilities
        // ==========

        /** @brief Updates the button state using a discrete focus state from input navigation and executes callbacks.
         *
         * @param isFocused `true` if the button is focused, `false` otherwise.
         */
        void Update(bool isFocused);

        /** @brief Updates the button state using a screen space position from cursor navigation and executes callbacks.
         *
         * @param pos Screen space cursor position.
         */
        void Update(const Vector2& pos);

        /** @brief Displays debug information in the debug menu. */
        void Debug() const;

    private:
        // ========
        // Helpers
        // ========

        /** @brief Checks if an input action in a given list is clicked.
         *
         * @param actionIds IDs of input actions to check.
         * @return `true` if an action is clicked, `false` otherwise.
         */
        bool IsActionClicked(const std::vector<ActionId>& actionIds) const;

        /** @brief Checks if an input action in a given list is held.
         *
         * @param actionIds IDs of input actions to check.
         * @return `true` if an action is held, `false` otherwise.
         */
        bool IsActionHeld(const std::vector<ActionId>& actionIds) const;

        /** @brief Checks if an input action in a given list is released.
         *
         * @param actionIds IDs of input actions to check.
         * @return `true` if an action is released, `false` otherwise.
         */
        bool IsActionReleased(const std::vector<ActionId>& actionIds) const;

        /** @brief Updates the button state using a screen space point from mouse or controller cursor navigation and executes callbacks.
         *
         * @param state `true` if the button is active, `false` otherwise.
         * @param selectActionIds Input action IDs used for selection.
         */
        void Update(bool state, const std::vector<ActionId>& selectActionIds);

        /** @brief Executes an interaction callback.
         *
         * @param callback Interaction callback.
         */
        void ExecuteCallback(const Callback& callback) const;
    };
}
