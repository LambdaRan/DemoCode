
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <cstddef>
#include <deque>
#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <Thread>
#include <mutex>
#include <condition_variable>

namespace LambdaRan {

class ThreadPool {
  public:    
    typedef std::function<void ()> Task;
    explicit ThreadPool(const std::string& name = "ThreadPool");
    ~ThreadPool();
    
    void SetMaxQueueSize(size_t maxSize) { 
        maxQueueSize_ = maxSize; 
    }
    void SetThreadInitCallback(const Task& cb) {
        threadInitCallback_ = cb;
    }
    
    void Start(int numThreads);
    void Stop();
    
    const std::string& Name() const { return name_; }
    
    size_t QueueSize() const;
    
    void Run(Task f);
    
  private:
    bool IsFull() const;
    void RunInThread();
    Task Take();
    
    mutable std::mutex mutex_;
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    std::string name_;
    Task threadInitCallback_;
    std::vector<std::thread> threads_;
    std::deque<Task> queue_;
    size_t maxQueueSize_;
    bool running_;
};

}  // namespace LambdaRan
#endif 