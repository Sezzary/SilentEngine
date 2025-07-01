#include "Framework.h"
#include "Engine/Services/Toasts.h"

#include "Engine/Application.h"
#include "Engine/Services/Options.h"
#include "Engine/Services/Time.h"
#include "Utils/Utils.h"

using namespace Silent::Utils;

namespace Silent::Services
{
    void ToastManager::Update()
    {
        for (auto& toast : _toasts)
        {
            if (toast.Life <= 0)
            {
                continue;
            }

            // Update opacity.
            if (toast.Life <= SEC_TO_TICK(LIFE_SEC_START_FADING))
            {
                toast.Col.A() = toast.Life / (float)SEC_TO_TICK(LIFE_SEC_START_FADING);
            }

            // Update life.
            toast.Life--;
        }

        ClearInactiveToasts();
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
            _toasts.pop_back();
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

    // TODO
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

    void ToastManager::ClearInactiveToasts()
    {
        EraseIf(_toasts, [](const Toast& toast)
        {
            return toast.Life <= 0;
        });
    }
}
