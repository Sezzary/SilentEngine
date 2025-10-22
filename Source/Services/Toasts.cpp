#include "Framework.h"
#include "Services/Toasts.h"

#include "Application.h"
#include "Renderer/Renderer.h"
#include "Services/Options.h"
#include "Services/Time.h"
#include "Utils/Utils.h"

using namespace Silent::Renderer;
using namespace Silent::Utils;

namespace Silent::Services
{
    void ToastManager::Update()
    {
        // Update toasts.
        for (auto& toast : _toasts)
        {
            if (toast.Life <= 0)
            {
                continue;
            }

            // Update opacity.
            if (toast.Life <= SEC_TO_TICK(LIFE_SEC_START_FADING))
            {
                toast.Col.A() = std::clamp(toast.Life / (float)SEC_TO_TICK(LIFE_SEC_START_FADING), 0.0f, 1.0f);
            }

            // Update life.
            toast.Life--;
        }

        // Erase inactive toasts.
        EraseIf(_toasts, [](const Toast& toast)
        {
            return toast.Life <= 0;
        });

        // Submit for rendering.
        Render();
    }

    void ToastManager::Add(const std::string& msg, const Color& color)
    {
        const auto& options = g_App.GetOptions();
        if (!options->EnableToasts)
        {
            return;
        }

        // If max count reached, remove oldest toast.
        if (_toasts.size() >= TOAST_COUNT_MAX)
        {
            _toasts.erase(_toasts.begin());
        }

        // Create new toast.
        auto toast = Toast
        {
            .Message  = msg,
            .Col      = Color(color.R(), color.G(), color.B(), 1.0f),
            .Life     = SEC_TO_TICK(LIFE_SEC_MAX)
        };
        _toasts.push_back(toast);
    }

    void ToastManager::Render()
    {
        constexpr float ROW_OFFSET = 2.0f;

        const auto& options = g_App.GetOptions();
        if (!options->EnableToasts)
        {
            return;
        }

        // Submit toasts to renderer.
        auto pos = Vector2(0.0f, ROW_OFFSET * _toasts.size());
        for (const auto& toast : _toasts)
        {
            //g_App.GetRenderer().SubmitText(toast.Message, toast.Position, toast.Col);
            pos.y -= ROW_OFFSET;
        }
    }
}
