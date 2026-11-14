#include "Framework.h"
#include "Utils/Video.h"

#include "Application.h"
#include "Renderer/Common/Constants.h"
#include "Services/Filesystem.h"
#include "Utils/DoubleBuffer.h"
#include "Utils/Parallel.h"

using namespace Silent::Renderer;
using namespace Silent::Services;

namespace Silent::Utils
{
    /** @brief Callback to convert an MPEG1 YCrCb frame to RGBA.
     *
     * @param plm PLM context.
     * @param samples Interleaved samples.
     * @param user User data for the RGBA video frame double buffer.
     */
    static void OnVideoFrame(plm_t* plm, plm_frame_t* frame, void* user)
    {
        auto* frameBuffer = (DoubleBuffer<std::vector<byte>>*)user;
        plm_frame_to_rgba(frame, (uint8*)frameBuffer->Back.data(), frame->width * RGBA_COMP_COUNT);
    }

    /** @brief Callback to decode audio samples from an MPEG1 stream.
     *
     * @param plm PLM context.
     * @param samples Interleaved samples.
     * @param user User data for the video player.
     */
    static void OnAudioFrame(plm_t* plm, plm_samples_t* samples, void* user)
    {
        auto* player = (VideoPlayer*)user;
        player->AppendAudio(samples->interleaved, samples->count * 2);
    }

    VideoPlayer::~VideoPlayer()
    {
        Stop();
    }

    Vector2i VideoPlayer::GetResolution() const
    {
        if (!IsLoaded())
        {
            Debug::Log("Attempted to get video resolution with no video playing.", Debug::LogLevel::Warning);
            return Vector2i::Zero;
        }

        return Vector2i(plm_get_width(_plm), plm_get_height(_plm));
    }

    float VideoPlayer::GetAspectRatio() const
    {
        if (!IsLoaded())
        {
            Debug::Log("Attempted to get video aspect ratio with no video playing.", Debug::LogLevel::Warning);
            return 1.0f;
        }

        auto res = GetResolution().ToVector2();
        return res.x / res.y;
    }

    float VideoPlayer::GetTime() const
    {
        if (!IsLoaded())
        {
            Debug::Log("Attempted to get video time with no video playing.", Debug::LogLevel::Warning);
            return 0.0f;
        }

        return plm_get_time(_plm);
    }

    float VideoPlayer::GetDuration() const
    {
        if (!IsLoaded())
        {
            Debug::Log("Attempted to get video duration with no video playing.", Debug::LogLevel::Warning);
            return 0.0f;
        }

        return plm_get_duration(_plm);
    }

    const std::string& VideoPlayer::GetName() const
    {
        return _activeVideoName;
    }

    const std::vector<byte>& VideoPlayer::GetVideoFrame() const
    {
        if (!IsLoaded())
        {
            Debug::Log("Attempted to get video frame with no video playing.", Debug::LogLevel::Warning);
        }

        return _frameBuffer.Front;
    }

    std::vector<float> VideoPlayer::GetAudioFrame()
    {
        if (!IsLoaded())
        {
            Debug::Log("Attempted to get audio frame with no video playing.", Debug::LogLevel::Warning);
            return {};
        }

        // @lock Restrict audio buffer access.
        {
            auto lock = ParallelLock(_audioMutex);

            auto data = _audioBuffer;
            _audioBuffer.clear();
            return data;
        }
    }

    void VideoPlayer::SetOnStopCallback(OnStopCallback onStop)
    {
        _onStop = std::move(onStop);
    }

    bool VideoPlayer::IsLoaded() const
    {
        return _plm != nullptr;
    }

    bool VideoPlayer::IsPlaying() const
    {
        if (!IsLoaded())
        {
            return false;
        }

        return plm_has_ended(_plm) == 0;
    }

    void VideoPlayer::Initialize(const stdfs::path& videosPath)
    {
        _videosPath = videosPath;
    }

    void VideoPlayer::Play(const std::string& filename)
    {
        const auto& fs = g_App.GetFilesystem();

        // Check if video player is initialized.
        if (_videosPath.empty())
        {
            Debug::Log(Fmt("Attempted to play video `{}` in uninitialized video player.", filename),
                       Debug::LogLevel::Warning);
            return;
        }

        // Check if same video is already playing.
        if (filename == _activeVideoName)
        {
            Debug::Log(Fmt("Video `{}` is already playing.", filename), Debug::LogLevel::Warning);
            return;
        }

        // Interrupt previous active video.
        if (IsLoaded())
        {
            Stop();
        }

        // Open video file.
        auto videoPath = fs.GetAssetsDirectory() / ASSETS_VIDEO_DIR_NAME / filename;
        _plm           = plm_create_with_filename(videoPath.string().c_str());
        if (!IsLoaded())
        {
            Debug::Log(Fmt("Failed to play video `{}`.", filename), Debug::LogLevel::Warning);
            return;
        }

        // Set active video parameters.
        _activeVideoName = filename;
        
        // Register callbacks.
        plm_set_audio_enabled(_plm, true);
        plm_set_video_decode_callback(_plm, OnVideoFrame, &_frameBuffer);
        plm_set_audio_decode_callback(_plm, OnAudioFrame, this);

        // Resize buffer.
        auto res = GetResolution();
        _frameBuffer.Back.resize((res.x * res.y) * RGBA_COMP_COUNT);

        // Decode frame.
        plm_decode(_plm, 0.0f);
        Debug::Log(Fmt("Playing video `{}`.", filename));
    }

    void VideoPlayer::Stop()
    {
        if (!IsLoaded())
        {
            Debug::Log("Attempted to stop video with none playing.", Debug::LogLevel::Warning);
            return;
        }

        // Execute `_onStop` callback.
        if (_onStop)
        {
            _onStop(_activeVideoName);
        }

        plm_destroy(_plm);
        _plm = nullptr;

        Debug::Log(Fmt("Stopped video `{}`.", _activeVideoName));

        _frameBuffer.Back.clear();
        _audioBuffer.clear();
        _activeVideoName = {};
    }

    void VideoPlayer::Update(float deltaTime)
    {
        if (!IsLoaded())
        {
            Debug::Log("Attempted to update video with none playing.", Debug::LogLevel::Warning);
            return;
        }

        // Resize buffer if required.
        auto res          = GetResolution();
        int  requiredSize = (res.x * res.y) * RGBA_COMP_COUNT;
        if (_frameBuffer.Back.size() != requiredSize)
        {
            _frameBuffer.Back.resize(requiredSize);
        }

        // Decode frame.
        plm_decode(_plm, deltaTime);
    }

    void VideoPlayer::SwapFrameBuffer()
    {
        _frameBuffer.Swap();
    }

    void VideoPlayer::AppendAudio(const float* samples, int count)
    {
        // @lock Restrict audio buffer access.
        {
            auto lock = ParallelLock(_audioMutex);

            _audioBuffer.insert(_audioBuffer.end(), samples, samples + count);
        }
    }
}
