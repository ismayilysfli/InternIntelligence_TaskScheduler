#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>
#include <atomic>

enum class TaskPriority {
    High,
    Medium,
    Low
};

struct Task {
    int id;
    TaskPriority priority;
    std::function<void()> function;

    bool operator<(const Task& other) const {
        return static_cast<int>(priority) < static_cast<int>(other.priority);
    }
};

class TaskScheduler {
private:
    std::priority_queue<Task> taskQueue;
    std::vector<std::thread> workerThreads;
    std::mutex queueMutex;
    std::condition_variable cv;
    std::atomic<bool> stopScheduler;

    void worker();

public:
    TaskScheduler(size_t numThreads);
    ~TaskScheduler();
    void addTask(Task task);
};

#endif // TASKSCHEDULER_H