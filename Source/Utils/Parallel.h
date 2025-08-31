#pragma once

namespace Silent::Utils
{
    using ParallelTask  = std::function<void()>;
    using ParallelTasks = std::vector<ParallelTask>;

    class ParallelTaskManager
    {
    private:
        // Fields

        std::vector<std::jthread> _threads      = {};
        std::queue<ParallelTask>  _tasks        = {};
        std::mutex                _taskMutex    = {};
        std::condition_variable   _taskCond     = {};
        bool                      _deinitialize = false;

    public:
        // Constructors, destructors

        ParallelTaskManager() = default;

        // Getters

        uint GetThreadCount() const;
        uint GetPendingTaskCount() const;

        // Utilities

        void              Initialize();
        void              Deinitialize();
        std::future<void> AddTask(const ParallelTask& task);
        std::future<void> AddTasks(const ParallelTasks& tasks);

    private:
        // Helpers

        void Worker();
        void AddTask(const ParallelTask& task, std::shared_ptr<std::atomic<int>> counter, std::shared_ptr<std::promise<void>> promise);
        void HandleTask(const ParallelTask& task, std::atomic<int>& counter, std::promise<void>& promise);
    };

    extern ParallelTaskManager g_Parallel;

    uint              GetCoreCount();
    std::future<void> GenerateReadyFuture();
}
