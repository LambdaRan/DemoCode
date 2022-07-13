
#include "CountDownLatch.h"

using namespace LambdaRan;

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



