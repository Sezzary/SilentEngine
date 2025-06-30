#pragma once

namespace Silent::Services
{
    class TimeManager
    {
    public:
        // Constants

        static constexpr uint TPS = 60;

    private:
        static constexpr uint TICK_INTERVAL_MICROSEC = 1000000 / TPS;

        // Fields

        uint   _ticks              = 0;
        uint64 _prevUptimeMicrosec = 0;
        uint64 _baseMicrosec       = 0;

    public:
        // Constructors

        TimeManager() = default;

        // Getters

        float GetDeltaTime() const;
        uint  GetTicks() const;

        // Inquirers

        bool TestInterval(uint intervalTicks, uint offsetTicks = 0) const;

        // Utilities

        void Initialize();
        void Update();
        void WaitForNextTick() const;

    private:
        // Helpers

        uint64 GetUptimeMicrosec() const;
        uint64 GetEpochMicrosec() const;
    };

    constexpr uint SEC_TO_TICK(float sec)
    {
        return (sec > 0.0f) ? std::max<uint>((uint)ROUND(sec * (float)TimeManager::TPS), 1) : 0;
    }

    constexpr float TICK_TO_SEC(uint ticks)
    {
        return (float)ticks / (float)TimeManager::TPS;
    }

    std::string GetCurrentDateString();
    std::string GetCurrentTimeString();
}
