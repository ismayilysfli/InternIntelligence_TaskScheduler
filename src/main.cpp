#include "taskScheduler.h"
#include <iostream>
#include <chrono>

int main() {
    TaskScheduler scheduler(4); // Create a scheduler with 4 worker threads

    auto taskFunction = [](int id, TaskPriority priority) {
        std::cout << "Task " << id << " with priority " << static_cast<int>(priority)
                  << " is running on thread " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
    };

    // Add tasks with different priorities
    scheduler.addTask({1, TaskPriority::High, std::bind(taskFunction, 1, TaskPriority::High)});
    scheduler.addTask({2, TaskPriority::Medium, std::bind(taskFunction, 2, TaskPriority::Medium)});
    scheduler.addTask({3, TaskPriority::Low, std::bind(taskFunction, 3, TaskPriority::Low)});
    scheduler.addTask({4, TaskPriority::High, std::bind(taskFunction, 4, TaskPriority::High)});
    scheduler.addTask({5, TaskPriority::Medium, std::bind(taskFunction, 5, TaskPriority::Medium)});
    scheduler.addTask({6, TaskPriority::Low, std::bind(taskFunction, 6, TaskPriority::Low)});

    // Allow time for tasks to complete
    std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}
