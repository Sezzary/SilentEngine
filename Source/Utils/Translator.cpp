#include "Framework.h"
#include "Utils/Translator.h"

#include "Services/Filesystem.h"
#include "Utils/Stream.h"
#include "Utils/Utils.h"

using namespace Silent::Services;

namespace Silent::Utils
{
    void TranslationManager::Initialize(const std::filesystem::path localesPath)
    {
        constexpr char LOCALE_FILENAME[] = "Locale";

        // Set path.
        _localesPath = localesPath;

        // Register locale names sorted alphabetically.
        for (const auto& entry : std::filesystem::directory_iterator(_localesPath))
        {
            if (entry.is_directory())
            {
                _localeNames.push_back(entry.path().filename().string());
            }
        }
        Sort(_localeNames);

        if (_localeNames.empty())
        {
            Debug::Log("No translator locales found.", Debug::LogLevel::Warning);
            return;
        }

        // Set first locale as default.
        if (!_localeNames.empty())
        {
            LoadActiveLocale(_localeNames.front());
        }
    }

    std::string TranslationManager::GetTranslation(const std::string& translationKey) const
    {
        // Check if active locale is loaded.
        if (_activeLocale.empty())
        {
            return translationKey;
        }

        // Get translated text or use translation key as fallback if it doesn't exist.
        return _activeLocale.value(translationKey, translationKey);
    }

    void TranslationManager::SetLocale(const std::string& localeName)
    {
        // Check if new locale is already active or queued.
        if (localeName == _activeLocaleName ||
            (_queuedLocaleName.has_value() && localeName == *_queuedLocaleName))
        {
            return;
        }

        // Check if new locale is registered.
        if (!Contains(_localeNames, localeName))
        {
            Debug::Log("Attempted to set unregistered translator locale `" + localeName + "`.", Debug::LogLevel::Warning);
            return;
        }

        // Load new locale or queue to load when translator becomes unlocked.
        if (_isLocked)
        {
            _queuedLocaleName = localeName;
        }
        else
        {
            LoadActiveLocale(localeName);
        }

        return;
    }

    void TranslationManager::Lock()
    {
        _isLocked = true;
    }

    void TranslationManager::Unlock()
    {
        _isLocked = false;

        // Set new locale if queued.
        if (_queuedLocaleName.has_value())
        {
            LoadActiveLocale(*_queuedLocaleName);
            _queuedLocaleName = std::nullopt;
        }
    }

    std::string TranslationManager::operator()(const std::string& translationKey) const
    {
        return GetTranslation(translationKey);
    }

    void TranslationManager::LoadActiveLocale(const std::string& localeName)
    {
        constexpr char LOCALE_FILENAME[] = "Locale";

        // Open locale file stream.
        auto localePath = _localesPath / localeName / (std::string(LOCALE_FILENAME) + JSON_FILE_EXT);
        auto stream     = Stream(localePath, true, false);
        if (!stream.IsOpen())
        {
            Debug::Log("Failed to load `" + localeName + "` translator locale.", Debug::LogLevel::Warning);
            return;
        }

        // Set new locale.
        _activeLocale     = stream.ReadJson();
        _activeLocaleName = localeName;
        stream.Close();
    }
}
