#include "simulator.h"
#include "BaseModule.h"
#include "MessageQueue.h"
#include "ReadActiveVertex.h"
#include "ReadEdgeID.h"
#include "ReadEdges.h"
#include "ReadDSTProp.h"
#include "ProcessEdge.h"
#include "Reduce.h"
#include "WriteDSTProp.h"
#include <memory>

int main() {
    Simulator simulator;

    auto readActiveVertexModule = std::make_shared<ReadActiveVertexPropertyModule>();
    auto readEdgeIDModule = std::make_shared<ReadEdgeIDModule>();
    auto readEdgeModule = std::make_shared<ReadEdgeModule>();
    auto readDSTModule = std::make_shared<ReadDSTPropertyModule>();
    auto processEdge = std::make_shared<ProcessEdgeModule>();
    auto reduce = std::make_shared<ReduceModule>();
    auto writeDST = std::make_shared<WriteDSTPropertyModule>();

    GraphData initialData;
    initialData.srcid = 0;
    initialData.Uprop = 0;
    initialData.edgenum = 0;
    initialData.dstid = {0, 0, 0};
    initialData.weight = {0, 0, 0, 0, 0, 0, 0};
    initialData.Vprop = {0, 0, 0, 0};
    initialData.Vtprop = {0, 0, 0, 0};

    simulator.addModule(readActiveVertexModule);
    simulator.addModule(readEdgeIDModule);
    simulator.addModule(readEdgeModule);
    simulator.addModule(readDSTModule);
    simulator.addModule(processEdge);
    simulator.addModule(reduce);
    simulator.addModule(writeDST);

    readActiveVertexModule->process(initialData);

    readActiveVertexModule->sendMessage(simulator.messageQueue, 0, readEdgeIDModule.get());

    readEdgeIDModule->process(readActiveVertexModule->getOutputData());

    readEdgeIDModule->sendMessage(simulator.messageQueue, 0, readEdgeModule.get());

    readEdgeModule->process(readEdgeIDModule->getOutputData());

    readEdgeModule->sendMessage(simulator.messageQueue, 0, readDSTModule.get());

    readDSTModule->process(readEdgeModule->getOutputData());

    readDSTModule->sendMessage(simulator.messageQueue, 0, processEdge.get());

    processEdge->process(readDSTModule->getOutputData());

    processEdge->sendMessage(simulator.messageQueue, 0, reduce.get());

    reduce->process(processEdge->getOutputData());

    reduce->sendMessage(simulator.messageQueue, 0, writeDST.get());

    writeDST->process(reduce->getOutputData());


    simulator.run();
    simulator.printResults();

    return 0;
}
