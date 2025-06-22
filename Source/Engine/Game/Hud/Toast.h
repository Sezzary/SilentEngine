#pragma once

namespace Silent::Hud
{
    struct Toast
    {
        std::string Message  = {};
        Color       Col      = Color::Black;
        int         Life     = 0;
    };

    class ToastManager
    {
    private:
        // Constants

        static constexpr uint  TOAST_COUNT_MAX       = 4;
        static constexpr float LIFE_SEC_MAX          = 4.0f;
        static constexpr float LIFE_SEC_START_FADING = LIFE_SEC_MAX * 0.25f;

        // Fileds

        std::vector<Toast> _toasts        = {};
        Vector2            _stackPosition = Vector2::Zero;

    public:
        // Constructors

        ToastManager() = default;

        // Utilities

        void Update();
        void Render();
        void Add(const std::string& msg, const Color& color);

    private:
        // Helpers

        void ClearInactiveToasts();
    };
}
