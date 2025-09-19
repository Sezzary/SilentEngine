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

        SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(_stream));
    }

    void AudioManager::Deinitialize()
    {
        SDL_CloseAudioDevice(SDL_GetAudioStreamDevice(_stream));
        SDL_DestroyAudioStream(_stream);
    }

    void AudioManager::Update()
    {
        static float time = 0.0f;

        // Generate a sine wave continuously.
        const int sampleCount = 4096; // Number of samples to push per frame.
        auto buf = std::vector<int16>(sampleCount * 2); // Stereo.

        float freq = 400.0f;
        for (int i = 0; i < sampleCount; i++)
        {
            float alpha = time + (float)i / 44100.0f;
            int16 sample = (int16)(std::sin((PI_MUL_2 * freq) * alpha) * 3000.0f);
            buf[(i * 2) + 0] = sample; // Left.
            buf[(i * 2) + 1] = sample; // Right.
        }
        time += (float)sampleCount / 44100.0f;

        SDL_PutAudioStreamData(_stream, buf.data(), buf.size() * sizeof(int16));
    }
}
