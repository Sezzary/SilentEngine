#include "Framework.h"
#include "Engine/Services/Hud/Hud.h"

#include "Engine/Services/Hud/Toast.h"

namespace Silent::Services
{
    HudManager g_Hud = HudManager();

    void HudManager::Update()
    {
        _toasts.Update();
    }

    void HudManager::AddToast(const std::string& msg, const Color& color)
    {
        _toasts.Add(msg, color);
    }
}
