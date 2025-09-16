#pragma once

namespace Silent::Services
{
    constexpr uint TICKS_PER_SECOND = 60;

    /** @brief Central time manager. */
    class TimeManager
    {
    private:
        static constexpr uint TICK_INTERVAL_MICROSEC = 1000000 / TICKS_PER_SECOND;

        // Fields

        uint   _ticks              = 0;
        uint64 _prevUptimeMicrosec = 0; /** Application uptime microseconds on the previous tick. */
        uint64 _baseMicrosec       = 0;

    public:
        // Constructors

        TimeManager() = default;

        // Getters

        float GetDeltaTime() const;
        uint  GetTicks() const;

        // Inquirers

        /** @brief Tests if an interval of time, defined in ticks, has passed.
         *
         * @param intervalTicks Tick duration of the desired interval.
         * @param offsetTicks Tick offset.
         * @return `true` if the interval has passed`, `false` otherwise.
         */
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

    /** @brief Converts seconds to ticks.
     *
     * @param sec Seconds to convert.
     * @return Input seconds converted to ticks.
     */
    constexpr uint SEC_TO_TICK(float sec)
    {
        return (sec > 0.0f) ? std::max<uint>((uint)ROUND(sec * (float)TICKS_PER_SECOND), 1) : 0;
    }

    /** @brief Converts ticks to seconds.
     *
     * @param ticks Ticks to convert.
     * @return Input ticks converted to seconds.
     */
    constexpr float TICK_TO_SEC(uint ticks)
    {
        return (float)ticks / (float)TICKS_PER_SECOND;
    }

    /** @brief Gets a string containing the current system date as `YYYY-MM-DD`.
     *
     * @return String containing the current system date.
     */
    std::string GetCurrentDateString();

    /** @brief Gets a string containing the current system time as `HH-MM-SS_mmm`.
     *
     * @return String containing the current system time.
     */
    std::string GetCurrentTimeString();
}
