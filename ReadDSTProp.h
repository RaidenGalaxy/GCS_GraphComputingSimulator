#ifndef RDP_H
#define RDP_H

#include "BaseModule.h"
#include "MessageQueue.h"


//input: Uprop, weight[n], dstid[n], edgenum  output:Uprop, weight[n], Vprop[n], edgenum, dstid[n]
class ReadDSTPropertyModule : public BaseModule {
public:

    const char* name() const override { return "ReadEdge"; }

    void process(const GraphData& inputData) override {
    int srcid = inputData.srcid;
    double Uprop = inputData.Uprop;
    int edgenum = inputData.edgenum;

    outputData.dstid.resize(edgenum);
    outputData.weight.resize(edgenum);
    outputData.Vprop.resize(edgenum);

    for (int i = 0; i < edgenum; i++) {
        if (i >= 0 && i < inputData.dstid.size()) {
            int dst = inputData.dstid[i];
            if (dst >= 0 && dst < VertexProperty.size()) {
                outputData.Vprop[i] = VertexProperty[dst];
            } else {
                std::cerr << "Invalid dst index: " << dst << std::endl;
                outputData.Vprop[i] = -1;
            }
            outputData.dstid[i] = dst;
            outputData.weight[i] = inputData.weight[i];
        } else {
            std::cerr << "Index out of bounds: " << i << std::endl;
            return;
        }
    }

    outputData.srcid = srcid;
    outputData.Uprop = Uprop;
    outputData.edgenum = edgenum;

    for (int dst : outputData.dstid) {
        outputData.activeVertices.insert(dst);
    }
    
    outputData.activeVertices = inputData.activeVertices;

    outputData.total_processed_edges = inputData.total_processed_edges;

    advanceClock();
    

    }

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        BaseModule::sendMessage(queue, timestamp, targetModule);
    }
};

#endif