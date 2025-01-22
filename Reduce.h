#ifndef REDUCE_H
#define REDUCE_H

#include "BaseModule.h"
#include "MessageQueue.h"


//input:Vprop[n], Vtprop[n], edgenum, dstid[n]; output:res[n], edgenum, dstid[n]
class ReduceModule : public BaseModule {  //min(Vprop,Vtprop)
public:
    void process(const GraphData& inputData) override {

        int edgenum = inputData.edgenum;
        int res[edgenum];

        outputData.dstid.resize(edgenum);
        outputData.weight.resize(edgenum);
        outputData.Vprop.resize(edgenum);
        outputData.Vtprop.resize(edgenum);
        outputData.res.resize(edgenum);
        //min();
        for(int i = 0; i < edgenum; i++){
            if(i >= 0 && i < edgeweight.size()){
                if(inputData.Vprop[i] < inputData.Vtprop[i]){
                    res[i] = inputData.Vprop[i];
                }
                else res[i] = inputData.Vtprop[i];

                if (timeManager) {
                    timeManager->incrementGlobalClock();
                }
                
            }
            else {
                std::cerr << "Index out of bounds: " << i << std::endl;
                return;
            }

            outputData.dstid[i] = inputData.dstid[i];
            outputData.weight[i] = inputData.weight[i];
            outputData.Vprop[i] = inputData.Vprop[i];
            outputData.Vtprop[i] = inputData.Vtprop[i];
            outputData.res[i] = res[i];

        }
        outputData.srcid = inputData.srcid;
        outputData.Uprop = inputData.Uprop;
        outputData.edgenum = inputData.edgenum;
    }

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        BaseModule::sendMessage(queue, timestamp, targetModule);
    }
};

#endif