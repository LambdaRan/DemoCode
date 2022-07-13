
#include <cstdio>

#include <iostream>
#include <thread>
#include <sstream>
#include <vector>
#include <functional>

#include "CountDownLatch.h"

using namespace LambdaRan;

void TestCountDownLatch(CountDownLatch& latch) {
    std::ostringstream oss;
    oss << std::this_thread::get_id();
    printf("tid = %s, latch count:%d\n", oss.str().c_str(), latch.getCount());
    latch.countDown();
}

int main(int argc, char **argv) {
    std::cout << "Test CountDownLatch MainThread:" << std::this_thread::get_id() << std::endl;
    
    std::vector<std::thread> threads;
    threads.reserve(10);
    CountDownLatch latch(10);
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(std::thread(TestCountDownLatch, std::ref(latch)));
    }
    printf("threads size: %zd\n", threads.size());
    printf("latch wait...\n");
    latch.wait();
    printf("latch wait end\n");
    
    for (auto & thr : threads) {
        if (thr.joinable())        
            thr.join();
    }
    std::cout << "End Test" << std::endl;
    return 0;
}