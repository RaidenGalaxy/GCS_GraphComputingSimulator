#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include "ReadActiveVertex.h"
#include "ReadEdgeID.h"
#include "ReadEdges.h"
#include "ReadDSTProp.h"
#include "ProcessEdge.h"
#include "Reduce.h"
#include "WriteDSTProp.h"
#include "simulator.h"
#include "MessageQueue.h"

int main() {
    MessageQueue messageQueue;

    ReadActiveVertexPropertyModule ReadVertex;
    ReadEdgeIDModule ReadEdgeID;
    ReadEdgeModule ReadEdge;
    ReadDSTPropertyModule ReadDSTProp;
    ProcessEdgeModule ProcessEdge;
    ReduceModule Reduce;
    WriteDSTPropertyModule WriteDSTProp;

    ReadVertex.receiveData();
    readEdgeID.receiveData({2});
    readEdges.receiveData({5});
    readDSTProp.receiveData({3});

    // 处理数据并进行消息传递
    readActiveVertex.process(GraphData& inputData);
    readActiveVertex.sendMessage(messageQueue, 1, &readEdgeID);

    readEdgeID.process();
    readEdgeID.sendMessage(messageQueue, 2, &readEdges);

    readEdges.process();
    readEdges.sendMessage(messageQueue, 3, &readDSTProp);

    readDSTProp.process();
    readDSTProp.sendMessage(messageQueue, 4, &processEdge);

    processEdge.process();
    processEdge.sendMessage(messageQueue, 5, &reduce);

    reduce.process();
    reduce.sendMessage(messageQueue, 6, &writeDSTProp);

    writeDSTProp.process();

    // 结果输出
    std::cout << "Final Output: ";
    for (int val : writeDSTProp.getOutputData()) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}