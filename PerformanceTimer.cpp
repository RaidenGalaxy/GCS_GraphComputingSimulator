#include "PerformanceTimer.h"

std::unordered_map<std::string, PerformanceTimer::TimePoint> PerformanceTimer::timers;

void PerformanceTimer::Start(const std::string& name) {
    timers[name] = Clock::now();
}

long PerformanceTimer::Stop(const std::string& name) {
    auto end = Clock::now();
    return std::chrono::duration_cast<Duration>(end - timers[name]).count();
}

void PerformanceTimer::Clear() {
    timers.clear();
}