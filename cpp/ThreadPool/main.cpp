
#include <cstdio>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <string>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "ThreadPool.h"

class CountDownLatch
{
 public:

  explicit CountDownLatch(int count);

  void wait();

  void countDown();

  int getCount() const;

 private:
  mutable std::mutex mutex_;
  std::condition_variable condition_;
  int count_;
};

CountDownLatch::CountDownLatch(int count)
  : mutex_(),
    condition_(),
    count_(count)
{
}

void CountDownLatch::wait()
{
    std::unique_lock<std::mutex> lock(mutex_);
    condition_.wait(lock, [this]{ return this->count_ == 0; });
}

void CountDownLatch::countDown()
{
  const std::lock_guard<std::mutex> lock(mutex_);
  --count_;
  if (count_ == 0)
  {
    condition_.notify_all();
  }
}

int CountDownLatch::getCount() const
{
  const std::lock_guard<std::mutex> lock(mutex_);
  return count_;
}


void print() {
    std::ostringstream oss;
    oss << std::this_thread::get_id() << std::endl;
    printf("tid = %s", oss.str().c_str());
}

void printString(const std::string& str) {
    printf("%s\n", str.c_str());
    std::this_thread::sleep_for(std::chrono::microseconds(300));
}

void test(int maxSize) {
    
    printf("Test ThreadPool with max queue size = %d\n", maxSize);
    
    LambdaRan::ThreadPool pool("MainThreadPool");
    pool.SetMaxQueueSize(maxSize);
    pool.Start(maxSize);
    
    printf("adding\n");
    
    pool.Run(print);
    pool.Run(print);
    for (int i = 0; i < 100; ++i)
    {
        char buf[32];
        snprintf(buf, sizeof buf, "task %d", i);
        pool.Run(std::bind(printString, std::string(buf)));
    }
    printf("done\n");
    
    CountDownLatch latch(1);
    pool.Run(std::bind(&CountDownLatch::countDown, &latch));
    latch.wait();
    pool.Stop();
}

void longTask(int num) {
    printf("longTask %d\n", num);
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

void test2() {
    printf("Test ThreadPool by stoping early.\n");
    LambdaRan::ThreadPool pool("ThreadPool");
    pool.SetMaxQueueSize(5);
    pool.Start(3);
    
    std::thread thread1([&pool] {
        printf("thread1 start\n");
        for (int i = 0; i < 20; ++i) {
            pool.Run(std::bind(longTask, i));
            printf("thread1 run %d\n", i);
        }
        printf("thread1 end\n");
    });
    std::this_thread::sleep_for(std::chrono::seconds(5));
    printf("stop pool\n");
    pool.Stop();
    
    thread1.join();
    pool.Run(print);
    printf("test2 done\n");
}

int main(int argc, char **argv) {
    std::cout << "MainThread:" << std::this_thread::get_id() << std::endl;
    test(0);
    test(1);
    test(5);
    test(10);
    test(50);
    test2();
    return 0;
}