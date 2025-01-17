#include "BaseModule.h"

class LoadVertexValueModule : public BaseModule {
public:
    void process() override {
        for (int i = 1; i < inputData.size(); i++) {
            int vid = inputData[i];
            int Vertex_Value[vid] = VertexProperty[vid];
            outputData.push_back(Vertex_Value[vid]);
        }
    }

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        queue.addMessage(timestamp, targetModule, outputData);
    }
};
