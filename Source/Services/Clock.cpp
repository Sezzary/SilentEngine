#include "Framework.h"
#include "Services/Clock.h"

#include "Utils/Utils.h"

using namespace Silent::Utils;

namespace Silent::Services
{
    float ClockManager::GetDeltaTime() const
    {
        uint64 uptimeDur  = GetUptimeMicroseconds();
        uint64 elapsedDur = uptimeDur - _prevUptimeDuration;
        return std::min((float)elapsedDur / (float)MICROSEC_PER_SEC,
                        ((float)TICKS_PER_SECOND / (float)TICKS_MAX) / (float)TICKS_PER_SECOND);
    }

    int ClockManager::GetTicks() const
    {
        return std::min(_ticks, TICKS_MAX);
    }

    const Performance& ClockManager::GetPerformance() const
    {
        return _performance;
    }

    bool ClockManager::TestInterval(int intervalTicks, int offsetTicks) const
    {
        intervalTicks = std::max(intervalTicks, 0);
        offsetTicks   = std::max(offsetTicks, 0);

        if (offsetTicks >= intervalTicks)
        {
            offsetTicks = 0;
            Debug::Log("Attempted to test time interval with offset greater than or equal to interval.",
                       Debug::LogLevel::Warning, Debug::LogMode::Debug);
        }
        
        uint64 ticks = SDL_GetPerformanceCounter() / TICK_INTERVAL_DURATION;
        return (ticks % intervalTicks) == offsetTicks;
    }

    void ClockManager::Initialize()
    {
        _ticks              = 0;
        _prevUptimeDuration = GetUptimeMicroseconds();

        _fpses.reserve(TICKS_PER_SECOND);
        _durations.reserve(TICKS_PER_SECOND);
    }

    void ClockManager::Update()
    {
        uint64 uptimeDur  = GetUptimeMicroseconds();
        uint64 elapsedDur = uptimeDur - _prevUptimeDuration;

        // Calculate ticks for elapsed period.
        _ticks = (int)(elapsedDur / TICK_INTERVAL_DURATION);

        // Set previous uptime if new ticks accumulated.
        if (_ticks != 0)
        {
            _prevUptimeDuration += _ticks * TICK_INTERVAL_DURATION;
        }
    }

    void ClockManager::UpdatePerformance()
    {
        static int ticks = 0;
        ticks++;

        // Compute tick duration.
        uint64 dur = GetUptimeMicroseconds() - _prevUptimeDuration;
        _durations.push_back(dur);

        // Compute instantaneous FPS for current tick.
        if (dur > 0)
        {
            float fps = (float)TICKS_PER_SECOND / ((float)dur / (float)MICROSEC_PER_SEC);
            if (fps > TICKS_PER_SECOND)
            {
                fps = TICKS_PER_SECOND;
            }

            _fpses.push_back(fps);
        }
        else
        {
            _fpses.push_back(TICKS_PER_SECOND);
        }

        // Update performance stats.
        if (ticks >= TICKS_PER_SECOND)
        {
            // Update average FPS.
            _performance.Fps = 0.0f;
            for (float fps : _fpses)
            {
                _performance.Fps += fps;
            }
            _performance.Fps /= (float)TICKS_PER_SECOND;

            // Update average frame time.
            _performance.FrameTime = 0;
            for (uint64 dur : _durations)
            {
                _performance.FrameTime += dur;
            }
            _performance.FrameTime /= TICKS_PER_SECOND;

            ticks = 0;
            _fpses.clear();
            _durations.clear();
        }
    }

    void ClockManager::WaitForNextTick() const
    {
        uint64 uptimeDur  = GetUptimeMicroseconds();
        uint64 elapsedDur = uptimeDur - _prevUptimeDuration;

        // Sleep current thread for remaining time before next tick.
        if (elapsedDur < TICK_INTERVAL_DURATION)
        {
            uint64 remainingDur = TICK_INTERVAL_DURATION - elapsedDur;
            std::this_thread::sleep_for(std::chrono::microseconds(remainingDur));
        }
    }

    uint64 ClockManager::GetUptimeMicroseconds() const
    {
        return (SDL_GetPerformanceCounter() * MICROSEC_PER_SEC) / SDL_GetPerformanceFrequency();
    }

    std::string GetCurrentDateString()
    {
        auto  now      = std::chrono::system_clock::now();
        auto  nowClock = std::chrono::system_clock::to_time_t(now);
        auto& tm       = *std::localtime(&nowClock);

        // Format: YYYY-MM-DD.
        auto strStream = std::ostringstream();
        strStream << std::put_time(&tm, "%Y-%m-%d");
        return strStream.str();
    }

    std::string GetCurrentTimeString()
    {
        auto  now      = std::chrono::system_clock::now();
        auto  nowClock = std::chrono::system_clock::to_time_t(now);
        auto& tm       = *std::localtime(&nowClock);
        auto  millisec = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        // Format: HH-MM-SS_mmm.
        auto strStream = std::ostringstream();
        strStream << std::put_time(&tm, "%H-%M-%S") << "_" << std::setw(3) << std::setfill('0') << millisec.count();
        return strStream.str();
    }
}
