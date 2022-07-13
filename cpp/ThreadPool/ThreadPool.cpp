#include <cassert>
#include <cstdio>
#include <exception>

#include "ThreadPool.h"

using namespace LambdaRan;

ThreadPool::ThreadPool(const std::string& name) 
        : mutex_(),
          notEmpty_(),
          notFull_(),
          name_(name),
          maxQueueSize_(0),
          running_(false)
{
}

ThreadPool::~ThreadPool() {
    if (running_) {
        Stop();
    }
}

void ThreadPool::Start(int numThreads) {
    assert(threads_.empty());
    running_ = true;
    threads_.reserve(numThreads);
    for (int i = 0; i < numThreads; ++i) {
        char id[32];
        snprintf(id, sizeof(id), "%d", i + 1);
        threads_.emplace_back(std::bind(&ThreadPool::RunInThread, this), name_ + id);
    }
    if (numThreads == 0 && threadInitCallback_) {
        threadInitCallback_();
    }
}

void ThreadPool::Stop() {
    {
        const std::lock_guard<std::mutex> lock(mutex_);
        running_ = false;
        notEmpty_.notify_all();
        notFull_.notify_all();
    }
    for (auto& thr : threads_) {
        if (thr.joinable()) {
            thr.join();            
        }
    }
}

size_t ThreadPool::QueueSize() const {
    const std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
}

void ThreadPool::Run(Task task) {
    if (threads_.empty()) {
        task();
    }
    else {
       std::unique_lock<std::mutex> lock(mutex_);
       notFull_.wait(lock, [this] {return !(this->running_ && this->IsFull());});
        if (!running_) return;
        assert(!IsFull());
        
        queue_.push_back(std::move(task));
        notEmpty_.notify_one();
    }
}

ThreadPool::Task ThreadPool::Take() {
    std::unique_lock<std::mutex> lock(mutex_);
    notEmpty_.wait(lock, [this] {return !(this->running_ && this->queue_.empty());});
    Task task;
    if (!queue_.empty()) {
        task = queue_.front();
        queue_.pop_front();
        if (maxQueueSize_ > 0) {
            notFull_.notify_one();
        }
    }
    return task;
}

bool ThreadPool::IsFull() const {
    // 都是在锁区域调用此函数
    return maxQueueSize_ > 0 && queue_.size() >= maxQueueSize_;
}

void ThreadPool::RunInThread() {
    try {
        if (threadInitCallback_) {
            threadInitCallback_();
        }
        while (running_) {
            Task task(Take());
            if (task) {
                task();
            }
        }
    }
    catch (const std::exception& ex) {
        fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        abort();
    }
    catch (...) {
        fprintf(stderr, "unknown exception caught in ThreadPool %s\n", name_.c_str());
        throw; // rethrow
    }
}
