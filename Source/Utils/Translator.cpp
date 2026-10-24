#include "Framework.h"
#include "Utils/Translator.h"

#include "Services/Filesystem.h"
#include "Utils/Stream.h"
#include "Utils/Utils.h"

using namespace Silent::Services;

namespace Silent::Utils
{
    void TranslationManager::Initialize(const stdfs::path& localesFolder)
    {
        // Set folder path.
        _localesFolder = localesFolder;
        if (!stdfs::exists(_localesFolder) || !stdfs::is_directory(_localesFolder))
        {
            Debug::Log("Locales folder is missing.", Debug::LogLevel::Error);
            return;
        }

        // Register new locales.
        _locales.clear();
        for (const auto& localeFolder : stdfs::directory_iterator(_localesFolder))
        {
            if (!localeFolder.is_directory())
            {
                continue;
            }

            AddLocale(localeFolder);
        }

        // Check if any locales have been registered.
        if (_locales.empty())
        {
            Debug::Log("No locales registered.", Debug::LogLevel::Error);
            return;
        }

        // Sort locales by system preference, priority, language code, country code, and name.
        auto sysCodes = GetSystemLocaleCodes();
        Sort(_locales, [&](const LocaleMetadata& locale0, const LocaleMetadata& locale1)
        {
            auto MatchLang = [&](const std::string& lang)
            {
                return FindIf(sysCodes, [&](const auto& sysCode)
                {
                    return sysCode.Language == lang;
                });
            };

            const auto* sysCodeIt0  = MatchLang(locale0.Code.Language);
            const auto* sysCodeIt1  = MatchLang(locale1.Code.Language);
            bool        hasSysLang0 = (sysCodeIt0 != nullptr);
            bool        hasSysLang1 = (sysCodeIt1 != nullptr);

            if (hasSysLang0 != hasSysLang1)
            {
                return hasSysLang0;
            }
            else if (hasSysLang0 && hasSysLang1)
            {
                if (sysCodeIt0->Language != sysCodeIt1->Language)
                {
                    return sysCodeIt0->Language < sysCodeIt1->Language;
                }
                else if (sysCodeIt0->Country != sysCodeIt1->Country)
                {
                    return sysCodeIt0->Country < sysCodeIt1->Country;
                }
            }

            if (locale0.Priority != locale1.Priority)
            {
                return locale0.Priority < locale1.Priority;
            }
            else if (locale0.Code.Language != locale1.Code.Language)
            {
                return locale0.Code.Language < locale1.Code.Language;
            }
            else if (locale0.Code.Country != locale1.Code.Country)
            {
                return locale0.Code.Country < locale1.Code.Country;
            }

            return locale0.Name < locale1.Name;
        });

        // Set first locale as default.
        LoadActiveLocale(_locales.front().Name);
    }

    const std::string& TranslationManager::GetActiveLocaleName() const
    {
        return _activeLocaleName;
    }

    const std::vector<LocaleMetadata>& TranslationManager::GetLocales() const
    {
        return _locales;
    }

    void TranslationManager::SetActiveLocale(const std::string& localeName)
    {
        // Check if requested locale is already active or queued.
        if (localeName == _activeLocaleName ||
            localeName == _queuedLocaleName)
        {
            return;
        }

        // Check if requested locale is registered.
        if (!ContainsLocale(localeName))
        {
            Debug::Log(Fmt("Attempted to set unregistered locale `{}`.", localeName), Debug::LogLevel::Error);
            return;
        }

        // Load requested locale or queue to load when translator becomes unlocked.
        if (_isLocked)
        {
            _queuedLocaleName = localeName;
        }
        else
        {
            LoadActiveLocale(localeName);
        }
    }

    void TranslationManager::Lock()
    {
        _isLocked = true;
    }

    void TranslationManager::Unlock()
    {
        _isLocked = false;

        // Load new queued locale.
        if (!_queuedLocaleName.empty())
        {
            LoadActiveLocale(_queuedLocaleName);
            _queuedLocaleName = {};
        }
    }

    std::string TranslationManager::operator()(const std::string& translationKey) const
    {
        if (_activeLocaleJson.empty())
        {
            return translationKey;
        }

        return _activeLocaleJson.value(translationKey, translationKey);
    }

    bool TranslationManager::ContainsLocale(const std::string& localeName) const
    {
        for (const auto& locale : _locales)
        {
            if (locale.Name != localeName)
            {
                continue;
            }

            return true;
        }

        return false;
    }

    void TranslationManager::AddLocale(const stdfs::path& localeFolder)
    {
        constexpr char KEY_LABEL[]        = "Label";
        constexpr char KEY_COMMENT[]      = "Comment";
        constexpr char KEY_LANG_CODE[]    = "LanguageCode";
        constexpr char KEY_COUNTRY_CODE[] = "CountryCode";
        constexpr char KEY_PRIORITY[]     = "Priority";

        auto localeName   = localeFolder.filename().string();
        auto metadataFile = localeFolder / Fmt("{}{}", ASSETS_LOCALE_METADATA_FILENAME, JSON_FILE_EXT);

        // Open metadata JSON.
        auto metadataJson = json();
        auto stream       = Stream(metadataFile, true, false);
        if (!stream.IsOpen())
        {
            Debug::Log(Fmt("Failed to load locale metadata `{}`.", localeName), Debug::LogLevel::Error);
        }
        else
        {
            metadataJson = stream.ReadJson();
        }

        // Create and return metadata.
        _locales.push_back(LocaleMetadata
        {
            .Name    = localeName,
            .Label   = metadataJson.value(KEY_LABEL, ""),
            .Comment = metadataJson.value(KEY_COMMENT, ""),
            .Code    = LocaleCode
            {
                .Language = metadataJson.value(KEY_LANG_CODE, ""),
                .Country  = metadataJson.value(KEY_COUNTRY_CODE, "")
            },
            .Priority = metadataJson.value(KEY_PRIORITY, INT_MAX)
        });
    }

    void TranslationManager::LoadActiveLocale(const std::string& localeName)
    {
        // Open locale JSON and set as active.
        auto localePath = _localesFolder / localeName / Fmt("{}{}", ASSETS_LOCALE_FILENAME, JSON_FILE_EXT);
        auto stream     = Stream(localePath, true, false);
        if (!stream.IsOpen())
        {
            Debug::Log(Fmt("Failed to load language locale `{}`.", localeName), Debug::LogLevel::Warning);
            _activeLocaleJson = json();
        }
        else
        {
            _activeLocaleJson = stream.ReadJson();
        }

        _activeLocaleName = localeName;
    }

    std::vector<LocaleCode> GetSystemLocaleCodes()
    {
        // Get locales.
        int    localeCount = 0;
        auto** locales     = SDL_GetPreferredLocales(&localeCount);
        if (locales == nullptr)
        {
            return {};
        }

        // Collect codes.
        auto codes = std::vector<LocaleCode>{};
        for (int i = 0; i < localeCount; i++)
        {
            const auto& locale = *locales[i];

            codes.push_back(LocaleCode
            {
                .Language = locale.language,
                .Country  = (locale.country != nullptr) ? locale.country : EMPTY_STRING
            });
        }

        SDL_free(locales);
        return codes;
    }
}
