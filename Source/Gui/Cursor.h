#pragma once

namespace Silent::Gui
{
    /** @brief GUI game cursor. */
    class Cursor
    {
    private:
        // =======
        // Fields
        // =======

        bool    _isEnabled = false;
        Vector2 _position  = Vector2::Zero;
    
    public:
        // ========
        // Getters
        // ========

        const Vector2& GetPosition() const;

        // ==========
        // Inquirers
        // ==========

        /** @brief Checks if the game cursor is enabled. */
        bool IsEnabled() const;

        // ==========
        // Utilities
        // ==========

        /** @brief Updates the game cursor. */
        void Update();

        /** @brief Submits the game cursor sprite to draw. */
        void Draw();
    };
}
