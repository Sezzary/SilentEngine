#pragma once

namespace Silent::Utils
{
    /** @brief Translator for internationalized text. */
    class TranslationManager
    {
        // =======
        // Fields
        // =======

        json        _activeLocale     = {};
        std::string _activeLocaleName = {};
        std::string _queuedLocaleName = {};
        bool        _isLocked         = false;

        std::vector<std::string> _localeNames = {};
        std::filesystem::path    _localesPath = {};

    public:
        // =============
        // Constructors
        // =============

        /** @brief Constructs an uninitialized default `Translator`. */
        TranslationManager() = default;

        /** @brief Initializes the `Translator` with locales from a path.
         *
         * @param localesPath Path containing locale folders.
         * @param localeNames Locale names to register.
         */
        void Initialize(const std::filesystem::path& localesPath, const std::vector<std::string>& localeNames);

        // ========
        // Getters
        // ========

        /** @brief Gets the translated text for the active locale using a given translation key.
         *
         * @param translationKey Translation key.
         * @return Translated text.
         */
        std::string GetTranslation(const std::string& translationKey) const;

        /** @brief Gets the name of the active locale.
         *
         * @return Active locale name.
         */
        const std::string& GetActiveLocaleName() const;

        /** @brief Gets the registered locale names.
         *
         * @return Registered locale names.
         */
        const std::vector<std::string>& GetLocaleNames() const;

        // ========
        // Setters
        // ========

        /** @brief Sets the active locale. If the translator is unlocked, it's set immediately.
         * If locked, it queues the new locale to be set on the next `Unlock` call.
         *
         * @param localeName Name of the new locale to set.
         */
        void SetActiveLocale(const std::string& localeName);

        // ==========
        // Utilities
        // ==========

        /** @brief Locks the translator to disallow setting the active locale. */
        void Lock();

        /** @brief Unlocks the translator to allow setting the active locale. If a new locale is queued, it becomes active. */
        void Unlock();

        // ==========
        // Operators
        // ==========

        std::string operator()(const std::string& translationKey) const;

    private:
        // ========
        // Helpers
        // ========

        /** @brief Loads and sets a new active locale.
         *
         * @param localeName Name of the new locale.
         */
        void LoadActiveLocale(const std::string& localeName);
    };
}
