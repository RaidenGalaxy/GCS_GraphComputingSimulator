#ifndef PE_H
#define PE_H

#include "BaseModule.h"
#include "MessageQueue.h"

class ProcessEdgeModule : public BaseModule {
private:

    void initializeOutputData(const GraphData& inputData) {
        int edgenum = inputData.edgenum;
        outputData.dstid.resize(edgenum);
        outputData.weight.resize(edgenum);
        outputData.Vprop.resize(edgenum);
        outputData.Vtprop.resize(edgenum);
        outputData.contributions.resize(edgenum);
        outputData.srcid = inputData.srcid;
        outputData.Uprop = inputData.Uprop;
        outputData.edgenum = edgenum;
    }

public:

    const char* name() const override { return "ProcessEdge"; }

    void process(const GraphData& inputData) override {

        initializeOutputData(inputData);

        int edgenum = inputData.edgenum;
        int start = offset[inputData.srcid];
        int end = offset[inputData.srcid + 1];
        int out_degree = end - start;

        for (int i = 0; i < edgenum; i++) {
            if (i >= 0 && i < inputData.weight.size()) {

                outputData.Vtprop[i] = inputData.Uprop + inputData.weight[i];//SSSP

                //outputData.Vtprop[i] = std::min(inputData.Uprop, inputData.weight[i]);//SSWP

                //double pr = inputData.Uprop;//PR
                //outputData.contributions[i] = pr / out_degree;

                outputData.dstid[i] = inputData.dstid[i];
                outputData.weight[i] = inputData.weight[i];
                outputData.Vprop[i] = inputData.Vprop[i];
                
                outputData.activeVertices.insert(outputData.dstid[i]);

                outputData.total_processed_edges = inputData.total_processed_edges + edgenum;

                advanceClock();

                /*std::cout << "Calculating contribution: src=" << inputData.srcid 
                << ", pr=" << pr 
                << ", out_degree=" << out_degree 
                << ", contrib=" << (pr/out_degree) << std::endl;*/


            } else {
                std::cerr << "PE Index out of bounds: " << i << std::endl;
                return;
            }
        }

        outputData.srcid = inputData.srcid;
        outputData.Uprop = inputData.Uprop;
        outputData.edgenum = edgenum;

        outputData.activeVertices = inputData.activeVertices;
    }
    

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        BaseModule::sendMessage(queue, timestamp, targetModule);
    }
};

#endif
