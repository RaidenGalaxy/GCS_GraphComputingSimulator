#include "BaseModule.h"
#include "MessageQueue.h"

class VertexTraversalModule : public BaseModule { //traversal the graph and get every vertex's outneighbor
public:
    //traversal vertex's out edge
    void process() override {
        for (int i = 1; i < inputData.size(); i++) {//vertex id = i;
            int vid = i;
            int start = offset[vid];                
            int end = offset[vid+1];                

            for (int j = start; j < end; j++) {
                int outneighbor = edge[j];          //get the outneighbor
                outputData.push_back(outneighbor);
            }
        }
    }

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        queue.addMessage(timestamp, targetModule, outputData);
    }
};
