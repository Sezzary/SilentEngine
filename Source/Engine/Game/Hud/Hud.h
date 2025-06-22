#pragma once

#include "Engine/Game/Hud/Toast.h"

namespace Silent::Hud
{
    class HudManager
    {
    private:
        // Fields

        ToastManager _toasts = ToastManager();

    public:
        // Constructors

        HudManager() = default;

        // Utilities

        void Update();
        void Render();

        void AddToast(const std::string& msg, const Color& color = Color::White);
    };

    extern HudManager g_Hud;
}
