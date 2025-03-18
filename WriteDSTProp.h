#ifndef WDP_H
#define WDP_H

#include "BaseModule.h"
#include "MessageQueue.h"

//input:res[n], edgenum, dstid[n]; output:VertexProperty[dstid]
class WriteDSTPropertyModule : public BaseModule {

    private:
        const double d = 0.85;
        const double threshold = 1e-6;  //
        const int N = 5;                //

    public:

        const char* name() const override { return "WriteDST"; }

        void process(const GraphData& inputData) override {
            int edgenum = inputData.edgenum;
    
            outputData.dstid.resize(edgenum);
            outputData.weight.resize(edgenum);
            outputData.Vprop.resize(edgenum);
            outputData.Vtprop.resize(edgenum);
            outputData.res.resize(edgenum);
    
            for (int i = 0; i < edgenum; i++) {
                int dst = inputData.dstid[i];
                if (dst < 0 || dst >= VertexProperty.size()) continue;
                
                //SSSP
                int old_value = VertexProperty[dst];
                int new_value = std::min(old_value, inputData.res[i]);
    
                if (new_value < old_value) {
                    VertexProperty[dst] = new_value;
                    outputData.activeVertices.insert(dst);
    
                    std::cout << "Updating VertexProperty[" << dst << "] from "
                              << old_value << " to " << new_value << std::endl;
                }

                //SSWP
                /*int old_value = VertexProperty[dst];
                int new_value = std::max(old_value, inputData.res[i]);

                if (new_value > old_value) {
                    VertexProperty[dst] = new_value;
                    outputData.activeVertices.insert(dst);

                    std::cout << "Updating VertexProperty[" << dst << "] from "
                              << old_value << " to " << new_value << std::endl;
                }*/

                outputData.dstid[i] = inputData.dstid[i];
                outputData.weight[i] = inputData.weight[i];
                outputData.Vprop[i] = inputData.Vprop[i];
                outputData.Vtprop[i] = inputData.Vtprop[i];
                outputData.res[i] = inputData.res[i];
            }
    
            outputData.srcid = inputData.srcid;
            outputData.Uprop = inputData.Uprop;
            outputData.edgenum = inputData.edgenum;

            outputData.total_processed_edges = inputData.total_processed_edges;
    
            advanceClock();
    
            std::cout << "Updated VertexProperty: ";
            for (int v : VertexProperty) {
                std::cout << v << " ";
            }
            std::cout << std::endl;
        }

        /*void process(const GraphData& inputData) override {
            for (const auto& entry : inputData.aggregated_contributions) {


                double hanging_contrib = 0.0;

                for (int src : inputData.activeVertices) {
                    if (offset[src+1] - offset[src] == 0) { 
                        hanging_contrib += VertexProperty[src];
                    }
                }

                double hanging_factor = d * hanging_contrib / N;

                int dstid = entry.first;
                double sum_contrib = entry.second;
                double old_pr = VertexProperty[dstid];
                double new_pr = (1.0 - d)/N + d * (sum_contrib + hanging_factor); 
    
                VertexProperty[dstid] = new_pr;
    
                if (std::abs(new_pr - old_pr) > threshold) {
                    outputData.activeVertices.insert(dstid);
                }
            }
        }*/
    
        void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
            BaseModule::sendMessage(queue, timestamp, targetModule);
        }
    };
        
#endif