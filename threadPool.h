#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <memory>
#include <assert.h>

template <typename T>
class ThreadPool {
public:
    ThreadPool() = default;

    explicit ThreadPool(size_t threadNum = 8);

    ~ThreadPool();

    void addTask(T&& tasks);

private:
    struct Pool {
        std::mutex mtx;
        std::condition_variable cond;
        std::queue<T> tasks;
        bool isClosed;
    };

    std::shared_ptr<Pool> pool_;//通过智能指针统一管理资源
};

template <typename T>
ThreadPool<T>::ThreadPool(size_t threadNum) : pool_(std::make_shared<Pool>()) {//类外定义不需要注明explicit
    assert(threadNum > 0);

    for (size_t i = 0; i < threadNum; ++i) {
        std::thread([pool = pool_, i] {
            std::unique_lock<std::mutex> locker(pool->mtx);//直接上锁
            std::cout << "thread id : " << i << std::endl;
            while (true) {
                if (pool->tasks.empty()) {
                    auto task = std::move(pool->tasks.front());
                    pool->tasks.pop();
                    locker.unlock();//关锁
                    task();
                    locker.lock();//unique_lock在离开作用域时可以由其其构函数自动解锁
                } else if (pool->isClosed) {
                    break;
                } else {
                    pool->cond.wait(locker);
                }
            }
        }).detach();
    }
}

template <typename T>
ThreadPool<T>::~ThreadPool() {
    {
        std::lock_guard<std::mutex> locker(pool_->mtx);//lock_guard<>对象创建时自动上锁，离开作用域时自动解锁，不能手动
        pool_->isClosed = true;
    }
    pool_->cond.notify_all();
}

template <typename T>
void ThreadPool<T>::addTask(T&& task) {
    pool_->mtx.lock();
    pool_->tasks.emplace(std::forward<T>(task));//std::forward<>() ? 
    pool_->mtx.unlock();

    pool_->cond.notify_one();
}

#endif