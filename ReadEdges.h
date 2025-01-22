#ifndef RE_H
#define RE_H

#include "BaseModule.h"
#include "MessageQueue.h"

//input: src id, Uprop, dst id, edgenum  output: Uprop, weight[n], dstid, edgenum
class ReadEdgeModule : public BaseModule { // get edge's weight
public:
    void process(const GraphData& inputData) override {
        
        int srcid = inputData.srcid;
        int Uprop = inputData.Uprop;
        int edgenum = inputData.edgenum;
        
        std::vector<int> weight(edgenum);
        outputData.dstid.resize(edgenum);
        outputData.weight.resize(edgenum);

        for(int i = 0; i < edgenum ; i++){
            if(i >= 0 && i < edgeweight.size()){
                weight[i] = edgeweight[i];
            }
            else {
                std::cerr << "Index out of bounds: " << i << std::endl;
                return;
            }

            outputData.dstid[i] = inputData.dstid[i]; //dstid[edgenum]
            outputData.weight[i] = weight[i];
        }
        outputData.srcid = srcid;
        outputData.Uprop = Uprop;
        outputData.edgenum = edgenum;

        
    }

    /*void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        queue.addMessage(timestamp, targetModule, outputData);
    }*/

   void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        BaseModule::sendMessage(queue, timestamp, targetModule);
    }

};

#endif