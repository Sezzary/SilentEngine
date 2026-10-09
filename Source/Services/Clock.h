#pragma once

namespace Silent::Services
{
    constexpr int TICKS_PER_SECOND = 30;

    /** @brief Game performance tracker. */
    struct Performance
    {
        float Fps       = 0.0f;
        int   FrameTime = 0;
    };

    /** @brief Game clock manager. */
    class ClockManager
    {
    public:
        // ==========
        // Constants
        // ==========

        static constexpr int MICROSEC_PER_SEC = 1000000;

    private:
        static constexpr int TICK_INTERVAL_DURATION = MICROSEC_PER_SEC / TICKS_PER_SECOND;
        static constexpr int TICKS_MAX              = 5;

        // =======
        // Fields
        // =======

        int    _ticks              = 0;
        uint64 _prevUptimeDuration = 0;

        Performance         _performance = {};
        std::vector<float>  _fpses       = {};
        std::vector<uint64> _durations   = {};

    public:
        // =============
        // Constructors
        // =============

        /** @brief Creates a default uninitialized instance. */
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
        int GetTicks() const;

        /** @brief Gets the game performance tracker for the current tick.
         *
         * @return Game performance tracker.
         */
        const Performance& GetPerformance() const;

        // ==========
        // Inquirers
        // ==========

        /** @brief Tests if a tick interval has passed.
         *
         * @param intervalTicks Tick duration of the desired interval.
         * @param offsetTicks Tick offset.
         * @return `true` if the interval has passed, `false` otherwise.
         */
        bool TestInterval(int intervalTicks, int offsetTicks = 0) const;

        // ==========
        // Utilities
        // ==========

        /** @brief Initializes the clock manager. */
        void Initialize();

        /** @brief Updates the clock manager, setting accumulated ticks. */
        void Update();

        /** @brief Updates the game performance tracker. Called before `WaitForNextTick`. */
        void UpdatePerformance();

        /** @brief Blocks the current thread until the next tick.
         * Used to avoid busy-waiting in the main loop if a logic iteration has finished early.
         * Called after `UpdatePerformance`.
         */
        void WaitForNextTick() const;

    private:
        // ========
        // Helpers
        // ========

        /** @brief Gets the current system uptime in microseconds.
         * This utilizes the high-resolution performance counter to provide a precise microsecond timestamp since the
         * start of the application.
         *
         * @return Current uptime in microseconds.
         */
        uint64 GetUptimeMicroseconds() const;
    };

    /** @brief Converts seconds to ticks.
     *
     * @param sec Seconds to convert.
     * @return Seconds converted to ticks.
     */
    constexpr int SEC_TO_TICK(float sec)
    {
        sec = std::max(sec, 0.0f);
        return (sec == 0.0f) ? 0 : std::max((int)ROUND(sec * (float)TICKS_PER_SECOND), 1);
    }

    /** @brief Converts ticks to seconds.
     *
     * @param ticks Ticks to convert.
     * @return Ticks converted to seconds.
     */
    constexpr float TICK_TO_SEC(int ticks)
    {
        ticks = std::max(ticks, 0);
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
