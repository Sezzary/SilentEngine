#include "Framework.h"
#include "Utils/Utils.h"

#include "Engine/Application.h"
#include "Engine/Renderer/Renderer.h"

using namespace Silent::Renderer;

namespace Silent::Utils
{
    std::string ToUpper(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), [](uchar c)
        {
            return std::toupper(c);
        });

        return str;
    }

    std::string ToLower(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), [](uchar c)
        {
            return std::tolower(c);
        });

        return str;
    }

    char* CopyString(const char src[], uint size)
    {
        char* dest = (char*)malloc((size + 1) * sizeof(char));
        if (dest != nullptr)
        {
            strcpy(dest, src);
        }

        return (char*)dest;
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
            correctedPos.x *= 1.0f - (aspectDelta / 2);
        }
        else if (aspectDelta < -EPSILON)
        {
            correctedPos.y *= 1.0f - (aspectDelta / 2);
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
