#ifndef WDP_H
#define WDP_H

#include "BaseModule.h"
#include "MessageQueue.h"


//input:res[n], edgenum, dstid[n]; output:VertexProperty[dstid]
class WriteDSTPropertyModule : public BaseModule {  //min(Vprop,Vtprop)
public:
    void process(const GraphData& inputData) override {

        int edgenum = inputData.edgenum;

        outputData.dstid.resize(edgenum);
        outputData.weight.resize(edgenum);
        outputData.Vprop.resize(edgenum);
        outputData.Vtprop.resize(edgenum);
        outputData.res.resize(edgenum);

        for(int i = 0; i < edgenum; i++){
            if(i >= 0 && i < edgeweight.size()){
                VertexProperty[inputData.dstid[i]] = inputData.res[i];
            }
            else {
                std::cerr << "Index out of bounds: " << i << std::endl;
                return;
            }
            outputData.dstid[i] = inputData.dstid[i];
            outputData.weight[i] = inputData.weight[i];
            outputData.Vprop[i] = inputData.Vprop[i];
            outputData.Vtprop[i] = inputData.Vtprop[i];
            outputData.res[i] = inputData.res[i];

        }
        
        outputData.srcid = inputData.srcid;
        outputData.Uprop = inputData.Uprop;
        outputData.edgenum = inputData.edgenum;
    }

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        queue.addMessage(timestamp, targetModule, outputData);
    }
};

#endif