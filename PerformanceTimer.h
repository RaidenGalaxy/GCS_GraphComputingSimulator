#ifndef PERFORMANCE_TIMER_H
#define PERFORMANCE_TIMER_H

#include <chrono>
#include <unordered_map>
#include <string>

class PerformanceTimer {
public:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    using Duration = std::chrono::microseconds;

    static void Start(const std::string& name);
    static long Stop(const std::string& name);
    static void Clear();

private:
    static std::unordered_map<std::string, TimePoint> timers;
};

#endif