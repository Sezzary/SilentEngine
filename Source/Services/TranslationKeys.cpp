#include "Framework.h"
#include "Services/TranslationKeys.h"

#include "Services/Options.h"

namespace Silent::Services
{
    const std::vector<std::string> LOCALE_NAMES
    {
        "English"
    };

    const std::unordered_map<LanguageType, std::string> LANGUAGE_LOCALES =
    {
        { LanguageType::English, LOCALE_NAMES[(int)LanguageType::English] }
    };
}
