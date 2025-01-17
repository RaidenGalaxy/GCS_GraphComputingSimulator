#ifndef PE_H
#define PE_H

#include "BaseModule.h"
#include "MessageQueue.h"

//input:Uprop, weight[n], Vprop[n], edgenum, dstid[n]  output:Vprop[n], Vtprop[n], edgenum, dstid[n];
class ProcessEdgeModule : public BaseModule {
public:
    void process(const GraphData& inputData) override {
        int srcid = inputData.srcid;
        int Uprop = inputData.Uprop;
        int edgenum = inputData.edgenum;
        std::vector<int> weight(edgenum);
        std::vector<int> Vtprop(edgenum);

        outputData.dstid.resize(edgenum);
        outputData.weight.resize(edgenum);
        outputData.Vprop.resize(edgenum);
        outputData.Vtprop.resize(edgenum);


        for(int i = 0; i < edgenum; i++){
            if(i >= 0 && i < edgeweight.size()){

            outputData.Vtprop[i] = inputData.Uprop + inputData.weight[i];

            outputData.dstid[i] = inputData.dstid[i];
            outputData.weight[i] = inputData.weight[i];
            outputData.Vprop[i] = inputData.Vprop[i];

            }
            else {
                std::cerr << "Index out of bounds: " << i << std::endl;
                return;
            }
        outputData.srcid = srcid;
        outputData.Uprop = Uprop;
        outputData.edgenum = edgenum;

        }
    
    }

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        queue.addMessage(timestamp, targetModule, outputData);
    }
};

#endif