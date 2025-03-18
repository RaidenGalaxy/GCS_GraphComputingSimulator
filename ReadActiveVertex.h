#ifndef RVP_H
#define RVP_H

#include "BaseModule.h"
#include "MessageQueue.h"

//input: src id  output: src id, Uprop
class ReadActiveVertexPropertyModule : public BaseModule { // get active vertex's prop
public:

    const char* name() const override { return "ReadActiveVertex"; }

    void process(const GraphData& inputData) override {
        if (inputData.activeVertices.find(inputData.srcid) != inputData.activeVertices.end()) {
        int srcid = inputData.srcid;
        //int Uprop = VertexProperty[srcid];
        double Uprop = VertexProperty[srcid];
        outputData.srcid = inputData.srcid;
        outputData.Uprop = VertexProperty[inputData.srcid]; 
        outputData.activeVertices = inputData.activeVertices;

        outputData.total_processed_edges = inputData.total_processed_edges;

        advanceClock();
        }
    }

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        BaseModule::sendMessage(queue, timestamp, targetModule);
    }
};

#endif

/*#ifndef RVP_H
#define RVP_H

#include "BaseModule.h"
#include "MessageQueue.h"
#include "mem.h"
#include "def.h"

#include <unordered_map>
#include <mutex>
#include <iostream>

using namespace ramulator;

class ReadActiveVertexPropertyModule : public BaseModule {
private:
    MEM::single_mem* memory;
    std::unordered_map<int, int> buffer;
    mutable std::mutex buffer_mtx;

public:

    ReadActiveVertexPropertyModule() : memory(nullptr) {} 

    explicit ReadActiveVertexPropertyModule(MEM::single_mem& mem) : memory(&mem) {
        // global cb
        memory->register_global_callback([this](addr_type addr, int data) {
            int srcid = addr / sizeof(int);
            {
                std::lock_guard<std::mutex> lock(buffer_mtx);
                buffer[srcid] = data;
            }
            trigger_downstream(srcid);
        });
    }

    void process(const GraphData& inputData) override {
        int srcid = inputData.srcid;
    
        Request readReq;
        readReq.addr = srcid * sizeof(int);
        readReq.type = Request::Type::READ;
        readReq.coreid = 0;
    
        {
            std::lock_guard<std::mutex> lock(buffer_mtx);
            buffer[srcid] = -1; // wait to be processed
        }
    
        if (!memory->send(readReq)) {
            std::cerr << "Read request failed for srcid: " << srcid << std::endl;
        }
    
        advanceClock();
    }
    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        BaseModule::sendMessage(queue, timestamp, targetModule);
    }

private:
    void trigger_downstream(int srcid) {
        int Uprop = 0;
        {
            std::lock_guard<std::mutex> lock(buffer_mtx);
            Uprop = buffer[srcid];
        }

        outputData.srcid = srcid;
        outputData.Uprop = Uprop;
    }
};

#endif  // RVP_H
*/

/*#ifndef RVP_H
#define RVP_H

#include "BaseModule.h"
#include "MessageQueue.h"
#include "mem.h"
#include "def.h"

#include <unordered_map>
#include <mutex>
#include <iostream>
#include <memory>

using namespace ramulator;

class ReadActiveVertexPropertyModule : public BaseModule, public std::enable_shared_from_this<ReadActiveVertexPropertyModule> {
private:
    std::shared_ptr<MEM::single_mem> memory;
    std::unordered_map<int, int> buffer;
    mutable std::mutex buffer_mtx;
    bool callback_registered = false;

public:
    ReadActiveVertexPropertyModule() : memory(nullptr) {}

    explicit ReadActiveVertexPropertyModule(std::shared_ptr<MEM::single_mem> mem) : memory(mem) {
        if (!memory) {
            std::cerr << "Error: memory is not initialized!" << std::endl;
            return;
        }
    }

    void process(const GraphData& inputData) override {
        if (!memory) {
            std::cerr << "Error: memory is not initialized!" << std::endl;
            return;
        }

        int srcid = inputData.srcid;
        Request readReq;
        readReq.addr = srcid * sizeof(int);
        readReq.type = Request::Type::READ;
        readReq.coreid = 0;

        {
            std::lock_guard<std::mutex> lock(buffer_mtx);
            buffer[srcid] = -1;
        }

        if (!memory->send(readReq)) {
            std::cerr << "Read request failed for srcid: " << srcid << std::endl;
            return;
        }

        if (!callback_registered) {
            std::weak_ptr<ReadActiveVertexPropertyModule> weak_self = shared_from_this();
            memory->register_global_callback([weak_self](addr_type addr, int data) {
                if (auto self = weak_self.lock()) {
                    int srcid = addr / sizeof(int);
                    {
                        std::lock_guard<std::mutex> lock(self->buffer_mtx);
                        self->buffer[srcid] = data;
                    }
                    self->trigger_downstream(srcid);
                } else {
                    std::cerr << "Warning: ReadActiveVertexPropertyModule has been destroyed before callback execution." << std::endl;
                }
            });
            callback_registered = true;
        }

        advanceClock();
    }

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        BaseModule::sendMessage(queue, timestamp, targetModule);
    }

private:
    void trigger_downstream(int srcid) {
        int Uprop = 0;
        {
            std::lock_guard<std::mutex> lock(buffer_mtx);
            if (buffer.find(srcid) != buffer.end()) {
                Uprop = buffer[srcid];
            } else {
                std::cerr << "Warning: srcid " << srcid << " not found in buffer!" << std::endl;
                return;
            }
        }

        outputData.srcid = srcid;
        outputData.Uprop = Uprop;
    }
};

#endif
*/