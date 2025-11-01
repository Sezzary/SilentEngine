#pragma once

namespace Silent::Utils
{
    class Font
    {
    private:
        
    public:
        // =============
        // Constructors
        // =============

        Font() = default;

        // ==========
        // Utilities
        // ==========

        void Load(const std::filesystem::path& fontPath);
    };
}
