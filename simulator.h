#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "BaseModule.h"
#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <memory>
#include <numeric>
#include "ReadActiveVertex.h"
#include "ReadEdgeID.h"
#include "ReadEdges.h"
#include "ReadDSTProp.h"
#include "ProcessEdge.h"
#include "Reduce.h"
#include "WriteDSTProp.h"
#include "MessageQueue.h"

#include "GraphLoader.h"
#include "Globaldata.h"

#include "PerformanceTimer.h"
#include "MemoryMonitor.h"
//#include "mem.h"
//#include "Config.h"
//#include "def.h"

//using namespace ramulator;

class Simulator {

public:

    std::vector<std::shared_ptr<BaseModule>> modules;
    MessageQueue messageQueue;
    std::unordered_set<int> activeVertices;
    //std::shared_ptr<MEM::single_mem> memory;

    std::shared_ptr<ReadActiveVertexPropertyModule> readActiveVertex;
    std::shared_ptr<ReadEdgeIDModule> readEdgeID;
    std::shared_ptr<ReadEdgeModule> readEdge;
    std::shared_ptr<ReadDSTPropertyModule> readDST;
    std::shared_ptr<ProcessEdgeModule> processEdge;
    std::shared_ptr<ReduceModule> reduce;
    std::shared_ptr<WriteDSTPropertyModule> writeDST;

    int globalClock = 0;
    int localClock = 0;
    int maxIterations = 10000;



    Simulator(const std::string& offset_path,
                const std::string& edge_path,
                const std::string& weight_path,
                const std::string& algo_type) {
    try {
        GraphLoader::LoadOffset(offset_path, offset);
        GraphLoader::LoadEdge(edge_path, edge);
        GraphLoader::LoadWeight(weight_path, edgeweight);
        
        //init vprop
        int num_vertices = offset.size() - 1;
        if (algo_type == "PR") {
            GraphLoader::InitVertexProperty(VertexProperty, num_vertices, algo_type);
        } else {
            std::vector<int> vprop;
            GraphLoader::InitVertexProperty(vprop, num_vertices, algo_type);
            //trans to double
            VertexProperty.assign(vprop.begin(), vprop.end());
        }
        
        //init active vertices
        if (algo_type == "SSSP" || algo_type == "SSWP") {
            activeVertices.insert(0); //default
        } else {
            for (int i = 0; i < num_vertices; ++i) {
                activeVertices.insert(i);
            }
        }
        

        readActiveVertex = std::make_shared<ReadActiveVertexPropertyModule>();
        readEdgeID = std::make_shared<ReadEdgeIDModule>();
        readEdge = std::make_shared<ReadEdgeModule>();
        readDST = std::make_shared<ReadDSTPropertyModule>();
        processEdge = std::make_shared<ProcessEdgeModule>();
        reduce = std::make_shared<ReduceModule>();
        writeDST = std::make_shared<WriteDSTPropertyModule>();

        std::cout << "All modules initialized successfully!\n";
    } catch (const std::exception& e) {
        std::cerr << "Exception while initializing modules: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    } catch (...) {
        std::cerr << "Unknown error while initializing modules!" << std::endl;
        exit(EXIT_FAILURE);
        }
    }


    void addModule(std::shared_ptr<BaseModule> module) {
        modules.push_back(module);
    }

