#pragma once

namespace Silent::Utils
{
    /** @brief Translator for internationalized text. */
    class TranslationManager
    {
        // =======
        // Fields
        // =======

        json                       _activeLocale     = {};
        std::string                _activeLocaleName = {};
        std::vector<std::string>   _localeNames      = {};
        std::optional<std::string> _queuedLocaleName = std::nullopt;

        std::filesystem::path _localesPath = {};
        bool                  _isLocked    = false;

    public:
        // =============
        // Constructors
        // =============

        /** @brief Constructs an uninitialized default `Translator`. */
        TranslationManager() = default;

        /** @brief Initializes the `Translator` with locales from a path.
         *
         * @param localesPath Path containing locale folders.
         */
        void Initialize(const std::filesystem::path localesPath);

        // ========
        // Getters
        // ========

        /** @brief Gets the translated text for the active locale using a given translation key.
         *
         * @param translationKey Translation key.
         * @return Translated text.
         */
        std::string GetTranslation(const std::string& translationKey) const;

        // ========
        // Setters
        // ========

        /** @brief Sets the active locale if the translator is unlocked.
         * If locked, the new locale will be queued and set when `Unlock` is called.
         *
         * @param localeName Name of the new locale to set.
         */
        void SetLocale(const std::string& localeName);

        // ==========
        // Utilities
        // ==========

        /** @brief Locks the translator to disallow setting the active locale. */
        void Lock();

        /** @brief Unlocks the translator to allow setting the active locale. If a new locale is queued, it becomes the active locale. */
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
