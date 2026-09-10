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

    std::vector<char32> GetUtf8CodePoints(const std::string& msg)
    {
        if (msg.empty())
        {
            return {};
        }

        // Reserve minimum size.
        auto codePoints = std::vector<char32>{};
        codePoints.reserve((msg.size() / 4) + 1);

        // Collect code points.
        utf8::utf8to32(msg.begin(), msg.end(), std::back_inserter(codePoints));
        return codePoints;
    }

    std::string GetUtf8Substring(const std::string& msg, int pos, int count)
    {
        if (msg.empty() || pos < 0 || count <= 0)
        {
            return {};
        }

        auto itStart = msg.begin();
        auto itEnd   = msg.end();

        // Advance to substring start.
        utf8::advance(itStart, pos, itEnd);

        // Advance to substring end.
        auto itSubEnd = itStart;
        utf8::advance(itSubEnd, count, itEnd);

        return std::string(itStart, itSubEnd);
    }
}
