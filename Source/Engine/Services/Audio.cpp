#include "Framework.h"
#include "Engine/Services/Audio.h"

namespace Silent::Services
{
    void AudioManager::Initialize()
    {
        // Desired output format.
        static auto spec = SDL_AudioSpec
        {
            .format   = SDL_AUDIO_S16, // 16-bit signed samples.
            .channels = 2,             // Stereo.
            .freq     = 44100          // 44.1 kHz sample rate.
        };

        _stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
        if (_stream == nullptr)
        {
            throw std::runtime_error("Failed to open audio stream: " + std::string(SDL_GetError()));
        }
    }

    void AudioManager::Deinitialize()
    {
        SDL_CloseAudioDevice(SDL_GetAudioStreamDevice(_stream));
        SDL_DestroyAudioStream(_stream);
    }
}
