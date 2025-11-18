#include "Framework.h"
#include "Services/Clock.h"

#include "Utils/Utils.h"

using namespace Silent::Utils;

namespace Silent::Services
{
    float ClockManager::GetDeltaTime() const
    {
        uint64 uptimeDur  = GetUptimeDuration();
        uint64 elapsedDur = uptimeDur - _prevUptimeDuration;
        return std::min((float)elapsedDur / 1000000.0f, ((float)TICKS_PER_SECOND / 6.0f) / (float)TICKS_PER_SECOND);
    }

    uint ClockManager::GetTicks() const
    {
        return std::min(_ticks, TICKS_PER_SECOND / 6);
    }

    bool ClockManager::TestInterval(uint intervalTicks, uint offsetTicks) const
    {
        if (offsetTicks >= intervalTicks)
        {
            offsetTicks = 0;
            Debug::Log("Attempted to test time interval with offset greater than or equal to interval.", Debug::LogLevel::Warning, Debug::LogMode::Debug);
        }
        
        uint64 ticks = GetUptimeDuration() / TICK_INTERVAL_DURATION;
        return (ticks % intervalTicks) == offsetTicks;
    }

    void ClockManager::Initialize()
    {
        _ticks              = 0;
        _prevUptimeDuration = 0;
        _startDuration       = GetEpochDuration();
    }

    void ClockManager::Update()
    {
        uint64 uptimeDur  = GetUptimeDuration();
        uint64 elapsedDur = uptimeDur - _prevUptimeDuration;

        // Calculate ticks for elapsed period.
        _ticks = (uint)(elapsedDur / TICK_INTERVAL_DURATION);

        // Set previous uptime if new ticks accumulated.
        if (_ticks != 0)
        {
            _prevUptimeDuration = uptimeDur;
        }
    }

    void ClockManager::WaitForNextTick() const
    {
        uint64 uptimeDur  = GetUptimeDuration();
        uint64 elapsedDur = uptimeDur - _prevUptimeDuration;

        // Sleep current thread for remaining time before next tick.
        uint64 remainingDur = TICK_INTERVAL_DURATION - (elapsedDur % TICK_INTERVAL_DURATION);
        if (remainingDur > 0 && remainingDur < TICK_INTERVAL_DURATION)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(remainingDur));
        }
    }

    uint64 ClockManager::GetUptimeDuration() const
    {
        return GetEpochDuration() - _startDuration;
    }

    uint64 ClockManager::GetEpochDuration() const
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

    std::string GetCurrentDateString()
    {
        auto now      = std::chrono::system_clock::now();
        auto nowClock = std::chrono::system_clock::to_time_t(now);
        auto tm       = *std::localtime(&nowClock);

        // Format: YYYY-MM-DD.
        auto strStream = std::ostringstream();
        strStream << std::put_time(&tm, "%Y-%m-%d");
        return strStream.str();
    }

    std::string GetCurrentTimeString()
    {
        auto now      = std::chrono::system_clock::now();
        auto nowClock = std::chrono::system_clock::to_time_t(now);
        auto tm       = *std::localtime(&nowClock);
        auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        // Format: HH-MM-SS_mmm.
        auto strStream = std::ostringstream();
        strStream << std::put_time(&tm, "%H-%M-%S") << "_" << std::setw(3) << std::setfill('0') << millisec.count();
        return strStream.str();
    }
}
