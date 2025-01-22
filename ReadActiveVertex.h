#ifndef RVP_H
#define RVP_H

#include "BaseModule.h"
#include "MessageQueue.h"

//input: src id  output: src id, Uprop
class ReadActiveVertexPropertyModule : public BaseModule { // get active vertex's prop
public:

    void process(const GraphData& inputData) override {

        int srcid = inputData.srcid;
        int Uprop = VertexProperty[srcid];
        outputData.srcid = srcid;
        outputData.Uprop = Uprop;
        

    }

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        BaseModule::sendMessage(queue, timestamp, targetModule);
    }
};

#endif