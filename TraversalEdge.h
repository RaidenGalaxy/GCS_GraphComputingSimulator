#include "BaseModule.h"
#include "MessageQueue.h"

class EdgeTraversalandLoadModule : public BaseModule {
public:
    void process() override {
        for (int i = 1; i < inputData.size(); i++) {//vertex u id = i;
            int uid = i; 
            int start = offset[uid];                //inputData[0][uid]??                
            int end = offset[uid+1];                //inputdata[0][uid + 1]??     

            //traversal edge and load edge weight
            for (int j = start; j < end; j++) {
                int weight= edgeweight[j];          //inputData[3][j]; 
                outputData.push_back(weight);
            }
        }
    }

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        queue.addMessage(timestamp, targetModule, outputData);
    }
};
