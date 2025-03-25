#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <sys/resource.h>
#include <unistd.h>

class MemoryMonitor {
public:
    static size_t GetPeakMemory() {
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        return usage.ru_maxrss * 1024;
    }
};

#endif