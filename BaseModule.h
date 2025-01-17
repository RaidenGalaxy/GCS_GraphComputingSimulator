#ifndef BASEMODULE_H
#define BASEMODULE_H

#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include "MessageQueue.h"

class Simulator;

std::vector<int> VertexProperty = {0,10,20,30};
std::vector<int> offset = {0,3,4,6};
std::vector<int> edgeweight = {20,30,40,10,10,10,10};
std::vector<int> edge = {1,2,3,3,1,3,2};

class BaseModule {
protected:
    GraphData inputData;
    GraphData outputData;

public:
    BaseModule() = default;

    virtual ~BaseModule() {}

    virtual void receiveData(const GraphData& data) {
        inputData = data;
    }

    virtual void process(const GraphData& inputData) = 0;

    virtual void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) = 0;

    GraphData getOutputData() const {
        return outputData;
    }
};



#endif
