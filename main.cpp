#include <iostream>
#include "../a-simple-ThreadPool/threadPool.h"

class Task {
public:
    void operator() () {
        std::cout << "process..." << std::endl;
    }
};

int main() {
    ThreadPool<Task> pool(8);
    for (int i = 0; i < 20; ++i) {
        Task t;
        pool.addTask(std::move(t));
    }

    return 0;
}