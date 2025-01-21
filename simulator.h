#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "BaseModule.h"
#include "TimeManager.h"
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

class Simulator : public TimeManager {
public:

    std::vector<std::shared_ptr<BaseModule>> modules;
    MessageQueue messageQueue;

    int globalClock = 0;
    int maxIterations = 1000;

    //Simulator() = default;

    void addModule(std::shared_ptr<BaseModule> module) {
        module->setTimeManager(this);
        modules.push_back(module);
    }

    void incrementGlobalClock() override {
        globalClock++;
    }

    int getGlobalClock() const override {
        return globalClock;
    }

    Simulator() 
        : messageQueue([this]() { return this->globalClock; }) {}

    void run() {
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

            targetModule->sendMessage(messageQueue, globalClock++, targetModule);

            incrementGlobalClock();
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

        std::cout << ", Global Clock: " << globalClock << std::endl; 

        std::cout << std::endl;
    }
}
};

#endif
