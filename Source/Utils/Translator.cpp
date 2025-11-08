#include "Framework.h"
#include "Utils/Translator.h"

#include "Services/Filesystem.h"
#include "Utils/Stream.h"
#include "Utils/Utils.h"

using namespace Silent::Services;

namespace Silent::Utils
{
    void TranslationManager::Initialize(const std::filesystem::path& localesPath, const std::vector<std::string>& localeNames)
    {
        constexpr char LOCALE_FILENAME[] = "Locale";

        // Set path.
        _localesPath = localesPath;

        // Register locale names.
        _localeNames = localeNames;
        if (_localeNames.empty())
        {
            Debug::Log("No translator locales registered.", Debug::LogLevel::Warning);
            return;
        }

        // Set first locale as default.
        LoadActiveLocale(_localeNames.front());
    }

    std::string TranslationManager::GetTranslation(const std::string& translationKey) const
    {
        // Get translated text or use translation key as fallback if it doesn't exist.
        return _activeLocale.value(translationKey, translationKey);
    }

    const std::string& TranslationManager::GetActiveLocaleName() const
    {
        return _activeLocaleName;
    }

    const std::vector<std::string>& TranslationManager::GetLocaleNames() const
    {
        return _localeNames;
    }

    void TranslationManager::SetActiveLocale(const std::string& localeName)
    {
        // Check if new locale is already active or queued.
        if (localeName == _activeLocaleName ||
            localeName == _queuedLocaleName)
        {
            return;
        }

        // Check if new locale is registered.
        if (!Contains(_localeNames, localeName))
        {
            Debug::Log(fmt::format("Attempted to set unregistered translator locale `{}`.", localeName), Debug::LogLevel::Warning);
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
        return GetTranslation(translationKey);
    }

    void TranslationManager::LoadActiveLocale(const std::string& localeName)
    {
        constexpr char LOCALE_FILENAME[] = "Locale";

        // Open locale file stream and set new locale.
        auto localePath = _localesPath / localeName / (std::string(LOCALE_FILENAME) + JSON_FILE_EXT);
        auto stream     = Stream(localePath, true, false);
        if (!stream.IsOpen())
        {
            Debug::Log(fmt::format("Failed to load translator locale `{}`.", localeName), Debug::LogLevel::Warning);
            _activeLocale = json();
        }
        else
        {
            _activeLocale = stream.ReadJson();
        }

        _activeLocaleName = localeName;
        stream.Close();
    }
}
