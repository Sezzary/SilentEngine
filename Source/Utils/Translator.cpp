#include "Framework.h"
#include "Utils/Translator.h"

#include "Utils/Stream.h"
#include "Utils/Utils.h"

namespace Silent::Utils
{
    void TranslationManager::Initialize(const std::filesystem::path localesPath)
    {
        constexpr char LOCALE_FILENAME[] = "Locale.json";

        // Collect locale names sorted alphabetically.
        auto localeNames = std::vector<std::string>{};
        for (const auto& entry : std::filesystem::directory_iterator(localesPath))
        {
            if (entry.is_directory())
            {
                localeNames.push_back(entry.path().filename().string());
            }
        }
        Sort(localeNames);

        if (localeNames.empty())
        {
            Debug::Log("Failed to initialize translator locales.", Debug::LogLevel::Warning);
            return;
        }

        // Collect locales.
        for (const auto& localeName : localeNames)
        {
            auto localePath = localesPath / localeName / LOCALE_FILENAME;
            auto stream     = Stream(localePath, true, false);
            if (!stream.IsOpen())
            {
                Debug::Log("Failed to initialize `" + localeName + "` translator locale.", Debug::LogLevel::Warning);
                continue;
            }

            _locales[localeName] = stream.ReadJson();
            stream.Close();
        }

        // Set first locale as default.
        if (!localeNames.empty())
        {
            _activeLocale = localeNames.front();
        }
    }

    std::string TranslationManager::GetTranslation(const std::string& translationKey) const
    {
        if (_locales.empty())
        {
            return translationKey;
        }

        // Get translated text or use translation key as fallback if it doesn't exist.
        const auto& locale = _locales.at(_activeLocale);
        return locale.value(translationKey, translationKey);
    }

    const std::string& TranslationManager::GetActiveLocale() const
    {
        return _activeLocale;
    }

    bool TranslationManager::SetLocale(const std::string& locale)
    {
        auto it = _locales.find(locale);
        if (it == _locales.end())
        {
            Debug::Log("Attempted to set uninitialized translator locale `" + locale + "`.", Debug::LogLevel::Warning);
            return false;
        }

        if (_isLocked)
        {
            _queuedLocale = locale;
        }
        else
        {
            _activeLocale = locale;
        }

        return true;
    }

    void TranslationManager::Lock()
    {
        _isLocked = true;
    }

    void TranslationManager::Unlock()
    {
        _isLocked = false;

        // Set new locale if queued.
        if (_queuedLocale.has_value())
        {
            _activeLocale = *_queuedLocale;
            _queuedLocale = std::nullopt;
        }
    }

    std::string TranslationManager::operator()(const std::string& translationKey) const
    {
        return GetTranslation(translationKey);
    }
}
