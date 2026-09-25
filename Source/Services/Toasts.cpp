#include "Framework.h"
#include "Services/Toasts.h"

#include "Application.h"
#include "Renderer/Renderer.h"
#include "Services/Clock.h"
#include "Services/Options.h"
#include "Utils/Utils.h"

using namespace Silent::Renderer;
using namespace Silent::Utils;

namespace Silent::Services
{
    void ToastManager::Update()
    {
        const auto& options = g_App.GetOptions();

        // Check if toasts are enabled.
        if (!options->EnableToasts)
        {
            _toasts.clear();
            return;
        }

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
                toast.Col.A() = std::clamp((float)toast.Life / (float)SEC_TO_TICK(LIFE_SEC_START_FADING), 0.0f, 1.0f);
            }

            // Update life.
            toast.Life--;
        }

        // Erase inactive toasts.
        Erase(_toasts, [](const Toast& toast)
        {
            return toast.Life <= 0;
        });

        // Submit for rendering.
        Render();
    }

    void ToastManager::Add(const std::string& msg, const Color& color)
    {
        const auto& options = g_App.GetOptions();

        // Check if toasts are enabled.
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
        constexpr float TEXT_SCALE  = RETRO_PIXEL_SCALE.y * 10.0f;
        constexpr float TEXT_MARGIN = (SCREEN_SPACE_RES.y / RETRO_SCREEN_SPACE_RES.y) * 5.0f;
        constexpr float LINE_OFFSET = (SCREEN_SPACE_RES.y / RETRO_SCREEN_SPACE_RES.y) * 12.0f;

        const auto& options  = g_App.GetOptions();
        auto&       renderer = g_App.GetRenderer();

        // Check if toasts are enabled.
        if (!options->EnableToasts)
        {
            return;
        }

        // Submit text to renderer.
        auto linePos = Vector2(TEXT_MARGIN, LINE_OFFSET);
        for (const auto& toast : _toasts)
        {
            auto fontName = (options->TextQuality == TextQualityType::Retro) ? "RetroSans" : "ModernSans";
            auto text     = Text2d::CreateText2d(toast.Message, fontName,
                                                 linePos, DEG_TO_RAD(0.0f), TEXT_SCALE,
                                                 toast.Col, (int)TextStyleFlags::Shadow,
                                                 0, AlignMode::BottomLeft);
            renderer.SubmitText2d(text);

            linePos.y += LINE_OFFSET;
        }
    }
}
