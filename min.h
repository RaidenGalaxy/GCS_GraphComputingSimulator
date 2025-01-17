#include "BaseModule.h"
#include "MessageQueue.h"

class Min : public BaseModule {
public:
    void process() override {
        int size = inputData.size();
        if (size = 2) {
        int min = (inputData[0] < inputData[1]) ? inputData[0] : inputData[1];
        outputData.push_back(min);
        }
    }
    

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        queue.addMessage(timestamp, targetModule, outputData);
    }
};
