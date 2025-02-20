#include "ThreadPool.h"
#include <chrono>

ThreadPool::ThreadPool(size_t threads)
: stop(false)
{
    // Reserve space for tokens and current priorities.
    tokens.resize(threads);
    currentTaskPriority.resize(threads, -1);

    // Launch worker threads.
    for (size_t i = 0; i < threads; ++i)
    {
        tokens[i] = std::make_shared<PreemptionToken>();
        workers.emplace_back([this, i]()
        {
            while (true)
            {
                Task task(0, nullptr);
                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    this->condition.wait(lock, [this] {
                        return stop || !taskQueue.empty();
                    });
                    if (stop && taskQueue.empty())
                        return;
                    task = std::move(taskQueue.top());
                    taskQueue.pop();
                }
                // Set the current task's priority for this worker.
                currentTaskPriority[i] = task.priority;
                // Reset the preemption flag.
                tokens[i]->shouldYield.store(false);
                // Execute the task, passing in this worker's token.
                task.func(*tokens[i]);
                // Mark worker as idle.
                currentTaskPriority[i] = -1;
            }
        });
    }

    // Start the monitor thread to check for preemption opportunities.
    monitorThread = std::thread(&ThreadPool::monitorPreemption, this);
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers)
        worker.join();
    if (monitorThread.joinable())
        monitorThread.join();
}

void ThreadPool::monitorPreemption()
{
    while (!stop)
    {
        int highestQueuedPriority = -1;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (!taskQueue.empty())
                highestQueuedPriority = taskQueue.top().priority;
        }
        // Signal workers to yield if a higher priority task is waiting.
        for (size_t i = 0; i < tokens.size(); ++i)
        {
            if (currentTaskPriority[i] != -1 && currentTaskPriority[i] < highestQueuedPriority)
                tokens[i]->shouldYield.store(true);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
