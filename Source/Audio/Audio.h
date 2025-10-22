#pragma once

namespace Silent::Audio
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

        /** @brief Initializes the audio stream. */
        void Initialize();

        /** @brief Gracefully deinitializes the audio manager. */
        void Deinitialize();

        /** @brief Updates the audio system. */
        void Update();
    };
}
