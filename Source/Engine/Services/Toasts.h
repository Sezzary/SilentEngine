#pragma once

namespace Silent::Services
{
    /** @brief Toast data. */
    struct Toast
    {
        std::string Message  = {};
        Color       Col      = Color::Black;
        int         Life     = 0;
    };

    /** @brief Central toast manager. */
    class ToastManager
    {
    private:
        // Constants

        static constexpr uint  TOAST_COUNT_MAX       = 8;
        static constexpr float LIFE_SEC_MAX          = 4.0f;
        static constexpr float LIFE_SEC_START_FADING = LIFE_SEC_MAX * 0.75f;

        // Fields

        std::vector<Toast> _toasts = {}; /** Active toasts. */

    public:
        // Constructors

        ToastManager() = default;

        // Utilities

        void Update();
        void Add(const std::string& msg, const Color& color = Color::White);

    private:
        // Helpers

        void Render();
    };
}
