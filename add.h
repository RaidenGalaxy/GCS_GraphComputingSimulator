#include "BaseModule.h"
#include "MessageQueue.h"

class Add : public BaseModule {
public:
    void process() override {
        int size = inputData.size();
        if (size = 2) {
        int sum = inputData[0] + inputData[1];
        outputData.push_back(sum);
        }
    }

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        queue.addMessage(timestamp, targetModule, outputData);
    }
};
