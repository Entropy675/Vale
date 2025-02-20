#ifndef THREADPOOL_H__
#define THREADPOOL_H__

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <atomic>
#include <stdexcept>
#include <utility>

#include "defs.h"

//----------------------------------------------------------------
// Cooperative preemption token
//----------------------------------------------------------------
struct PreemptionToken
{
    std::atomic<bool> shouldYield;
    PreemptionToken() : shouldYield(false) {}
};

// Helper for tasks to check at yield points
inline bool yieldPoint(PreemptionToken &token)
{
    return token.shouldYield.load();
}

//----------------------------------------------------------------
// ThreadPool
//
// A thread pool that schedules tasks based on priority and allows
// cooperative preemption. Higher integer values indicate higher
// priority. Tasks are expected to be defined with the signature:
//     ReturnType task(PreemptionToken&, ...)
//----------------------------------------------------------------
class ThreadPool
{
public:
    // Constructor: Creates a pool with 'threads' worker threads.
    // Defaults to the number of hardware threads.
    ThreadPool(size_t threads = std::thread::hardware_concurrency());

    // Destructor: Signals workers to stop and joins them.
    ~ThreadPool();

    // Enqueue a task with a given priority.
    // F must be callable as: ReturnType(PreemptionToken&, Args...).
    template<class F, class... Args>
    auto enqueue(int priority, F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(PreemptionToken&, Args...)>::type>;

private:
    // Internal task structure.
    struct Task
    {
        int priority;
        std::function<void(PreemptionToken&)> func;
        Task(int p, std::function<void(PreemptionToken&)> f)
        : priority(p), func(f) {}
    };

    // Comparator: higher priority tasks come first.
    struct CompareTask
    {
        bool operator()(const Task &a, const Task &b)
        {
            return a.priority < b.priority;
        }
    };

    // Worker threads.
    std::vector<std::thread> workers;
    // Priority queue (heap) of tasks.
    std::priority_queue<Task, std::vector<Task>, CompareTask> taskQueue;

    // Synchronization primitives.
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;

    // For cooperative preemption:
    // Each worker has an associated PreemptionToken.
    std::vector<std::shared_ptr<PreemptionToken>> tokens;
    // The current task's priority running on each worker (-1 if idle).
    std::vector<int> currentTaskPriority;

    // A monitor thread that checks for preemption.
    std::thread monitorThread;

    // Function run by the monitor thread.
    void monitorPreemption();
};

// Template method: Enqueue a task with a given priority.
// F must be callable with signature: ReturnType(PreemptionToken&, Args...).
template<class F, class... Args>
auto ThreadPool::enqueue(int priority, F&& f, Args&&... args)
-> std::future<typename std::result_of<F(PreemptionToken&, Args...)>::type>
{
    using return_type = typename std::result_of<F(PreemptionToken&, Args...)>::type;

    // Create a packaged task that accepts a PreemptionToken&.
    auto task = std::make_shared<std::packaged_task<return_type(PreemptionToken&)>>(
        std::bind(std::forward<F>(f), std::placeholders::_1, std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        if (stop)
            throw std::runtime_error("Enqueue on stopped ThreadPool");
        // Wrap the task in a lambda that matches our stored function signature.
        taskQueue.emplace(priority, [task](PreemptionToken &token) { (*task)(token); });
    }
    condition.notify_one();
    return res;
}

#endif
