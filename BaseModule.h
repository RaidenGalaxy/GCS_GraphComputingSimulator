#ifndef BASEMODULE_H
#define BASEMODULE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <functional>
#include "MessageQueue.h"
#include "TimeManager.h"
#include "PerformanceTimer.h"

#define MIN 0
#define MAX 99999


extern std::vector<int> offset;
extern std::vector<int> edge;
extern std::vector<double> edgeweight;
extern std::vector<int> VertexProperty;
extern std::unordered_set<int> activeVertices;
//SSSP
/*std::vector<int> VertexProperty = {0, MAX, MAX, MAX, MAX, MAX, MAX};
std::vector<int> offset = {0, 2, 4, 6, 8, 9, 10, 11};
std::vector<int> edgeweight = {5, 10, 5, 20, 15, 25, 10, 30, 10, 5, 10};
std::vector<int> edge = {1, 2, 2, 3, 3, 4, 4, 5, 6, 6, 6};
std::unordered_set<int> activeVertices = {0};*/

//SSWP
/*std::vector<int> VertexProperty = {MAX, MIN, MIN, MIN, MIN};
std::vector<int> offset = {0, 2, 3, 5, 6, 6};
std::vector<int> edge = {1, 2, 3, 3, 4, 2};
std::vector<int> edgeweight = {5, 10, 15, 5, 10, 20}; 
std::unordered_set<int> activeVertices = {0};*/

//PR
/*std::vector<double> VertexProperty = {0.2, 0.2, 0.2, 0.2, 0.2};
std::vector<int> offset = {0, 2, 3, 5, 6, 6};
std::vector<int> edge = {1, 2, 3, 3, 4, 2};
std::vector<int> edgeweight = {1, 1, 1, 1, 1, 1};*/




class BaseModule {
protected:
    GraphData inputData;
    GraphData outputData;
    GraphData lastSentData;

    int localClock = 0;

    TimeManager* timeManager = nullptr;

    virtual void preProcess() {
        PerformanceTimer::Start(this->name());
    }
    
    virtual void postProcess() {
        auto duration = PerformanceTimer::Stop(this->name());
    }
    
    virtual const char* name() const = 0;

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
