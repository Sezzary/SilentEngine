#include "Framework.h"
#include "Engine/Game/Hud/Hud.h"

#include "Engine/Game/Hud/Toast.h"

namespace Silent::Hud
{
    HudManager g_Hud = HudManager();

    void HudManager::Update()
    {
        _toasts.Update();
    }

    void HudManager::Render()
    {
        _toasts.Render();
    }

    void HudManager::AddToast(const std::string& msg, const Color& color)
    {
        _toasts.Add(msg, color);
    }
}
