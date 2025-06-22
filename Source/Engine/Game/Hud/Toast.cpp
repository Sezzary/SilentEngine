#include "Framework.h"
#include "Engine/Game/Hud/Toast.h"

#include "Engine/Application.h"
#include "Engine/Services/Options.h"
#include "Engine/Services/Time.h"
#include "Utils/Utils.h"

using namespace Silent::Services;
using namespace Silent::Utils;

namespace Silent::Hud
{
    void ToastManager::Update()
    {
        for (auto& toast : _toasts)
        {
            // Update life.
            if (toast.Life <= 0)
            {
                continue;
            }
            toast.Life--;

            // TODO: Update position.

            // Update opacity.
            if (toast.Life <= SEC_TO_TICK(LIFE_SEC_START_FADING))
            {
                toast.Col.A() = toast.Life / (float)SEC_TO_TICK(LIFE_SEC_START_FADING);
            }
        }

        ClearInactiveToasts();
    }

    void ToastManager::Render()
    {
        const auto& options = g_App.GetOptions();
        if (!options->EnableToasts)
        {
            return;
        }

        // Submit toasts for rendering.
        auto pos = _stackPosition;
        for (const auto& toast : _toasts)
        {
            if (toast.Life <= 0)
            {
                continue;
            }

            //g_App.GetRenderer().SubmitText(toast.Message, toast.Position, toast.Col);
            // TODO: Update `pos`.
        }
    }

    void ToastManager::Add(const std::string& msg, const Color& color)
    {
        const auto& options = g_App.GetOptions();
        if (!options->EnableToasts)
        {
            _toasts.clear();
            return;
        }

        // If max count reached, remove oldest toast.
        if (_toasts.size() >= TOAST_COUNT_MAX)
        {
            _toasts.erase(_toasts.begin());
        }

        // TODO: Set position.

        // Create new toast.
        auto toast = Toast
        {
            .Message  = msg,
            .Col      = Color(color.R(), color.G(), color.B(), 1.0f),
            .Life     = SEC_TO_TICK(LIFE_SEC_MAX)
        };
        _toasts.push_back(toast);
    }

    void ToastManager::ClearInactiveToasts()
    {
        EraseIf(_toasts, [](const Toast& toast)
        {
            return toast.Life <= 0;
        });
    }
}
