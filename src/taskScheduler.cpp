#include "taskScheduler.h"
#include <iostream>

void TaskScheduler::worker() {
    while (true) {
        Task task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [this]() { return !taskQueue.empty() || stopScheduler; });

            if (stopScheduler.load() && taskQueue.empty()) {
                break;
            }

            task = taskQueue.top();
            taskQueue.pop();
        }

        task.function(); // Execute the task
    }
}

TaskScheduler::TaskScheduler(size_t numThreads) : stopScheduler(false) {
    // Use only one worker thread to ensure strict priority order
    workerThreads.emplace_back(&TaskScheduler::worker, this);
}

TaskScheduler::~TaskScheduler() {
    stopScheduler = true;
    cv.notify_all();
    for (auto& thread : workerThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void TaskScheduler::addTask(Task task) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        taskQueue.push(task);
    }
    cv.notify_one(); // Notify the worker thread
}