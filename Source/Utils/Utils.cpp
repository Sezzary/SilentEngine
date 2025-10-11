#include "Framework.h"
#include "Utils/Utils.h"

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

    Vector2 ConvertPercentToNdc(const Vector2& pos)
    {
        return Vector2(((pos.x * 2) / SCREEN_SPACE_RES.x) - 1.0f,
                       1.0f - ((pos.y * 2) / SCREEN_SPACE_RES.y));
    }

    Vector2 ConvertNdcToPercent(const Vector2& ndc)
    {
        return Vector2(((ndc.x + 1.0f) * SCREEN_SPACE_RES.x) / 2,
                       ((1.0f - ndc.y) * SCREEN_SPACE_RES.y) / 2);
    }
}
