#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "BaseModule.h"
#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <memory>
#include "ReadActiveVertex.h"
#include "ReadEdgeID.h"
#include "ReadEdges.h"
#include "ReadDSTProp.h"
#include "ProcessEdge.h"
#include "Reduce.h"
#include "WriteDSTProp.h"
#include "MessageQueue.h" 

class Simulator {
public:
    std::vector<std::shared_ptr<BaseModule>> modules;
    MessageQueue messageQueue;
    int currentTimestamp = 0;

    Simulator() = default;

    void addModule(std::shared_ptr<BaseModule> module) {
        modules.push_back(module);
    }

    void run() {
    int maxIterations = 10000;
    int iterationCount = 0;

    while (!messageQueue.isEmpty()) {
        if (iterationCount++ > maxIterations) {
            std::cerr << "Maximum iterations reached. Terminating to prevent infinite loop." << std::endl;
            break;
        }

        MessageQueue::Message msg = messageQueue.popMessage();

        BaseModule* targetModule = static_cast<BaseModule*>(msg.targetModule);
        GraphData inputData = msg.data;

        targetModule->process(inputData);

        targetModule->sendMessage(messageQueue, currentTimestamp++, targetModule);
    }
}

    void printResults() {
    for (auto& module : modules) {
        GraphData outputData = module->getOutputData();

        std::cout << "Module output -> srcid: " << outputData.srcid
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

        std::cout << std::endl;
    }
}

};

/*
class Simulator {
private:
    int globalClock = 0;
    static Simulator* instance;  // Singleton instance

public:
    std::vector<std::shared_ptr<BaseModule>> modules;
    MessageQueue messageQueue;
    int maxIterations = 10000;  // Maximum number of iterations
    int iterationCount = 0;

    Simulator() = default;

    // Ensure only one instance of Simulator exists (Singleton pattern)
    static Simulator* getInstance() {
        if (instance == nullptr) {
            instance = new Simulator();
        }
        return instance;
    }

    // Method to add modules
    void addModule(std::shared_ptr<BaseModule> module) {
        modules.push_back(module);
    }

    // Global clock getter and incrementer
    int getGlobalClock() const {
        return globalClock;
    }

    void incrementGlobalClock() {
        globalClock++;
    }

    // Run the simulation
    void run() {
        while (!messageQueue.isEmpty()) {
            if (iterationCount++ > maxIterations) {
                std::cerr << "Maximum iterations reached. Terminating to prevent infinite loop." << std::endl;
                break;
            }

            // Increment the global clock
            incrementGlobalClock();

            // Process all modules for the current global clock cycle
            for (auto& module : modules) {
                if (module->getModuleClock() <= globalClock) {
                    // Get data from message queue or previous module outputs
                    GraphData inputData = messageQueue.popMessage().data;  
                    module->process(inputData);  // Process data from the message queue
                    module->incrementModuleClock();  // Increment the module's internal clock
                }
            }

            // Perform message sending for modules that have processed data
            for (auto& module : modules) {
                module->sendMessage(messageQueue, globalClock, module.get());
            }
        }
    }

    // Print simulation results
    void printResults() {
        for (auto& module : modules) {
            GraphData outputData = module->getOutputData();
            std::cout << "Module output -> srcid: " << outputData.srcid
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

            std::cout << std::endl;
        }
    }
};*/


#endif

