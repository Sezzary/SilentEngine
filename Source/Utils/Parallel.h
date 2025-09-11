#pragma once

namespace Silent::Utils
{
    using ParallelTask  = std::function<void()>;
    using ParallelTasks = std::vector<ParallelTask>;

    /** @brief Wraps a task in a lambda for parallel execution.
     *
     * @param task Task to execute.
     * @return Lambda with a generic context capture that runs the task.
     */
    #define TASK(task) \
        ([&]() { task; })

    /** @brief Parallel task executor. */
    class ParallelExecutor
    {
    private:
        // =======
        // Fields
        // =======

        std::vector<std::jthread> _threads      = {};    /** Worker threads. */
        std::queue<ParallelTask>  _tasks        = {};    /** Pending tasks to execute. */
        std::mutex                _taskMutex    = {};    /** Mutex for locking `_tasks` access. */
        std::condition_variable   _taskCond     = {};    /** Condition variable to notify idle threads of new tasks. */
        bool                      _deinitialize = false; /** Flag notifying worker threads of a shutdown. */

    public:
        // =============
        // Constructors
        // =============

        ParallelExecutor() = default;

        // ========
        // Getters
        // ========

        /** @brief Gets the number of worker threads available in the executor for task execution.
         *
         * @return Worker thread count.
         */
        uint GetThreadCount() const;

        /** @brief Gets the number of pending tasks to be executed.
         *
         * @return Queued task count.
         */
        uint GetPendingTaskCount() const;

        // ==========
        // Utilities
        // ==========

        /** @brief Initializes the parallel task executor. */
        void Initialize();

        /** @brief Gracefully deinitializes the parallel task executor. */
        void Deinitialize();

        /** @brief Adds a task to the queue for execution.
         *
         * @param task Task to queue.
         * @return `std::future` of the parallel task.
         */
        std::future<void> AddTask(const ParallelTask& task);

        /** @brief Adds a collection task to the queue for execution.
         *
         * @param task Tasks to queue.
         * @return `std::future` of the tasks.
         */
        std::future<void> AddTasks(const ParallelTasks& tasks);

    private:
        // ========
        // Helpers
        // ========

        /** @brief Thread worker. Automatically picks up and executes tasks in the queue. */
        void Worker();

        /** @brief Adds a task to the queue for execution, associating it with a task group via a counter and promise.
         *
         * @param task Task to execute.
         * @param counter Shared counter tracking the number of tasks in the group.
         * @param promise Shared promise for task group synchronization.
         */
        void AddTask(const ParallelTask& task, std::shared_ptr<std::atomic<int>> counter, std::shared_ptr<std::promise<void>> promise);

        /** @brief Executes a grouped task and decrements the associated counter.
         * When all tasks in the group are complete, it sets the `promise` to notify blocked threads waiting on the group.
         *
         * @param task Task to execute in the group.
         * @param counter Group counter to decrement when the task completes.
         * @param promise Promise to set when the group counter reaches 0, indicating the group tasks are complete.
         */
        void HandleTask(const ParallelTask& task, std::atomic<int>& counter, std::promise<void>& promise);
    };

    /** @brief Global parallel task executor. */
    extern ParallelExecutor g_Executor;

    /** @brief Gets the number of CPU cores on the system.
     *
     * @return CPU core count.
     */
    uint GetCoreCount();

    /** @brief Generates a ready dummy `std::future`. For use when a task future cannot be created.
     *
     * @return Ready dummy `std::future`.
     */
    std::future<void> GenerateReadyFuture();
}
