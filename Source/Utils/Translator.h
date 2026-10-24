#pragma once

namespace Silent::Utils
{
    /** @brief Language locale code. */
    struct LocaleCode
    {
        std::string Language = {};
        std::string Country  = {};
    };

    /** @brief Language locale metadata. */
    struct LocaleMetadata
    {
        std::string Name    = {};
        std::string Label   = {};
        std::string Comment = {};

        LocaleCode Code     = {};
        int        Priority = 0;
    };

    /** @brief Translator for internationalized text. */
    class TranslationManager
    {
        // =======
        // Fields
        // =======

        json        _activeLocaleJson = {};
        std::string _activeLocaleName = {};
        std::string _queuedLocaleName = {};

        std::vector<LocaleMetadata> _locales       = {};
        stdfs::path                 _localesFolder = {};
        bool                        _isLocked      = false;

    public:
        // =============
        // Constructors
        // =============

        /** @brief Creates a default uninitialized instance. */
        TranslationManager() = default;

        /** @brief Initializes the instance with locales from a path.
         *
         * @param localesFolder Path to folder containing locale folders.
         */
        void Initialize(const stdfs::path& localesFolder);

        // ========
        // Getters
        // ========

        /** @brief Gets the active locale name.
         *
         * @return Active locale name.
         */
        const std::string& GetActiveLocaleName() const;

        /** @brief Gets the registered locales metadata ordered by system preference.
         *
         * @return Registered locales metadata.
         */
        const std::vector<LocaleMetadata>& GetLocales() const;

        // ========
        // Setters
        // ========

        /** @brief Sets the requested locale as the active one. If the translator is unlocked, it's set immediately.
         * If locked, the requested locale is queued to be set on the next `Unlock` call.
         *
         * @param localeName Name of the requested locale to set.
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

        /** @brief Checks if a locale is registered.
         *
         * @param localeName Name of the locale to find.
         * @return `true` if the locale is registered, `false` otherwise.
         */
        bool ContainsLocale(const std::string& localeName) const;

        /** @brief Registers a new locale.
         *
         * @param localeFolder Folder of the new locale.
         */
        void AddLocale(const stdfs::path& localeFolder);

        /** @brief Loads and sets a new active locale.
         *
         * @param localeName Name of the new locale.
         */
        void LoadActiveLocale(const std::string& localeName);
    };

    /** @brief Gets the locale codes of the current system in order of preference.
     *
     * @return System locale codes.
     */
    std::vector<LocaleCode> GetSystemLocaleCodes();
}
