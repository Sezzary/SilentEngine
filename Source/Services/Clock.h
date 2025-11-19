#pragma once

namespace Silent::Services
{
    constexpr uint TICKS_PER_SECOND = 60;

    /** @brief Game clock manager. */
    class ClockManager
    {
    private:
        static constexpr uint TICK_INTERVAL_DURATION = 1000000 / TICKS_PER_SECOND;

        // =======
        // Fields
        // =======

        uint   _ticks              = 0;
        uint64 _prevUptimeDuration = 0;
        uint64 _startDuration      = 0;

    public:
        // =============
        // Constructors
        // =============

        /** @brief Constructs an uninitialized default `ClockManager`. */
        ClockManager() = default;

        // ========
        // Getters
        // ========

        /** @brief Gets the delta time in seconds since the previous tick.
         *
         * @return Delta time.
         */
        float GetDeltaTime() const;

        /** @brief Gets the accumulated ticks since the previous update.
         * If the main loop takes too long, it returns more than one tick to compensate.
         *
         * @return Accumulated ticks.
         */
        uint GetTicks() const;

        // ==========
        // Inquirers
        // ==========

        /** @brief Tests if a tick interval has passed.
         *
         * @param intervalTicks Tick duration of the desired interval.
         * @param offsetTicks Tick offset.
         * @return `true` if the interval has passed, `false` otherwise.
         */
        bool TestInterval(uint intervalTicks, uint offsetTicks = 0) const;

        // ==========
        // Utilities
        // ==========

        /** @brief Initializes the clock manager. */
        void Initialize();

        /** @brief Updates the clock manager, setting accumulated ticks. */
        void Update();

        /** @brief Blocks the current thread until the next tick.
         * Used to avoid busy-waiting in the main loop if a logic iteration has finished early.
         */
        void WaitForNextTick() const;

    private:
        // ========
        // Helpers
        // ========

        /** @brief Gets the clock uptime duration in microseconds.
         *
         * @return Uptime in microseconds.
         */
        uint64 GetUptimeDuration() const;

        /** @brief Gets the elapsed duration in microseconds since the epoch (January 1st, 1970).
         *
         * @return Elapsed microseconds since the epoch.
         */
        uint64 GetEpochDuration() const;
    };

    /** @brief Converts seconds to ticks.
     *
     * @param sec Seconds to convert.
     * @return Seconds converted to ticks.
     */
    constexpr uint SEC_TO_TICK(float sec)
    {
        return (sec > 0.0f) ? std::max<uint>((uint)ROUND(sec * (float)TICKS_PER_SECOND), 1) : 0;
    }

    /** @brief Converts ticks to seconds.
     *
     * @param ticks Ticks to convert.
     * @return Ticks converted to seconds.
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
