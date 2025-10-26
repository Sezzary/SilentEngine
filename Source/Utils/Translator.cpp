#include "Framework.h"
#include "Utils/Translator.h"

#include "Utils/Stream.h"

namespace Silent::Utils
{
    void TranslationManager::Initialize(const std::filesystem::path localesPath, const std::vector<std::string>& localeNames)
    {
        // Collect locales.
        for (const auto& localeName : localeNames)
        {
            auto localePath = localesPath / localeName;
            auto stream     = Stream(localePath, true, false);
            if (!stream.IsOpen())
            {
                Debug::Log("Failed to load locale file `" + localePath.string() + "` for translator.", Debug::LogLevel::Warning);
                continue;
            }

            _locales[localeName] = stream.ReadJson();
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

    void TranslationManager::SetLocale(const std::string& locale)
    {
        auto it = _locales.find(locale);
        if (it == _locales.end())
        {
            Debug::Log("Attempted to set invalid locale `" + locale + "` for translator.", Debug::LogLevel::Warning);
            return;
        }

        if (_isLocked)
        {
            _queuedLocale = locale;
        }
        else
        {
            _activeLocale = locale;
        }
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
