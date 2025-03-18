#ifndef RE_H
#define RE_H

#include "BaseModule.h"
#include "MessageQueue.h"

//input: src id, Uprop, dst id, edgenum  output: Uprop, weight[n], dstid, edgenum
class ReadEdgeModule : public BaseModule { // get edge's weight
public:

    const char* name() const override { return "ReadEdge"; }

    void process(const GraphData& inputData) override {
        
    int srcid = inputData.srcid;
    double Uprop = inputData.Uprop;
    int edgenum = inputData.edgenum;
    int start = offset[srcid];

    outputData.dstid.resize(edgenum);
    outputData.weight.resize(edgenum);

    for (int i = 0; i < edgenum; i++) {
        int edge_idx = start + i;
        if (edge_idx >= 0 && edge_idx < edgeweight.size()) {
            outputData.weight[i] = edgeweight[edge_idx];
            outputData.dstid[i] = inputData.dstid[i];
        } else {
            std::cerr << "Index out of bounds: " << edge_idx << std::endl;
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

    /*void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        queue.addMessage(timestamp, targetModule, outputData);
    }*/

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        BaseModule::sendMessage(queue, timestamp, targetModule);
    }

};

#endif