#include "Framework.h"
#include "Engine/Services/Time.h"

#include "Utils/Utils.h"

using namespace Silent::Utils;

namespace Silent::Services
{
    float TimeManager::GetDeltaTime() const
    {
        uint64 uptimeMicrosec  = GetUptimeMicrosec();
        uint64 elapsedMicrosec = uptimeMicrosec - _prevUptimeMicrosec;
        return (float)elapsedMicrosec / 1000000.0f;
    }

    uint TimeManager::GetTicks() const
    {
        return std::min(_ticks, TICKS_PER_SECOND / 2);
    }

    bool TimeManager::TestInterval(uint intervalTicks, uint offsetTicks) const
    {
        if (offsetTicks >= intervalTicks)
        {
            offsetTicks = 0;
            Log("Attempted to test time interval with offset greater than or equal to interval.", LogLevel::Warning, LogMode::Debug);
        }
        
        uint64 ticks = GetUptimeMicrosec() / TICK_INTERVAL_MICROSEC;
        return (ticks % intervalTicks) == offsetTicks;
    }

    void TimeManager::Initialize()
    {
        _baseMicrosec = GetEpochMicrosec();
    }

    void TimeManager::Update()
    {
        uint64 uptimeMicrosec  = GetUptimeMicrosec();
        uint64 elapsedMicrosec = uptimeMicrosec - _prevUptimeMicrosec;

        // Calculate ticks for elapsed period.
        _ticks = (uint)(elapsedMicrosec / TICK_INTERVAL_MICROSEC);

        // Set previous uptime if new ticks accumulated.
        if (_ticks != 0)
        {
            uint consumedTime   = _ticks * TICK_INTERVAL_MICROSEC;
            _prevUptimeMicrosec = uptimeMicrosec;
        }
    }

    void TimeManager::WaitForNextTick() const
    {
        uint64 uptimeMicrosec  = GetUptimeMicrosec();
        uint64 elapsedMicrosec = uptimeMicrosec - _prevUptimeMicrosec;

        // Sleep current thread for remaining time before next tick.
        uint64 remainingMicrosec = TICK_INTERVAL_MICROSEC - (elapsedMicrosec % TICK_INTERVAL_MICROSEC);
        if (remainingMicrosec > 0 && remainingMicrosec < TICK_INTERVAL_MICROSEC)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(remainingMicrosec));
        }
    }

    uint64 TimeManager::GetUptimeMicrosec() const
    {
        return GetEpochMicrosec() - _baseMicrosec;
    }

    uint64 TimeManager::GetEpochMicrosec() const
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
