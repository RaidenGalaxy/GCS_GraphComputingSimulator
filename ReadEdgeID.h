/*#ifndef REID_H
#define REID_H

#include "BaseModule.h"
#include "MessageQueue.h"

//input: src id, Uprop  output: src id, Uprop, dst id, edgenum
class ReadEdgeIDModule : public BaseModule { // get edge's id
public:
    void process(GraphData& inputData) override {

        int srcid = inputData.srcid;
        int Uprop = inputData.Uprop;
        int start = offset[srcid];
        int end = offset[srcid + 1];
        int edgenum = end - start;
        std::vector<int> dstid(edgenum);
        outputData.dstid.resize(edgenum);

        for(int i = start; i < end ; i++){
            dstid[i - start] = edge[i];              // on-chip memory
            outputData.dstid[i] = dstid[i - start];
        }
        outputData.edgenum = edgenum;
        outputData.Uprop = Uprop;
        outputData.srcid = srcid;
    }

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        queue.addMessage(timestamp, targetModule, outputData);
    }
};

#endif
*/

#ifndef REID_H
#define REID_H

#include "BaseModule.h"
#include "MessageQueue.h"

//input: src id, Uprop  output: src id, Uprop, dst id, edgenum
class ReadEdgeIDModule : public BaseModule { // get edge's id
public:
    void process(const GraphData& inputData) override {
        int srcid = inputData.srcid;
        int Uprop = inputData.Uprop;

        if (srcid < 0 || srcid >= offset.size()) {
            std::cerr << "Invalid srcid: " << srcid << std::endl;
            return;
        }

        int start = offset[srcid];
        int end = offset[srcid + 1];
        int edgenum = end - start;

        std::vector<int> dstid(edgenum);
        outputData.dstid.resize(edgenum);

        for (int i = start; i < end; i++) {

            if (i >= 0 && i < edge.size()) {
                dstid[i - start] = edge[i];
                outputData.dstid[i - start] = dstid[i - start];
            } else {
                std::cerr << "Index out of bounds: " << i << std::endl;
                return;
            }
        }

        outputData.edgenum = edgenum;
        outputData.Uprop = Uprop;
        outputData.srcid = srcid;

    }

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        queue.addMessage(timestamp + 1, targetModule, outputData);
    }
};

#endif
