#pragma once

namespace Silent::Utils
{
    /** @brief Translator for internationalized text. */
    class TranslationManager
    {
        // =======
        // Fields
        // =======

        // @todo Load one at a time to prevent memory overallocation.

        std::unordered_map<std::string, json> _locales      = {}; /** Key = locale name, value = locale translations. */
        std::string                           _activeLocale = {};
        std::optional<std::string>            _queuedLocale = std::nullopt;
        bool                                  _isLocked     = false;

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

        /** @brief Gets the currently used locale.
         *
         * @return Active locale.
         */
        const std::string& GetActiveLocale() const;

        // ========
        // Setters
        // ========

        /** @brief Sets the active locale if the translator is unlocked.
         * If locked, the new locale will be queued and set when `Unlock` is called.
         *
         * @param locale New active locale to set.
         * @return `true` if the new locale is valid, `false` otherwise.
         */
        bool SetLocale(const std::string& locale);

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
    };
}
