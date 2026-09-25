#include "Framework.h"
#include "Utils/Utils.h"

#include "Application.h"
#include "Renderer/Renderer.h"

using namespace Silent::Renderer;

namespace Silent::Utils
{
    std::string ToUpper(const std::string& str)
    {
        auto transformedStr = str;
        std::transform(transformedStr.begin(), transformedStr.end(), transformedStr.begin(), [](uchar c)
        {
            return std::toupper(c);
        });

        return transformedStr;
    }

    std::string ToLower(const std::string& str)
    {
        auto transformedStr = str;
        std::transform(transformedStr.begin(), transformedStr.end(), transformedStr.begin(), [](uchar c)
        {
            return std::tolower(c);
        });

        return transformedStr;
    }

    char* CopyString(const char src[], int size)
    {
        char* dest = (char*)malloc((size + 1) * sizeof(char));
        if (dest != nullptr)
        {
            strcpy(dest, src);
        }

        return (char*)dest;
    }

    std::vector<char32> GetUtf8CodePoints(const std::string& str)
    {
        if (str.empty())
        {
            return {};
        }

        // Reserve minimum size.
        auto codePoints = std::vector<char32>{};
        codePoints.reserve((str.size() / 4) + 1);

        // Collect code points.
        utf8::utf8to32(str.begin(), str.end(), std::back_inserter(codePoints));
        return codePoints;
    }

    std::string GetUtf8Substring(const std::string& str, int pos, int count)
    {
        if (str.empty() || pos < 0 || count <= 0)
        {
            return {};
        }

        // Advance to substring start.
        auto itSubStart = str.begin();
        utf8::advance(itSubStart, pos, str.end());
        
        // Advance to substring end.
        auto itSubEnd = itSubStart;
        utf8::advance(itSubEnd, count, str.end());

        return std::string(itSubStart, itSubEnd);
    }
}
