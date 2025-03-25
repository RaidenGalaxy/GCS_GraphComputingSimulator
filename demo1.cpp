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
#include "PerformanceTimer.h"
//#include "mem.h"
//#include "def.h"
//#include <memory>

//using namespace ramulator;

int main() {


    std::string offset_file = "/home/egalaxy/Desktop/simulator/test_src/test_generate/ofs/ofs_9";
    std::string edge_file = "/home/egalaxy/Desktop/simulator/test_src/test_generate/edges/edges_9";
    std::string edgeweight_file = "/home/egalaxy/Desktop/simulator/test_src/graph_generate/BFS_weight_5000e";

    Simulator simulator(offset_file, edge_file, edgeweight_file, "SSSP");

    /*uint64_t swap_start;
    MEM::runtime_config run_cfg;

    /simulator.initMemory(swap_start,run_cfg);

    if (!simulator.memory) {
        std::cerr << "Error: Memory initialization failed!" << std::endl;
        return 1;
    }
        */


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
    initialData.weight = {0, 0, 0, 0, 0, 0};
    initialData.Vprop = {0, 0, 0, 0};
    initialData.Vtprop = {0, 0, 0, 0};
    initialData.total_processed_edges = 0;

    initialData.activeVertices.insert(initialData.srcid);
    simulator.activeVertices.insert(initialData.srcid);

    simulator.addModule(readActiveVertexModule);
    simulator.addModule(readEdgeIDModule);
    simulator.addModule(readEdgeModule);
    simulator.addModule(readDSTModule);
    simulator.addModule(processEdge);
    simulator.addModule(reduce);
    simulator.addModule(writeDST);

    
    simulator.run();
    //simulator.printResults();
    
    return 0;
}