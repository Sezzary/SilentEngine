#include "Framework.h"
#include "Engine/Renderer/Common/Utils.h"

#include "Engine/Application.h"
#include "Engine/Renderer/Common/Enums.h"
#include "Engine/Renderer/Renderer.h"

namespace Silent::Renderer
{
    Vector2 GetAspectCorrectScreenPosition(const Vector2 pos, ScaleMode scaleMode)
    {
        constexpr float RETRO_SCREEN_SPACE_ASPECT = RETRO_SCREEN_SPACE_RES.x / RETRO_SCREEN_SPACE_RES.y;

        if (scaleMode == ScaleMode::Stretch)
        {
            return pos;
        }

        const auto& renderer = g_App.GetRenderer();

        auto  screenRes        = renderer.GetScreenResolution().ToVector2();
        float screenResAspect  = screenRes.x / screenRes.y;
        float retroAspectDelta = screenResAspect / RETRO_SCREEN_SPACE_ASPECT; // @todo Need to consider some weird consequences with using this.
        
        // Compute aspect correction.
        auto aspectCorrection = Vector2::One;
        switch (scaleMode)
        {
            default:
            case ScaleMode::Fit:
            {
                if (screenResAspect >= 1.0f)
                {
                    aspectCorrection.x = 1.0f / screenResAspect;
                }
                else
                {
                    aspectCorrection.y = screenResAspect;
                }
                break;
            }
            case ScaleMode::Fill:
            {
                break;
            }
            case ScaleMode::Stretch:
            {
                break;
            }
        }

        return pos * aspectCorrection;
    }

    Vector2 GetAspectCorrectScreenPosition(const Vector2& pos)
    {
        constexpr float RETRO_SCREEN_SPACE_ASPECT = RETRO_SCREEN_SPACE_RES.x / RETRO_SCREEN_SPACE_RES.y;

        const auto& renderer = g_App.GetRenderer();

        auto  screenRes       = renderer.GetScreenResolution().ToVector2();
        float screenResAspect = screenRes.x / screenRes.y;
        float aspectDelta     = screenResAspect - RETRO_SCREEN_SPACE_ASPECT;

        auto correctedPos = pos;
        if (aspectDelta > EPSILON)
        {
            correctedPos.x *= 1.0f - (aspectDelta / 2.0f);
        }
        else if (aspectDelta < -EPSILON)
        {
            correctedPos.y *= 1.0f - (aspectDelta / 2.0f);
        }
        return correctedPos;
    }

    Vector2 ConvertScreenPositionToNdc(const Vector2& pos)
    {
        return Vector2(((pos.x * 2) / SCREEN_SPACE_RES.x) - 1.0f,
                       1.0f - ((pos.y * 2) / SCREEN_SPACE_RES.y));
    }

    Vector2 ConvertNdcToScreenPosition(const Vector2& ndc)
    {
        return Vector2(((ndc.x + 1.0f) * SCREEN_SPACE_RES.x) / 2,
                       ((1.0f - ndc.y) * SCREEN_SPACE_RES.y) / 2);
    }
}