    //SSSP & SSWP
    void run() {

        PerformanceStats stats;
        size_t total_edges = 0;//TEPS

        size_t peak_memory = 0;//MEM

        int iterationCount = 0;
        std::unordered_set<int> activeVertices = {0};
        size_t total_processed_edges = 0;//TEPS

        auto start_time = PerformanceTimer::Clock::now();//TEPS

        size_t total_comm_bytes = 0;//COMMUNICATE

        std::vector<GraphData> module_outputs;//COMMUNICATE
    
        while (iterationCount < maxIterations && !activeVertices.empty()) {

            auto iter_start = PerformanceTimer::Clock::now();//TEPS
            
            size_t iter_comm = 0;//COMMUNICATE


            std::cout << "Iteration " << iterationCount 
                      << " started. Active vertices: " << activeVertices.size() << std::endl;
    
            std::unordered_set<int> newActiveVertices;
            std::vector<int> oldVertexProperty = VertexProperty;
    
            for (int src : activeVertices) {
                GraphData currentData;
                currentData.srcid = src;
                currentData.activeVertices.insert(src);
    
                PerformanceTimer::Start("ReadActiveVertex");
                readActiveVertex->process(currentData);
                stats.module_times["ReadActiveVertex"] += PerformanceTimer::Stop("ReadActiveVertex");

                PerformanceTimer::Start("ReadEdgeID");
                readEdgeID->process(readActiveVertex->getOutputData());
                stats.module_times["ReadEdgeID"] += PerformanceTimer::Stop("ReadEdgeID");

                PerformanceTimer::Start("ReadEdge");
                readEdge->process(readEdgeID->getOutputData());
                stats.module_times["ReadEdge"] += PerformanceTimer::Stop("ReadEdge");

                PerformanceTimer::Start("ReadDST");
                readDST->process(readEdge->getOutputData());
                stats.module_times["ReadDST"] += PerformanceTimer::Stop("ReadDST");

                PerformanceTimer::Start("ProcessEdge");
                processEdge->process(readDST->getOutputData());
                stats.module_times["ProcessEdge"] += PerformanceTimer::Stop("ProcessEdge");

                PerformanceTimer::Start("Reduce");
                reduce->process(processEdge->getOutputData());
                stats.module_times["Reduce"] += PerformanceTimer::Stop("Reduce");

                PerformanceTimer::Start("WriteDST");
                writeDST->process(reduce->getOutputData());
                stats.module_times["WriteDST"] += PerformanceTimer::Stop("WriteDST");

                total_processed_edges = writeDST->getOutputData().total_processed_edges;


                auto iter_end = PerformanceTimer::Clock::now();
                stats.iteration_times.push_back(
                    std::chrono::duration_cast<PerformanceTimer::Duration>(iter_end - iter_start).count()
                );

                total_edges += currentData.edgenum;


                module_outputs.push_back(readActiveVertex->getOutputData());
                module_outputs.push_back(readEdgeID->getOutputData());
                module_outputs.push_back(readEdge->getOutputData());
                module_outputs.push_back(readDST->getOutputData());
                module_outputs.push_back(processEdge->getOutputData());
                module_outputs.push_back(reduce->getOutputData());
                module_outputs.push_back(writeDST->getOutputData());
    
                for (size_t i = 0; i < VertexProperty.size(); ++i) {
                    if (VertexProperty[i] != oldVertexProperty[i]) {  
                        newActiveVertices.insert(i);
                    }
                }
            }
    
            activeVertices = std::move(newActiveVertices);
            iterationCount++;


            for (auto& data : module_outputs) {
                iter_comm += data.CalculateSize();
            }

            stats.comm_mb_per_iter.push_back(iter_comm / (1024.0 * 1024.0));
            total_comm_bytes += iter_comm;

            auto current_time = PerformanceTimer::Clock::now();
            double elapsed_sec = std::chrono::duration<double>(current_time - start_time).count();
            double teps = total_processed_edges / elapsed_sec;
            std::cout << "Current TEPS: " << teps << "\n";

            size_t current_mem = MemoryMonitor::GetPeakMemory();
            if (current_mem > stats.peak_memory) {
                stats.peak_memory = current_mem;
            }

        }

        auto total_time = std::accumulate(stats.iteration_times.begin(), stats.iteration_times.end(), 0L);
        stats.edges_per_sec = (total_edges * 1e6) / total_time; 

        stats.total_comm_mb = total_comm_bytes / (1024.0 * 1024.0);
    
        stats.PrintSummary();
    }
    

