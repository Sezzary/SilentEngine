#pragma once

namespace Silent::Services
{
    /** @brief Central audio manager. */
    class AudioManager
    {
    private:
        // =======
        // Fields
        // =======

        SDL_AudioStream* _stream = nullptr;

    public:
        // =============
        // Constructors
        // =============

        AudioManager() = default;

        // ==========
        // Utilities
        // ==========

        /** @brief Initializes streams for the audio manager. */
        void Initialize();

        /** @brief Gracefully deinitializes the audio manager. */
        void Deinitialize();
    };
}
