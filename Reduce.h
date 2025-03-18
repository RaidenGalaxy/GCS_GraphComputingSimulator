#ifndef REDUCE_H
#define REDUCE_H

#include "BaseModule.h"
#include "MessageQueue.h"
#include <limits>
#include <unordered_set>


//input:Vprop[n], Vtprop[n], edgenum, dstid[n]; output:res[n], edgenum, dstid[n]
class ReduceModule : public BaseModule {
    public:

    const char* name() const override { return "Reduce"; }

        void process(const GraphData& inputData) override {
            int edgenum = inputData.edgenum;
            std::vector<int> res(edgenum, std::numeric_limits<int>::max());
    
            outputData.dstid.resize(edgenum);
            outputData.weight.resize(edgenum);
            outputData.Vprop.resize(edgenum);
            outputData.Vtprop.resize(edgenum);
            outputData.res.resize(edgenum);
    
            for (int i = 0; i < edgenum; i++) {
                if (i < 0 || i >= inputData.dstid.size()) {
                    std::cerr << "ReduceModule: Index out of bounds: " << i << std::endl;
                    return;
                }
    
                res[i] = std::min(inputData.Vprop[i], inputData.Vtprop[i]);//SSSP

                //res[i] = std::max(inputData.Vprop[i], inputData.Vtprop[i]);//SSWP
    
                outputData.dstid[i] = inputData.dstid[i];
                outputData.weight[i] = inputData.weight[i];
                outputData.Vprop[i] = inputData.Vprop[i];
                outputData.Vtprop[i] = inputData.Vtprop[i];
                outputData.res[i] = res[i];

                std::cout << "Reduce Debug: Vprop[" << i << "] = " << inputData.Vprop[i]
                << ", Vtprop[" << i << "] = " << inputData.Vtprop[i] << std::endl;

    
                if (res[i] < inputData.Vtprop[i]) {
                    outputData.activeVertices.insert(outputData.dstid[i]);
                }
            }
    
            outputData.srcid = inputData.srcid;
            outputData.Uprop = inputData.Uprop;
            outputData.edgenum = inputData.edgenum;

            outputData.total_processed_edges = inputData.total_processed_edges;
    
            advanceClock();
    
            std::cout << "Reduce: res = ";
            for (int v : outputData.res) std::cout << v << " ";
            std::cout << std::endl;
        }

        /*void process(const GraphData& inputData) override {
            std::unordered_map<int, double> contributions_map;
            
            for (int i = 0; i < inputData.edgenum; i++) {
                int dst = inputData.dstid[i];
                contributions_map[dst] += inputData.contributions[i];
                
                std::cout << "[Reduce] Add contribution " << inputData.contributions[i]
                          << " to dst=" << dst << std::endl;
            }
            
            outputData.aggregated_contributions = contributions_map;
        }*/
    
        void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
            BaseModule::sendMessage(queue, timestamp, targetModule);
        }
    };

#endif