    //PR
    /*void run() {
        int iterationCount = 0;
        const int N = 5;
        const double d = 0.85;
        const double threshold = 1e-6;
    
        std::unordered_set<int> activeVertices;
        for (int i = 0; i < N; i++) activeVertices.insert(i);
    
        while (iterationCount < maxIterations && !activeVertices.empty()) {
            std::unordered_map<int, double> global_contributions;

            double hanging_contrib = 0.0;

            for (int src : activeVertices) {
                if (offset[src+1] - offset[src] == 0) { // 无出边
                    hanging_contrib += VertexProperty[src];
                }
            }

            double hanging_factor = d * hanging_contrib / N;
    
            for (int src : activeVertices) {
                GraphData currentData;
                currentData.srcid = src;
                currentData.activeVertices = activeVertices;
    
                readActiveVertex->process(currentData);
                readEdgeID->process(readActiveVertex->getOutputData());
                readEdge->process(readEdgeID->getOutputData());
                readDST->process(readEdge->getOutputData());
                processEdge->process(readDST->getOutputData());
                reduce->process(processEdge->getOutputData());
    
                for (const auto& [dst, contrib] : reduce->getOutputData().aggregated_contributions) {
                    global_contributions[dst] += contrib;
                }
            }
    
            std::unordered_set<int> newActiveVertices;
            for (int i = 0; i < N; i++) {
                double old_pr = VertexProperty[i];
                double sum_contrib = global_contributions[i] + hanging_factor;
                double new_pr = (1-d)/N + d * sum_contrib;
    
                if (std::abs(new_pr - old_pr) > threshold) {
                    newActiveVertices.insert(i);
                }
                VertexProperty[i] = new_pr;
            }
    
            activeVertices.swap(newActiveVertices);
            iterationCount++;
    
            std::cout << "Iteration " << iterationCount << " results:\n";
            for (int i = 0; i < N; i++) {
                std::cout << "Vertex " << i << ": " << VertexProperty[i] << "\n";
            }
        }
    }*/
    
struct PerformanceStats {

    std::vector<long> iteration_times; 
    std::unordered_map<std::string, long> module_times;

    std::vector<double> comm_mb_per_iter; //comm per iter
    double total_comm_mb = 0.0;           //total comm
    

    size_t peak_memory = 0;
    

    double edges_per_sec = 0.0;
    
    void PrintSummary() const {
        std::cout << "\n====== Performance Report ======\n";
        std::cout << "Total Iterations: " << iteration_times.size() << "\n";
        std::cout << "Avg Iteration Time: " 
                 << std::accumulate(iteration_times.begin(), iteration_times.end(), 0.0)/iteration_times.size() 
                 << " μs\n";
        //other
        std::cout << "\n====== Communication Metrics ======\n";
        std::cout << "Avg Comm/Iter: " 
                 << std::accumulate(comm_mb_per_iter.begin(), comm_mb_per_iter.end(), 0.0) 
                    / comm_mb_per_iter.size()
                 << " MB\n";
        std::cout << "Total Comm: " << total_comm_mb << " MB\n";

        long total_time = std::accumulate(iteration_times.begin(), iteration_times.end(), 0L);
        for (const auto& [name, time] : module_times) {
            double percent = (time * 100.0) / total_time;
            std::cout << name << " Time: " << time << " μs (" << percent << "%)\n";
        }
    }
};
    
    

    void printResults() {
    for (auto& module : modules) {
        GraphData outputData = module->getOutputData();

        std::cout << "Process at localClock: " << module->getLocalClock()
                  << ", Module output -> srcid: " << outputData.srcid
                  << ", Uprop: " << outputData.Uprop
                  << ", edgenum: " << outputData.edgenum
                  << ", dstid: ";

        for (size_t i = 0; i < outputData.dstid.size(); ++i) {
            std::cout << outputData.dstid[i];
            if (i != outputData.dstid.size() - 1) {
                std::cout << ", ";
            }
        }

        std::cout << ", weights: ";
        for (size_t i = 0; i < outputData.weight.size(); ++i) {
            std::cout << outputData.weight[i];
            if (i != outputData.weight.size() - 1) {
                std::cout << ", ";
            }
        }

        std::cout << ", Vprop: ";
        for (size_t i = 0; i < outputData.Vprop.size(); ++i) {
            std::cout << outputData.Vprop[i];
            if (i != outputData.Vprop.size() - 1) {
                std::cout << ", ";
            }
        }

        std::cout << ", Vtprop: ";
        for (size_t i = 0; i < outputData.Vtprop.size(); ++i) {
            std::cout << outputData.Vtprop[i];
            if (i != outputData.Vtprop.size() - 1) {
                std::cout << ", ";
            }
        }

        std::cout << ", res: ";
        for (size_t i = 0; i < outputData.res.size(); ++i) {
            std::cout << outputData.res[i];
            if (i != outputData.res.size() - 1) {
                std::cout << ", ";
            }
        }

        std::cout << ", VertexProperty: ";
        for (size_t i = 0; i < VertexProperty.size(); ++i) {
            std::cout << VertexProperty[i];
            if (i != VertexProperty.size() - 1) {
                std::cout << ", ";
            }
        }

        std::cout << ", Global Clock: " << globalClock << std::endl; 

        std::cout << std::endl;
    }
}
};

#endif
