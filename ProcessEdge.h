#ifndef PE_H
#define PE_H

#include "BaseModule.h"
#include "MessageQueue.h"

class ProcessEdgeModule : public BaseModule {
private:

    void initializeOutputData(const GraphData& inputData) {
        int edgenum = inputData.edgenum;
        outputData.dstid.resize(edgenum);
        outputData.weight.resize(edgenum);
        outputData.Vprop.resize(edgenum);
        outputData.Vtprop.resize(edgenum);
        outputData.srcid = inputData.srcid;
        outputData.Uprop = inputData.Uprop;
        outputData.edgenum = edgenum;
    }

public:

    void process(const GraphData& inputData) override {

        initializeOutputData(inputData);

        int edgenum = inputData.edgenum;

        for (int i = 0; i < edgenum; i++) {
            if (i >= 0 && i < inputData.weight.size()) {
                outputData.Vtprop[i] = inputData.Uprop + inputData.weight[i];
                outputData.dstid[i] = inputData.dstid[i];
                outputData.weight[i] = inputData.weight[i];
                outputData.Vprop[i] = inputData.Vprop[i];

                if (timeManager) {
                    timeManager->incrementGlobalClock();
                }

            } else {
                std::cerr << "PE Index out of bounds: " << i << std::endl;
                return;
            }
        }

        outputData.srcid = inputData.srcid;
        outputData.Uprop = inputData.Uprop;
        outputData.edgenum = edgenum;
    }
    

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        BaseModule::sendMessage(queue, timestamp, targetModule);
    }
};

#endif
