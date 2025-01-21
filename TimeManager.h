#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

class TimeManager {
public:
    virtual ~TimeManager() = default;

    virtual void incrementGlobalClock() = 0;

    virtual int getGlobalClock() const = 0;
};

#endif
