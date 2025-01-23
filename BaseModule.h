#ifndef BASEMODULE_H
#define BASEMODULE_H

#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include "MessageQueue.h"
#include "TimeManager.h"

std::vector<int> VertexProperty = {1,25,35,45};
std::vector<int> offset = {0,3,4,6};
std::vector<int> edgeweight = {20,30,40,10,10,10,10};
std::vector<int> edge = {1,2,3,3,1,3,2};

class BaseModule {
protected:
    GraphData inputData;
    GraphData outputData;
    GraphData lastSentData;

    int localClock = 0;

    TimeManager* timeManager = nullptr;

public:

    void setTimeManager(TimeManager* manager) {
        timeManager = manager;
    }

    BaseModule() = default;

    virtual ~BaseModule() {}

    virtual void receiveData(const GraphData& data) {
        inputData = data;
    }

    virtual void process(const GraphData& inputData) = 0;

    //virtual void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) = 0;

    virtual void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) {
        if (outputData != lastSentData) {
            queue.addMessage(timestamp, targetModule, outputData);
            lastSentData = outputData;
        }
    }

    void advanceClock() {
        localClock++;
    }

    int getLocalClock() const {
        return localClock;
    }

    GraphData getOutputData() const {
        return outputData;
    }
};



#endif
