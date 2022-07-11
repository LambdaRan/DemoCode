
#include <iostream>
#include <thread>

#include <chrono>
#include <ratio>
#include <sstream>
#include <string>

class ElapsedTime {
  public:    
    ElapsedTime() : Start_(std::chrono::steady_clock::now()) {
    }
    
    void Reset() {
        Start_ = std::chrono::steady_clock::now();
    }
    
    double CurrentElapsedMilliseconds() const {
        auto end = std::chrono::steady_clock::now();
        return std::chrono::duration<double, std::milli>(end - Start_).count();
    }
    
    double CurrentElapsedMicroseconds() const {
        auto end = std::chrono::steady_clock::now();
        return std::chrono::duration<double, std::micro>(end - Start_).count();
    }
    
    double CurrentElapsedSeconds() const {
        auto end = std::chrono::steady_clock::now();
        return std::chrono::duration<double, std::centi>(end - Start_).count();
    }

    std::string ToString() const {
        auto end = std::chrono::steady_clock::now();
        auto diff = end - Start_;
        std::ostringstream buff;
        buff << std::chrono::duration<double, std::centi>(diff).count() << "s";
        buff << "/";
        buff << std::chrono::duration<double, std::milli>(diff).count() << "ms";
        buff << "/";
        buff << std::chrono::duration<double, std::micro>(diff).count() << "us";
        return buff.str();
    }
    
  private:
    std::chrono::time_point<std::chrono::steady_clock> Start_;
};

int main() {
    
    ElapsedTime et;
    std::this_thread::sleep_for(std::chrono::microseconds(123400));
    std::cout << et.ToString() << std::endl;
}