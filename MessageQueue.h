#ifndef MSGQ_H
#define MSGQ_H

#include <iostream>
#include <vector>
#include <queue>
#include <functional>

struct GraphData {
    int srcid;        //source id
    int Uprop;        //source property
    int edgenum;      //edge number of source

    std::vector<int> dstid;       // destination id
    std::vector<int> weight;      // edge weight
    std::vector<int> Vprop;       // destination property
    std::vector<int> Vtprop;      // destination temporary property
    std::vector<int> res;         // min(vprop, vtprop)

    GraphData()
        : srcid(0), Uprop(0), edgenum(0),
          dstid(), weight(), Vprop(), Vtprop(), res() {} //default construction

    bool operator==(const GraphData& other) const {
        return srcid == other.srcid &&
               Uprop == other.Uprop &&
               edgenum == other.edgenum &&
               dstid == other.dstid &&
               weight == other.weight &&
               Vprop == other.Vprop &&
               Vtprop == other.Vtprop &&
               res == other.res;
    }

    bool operator!=(const GraphData& other) const {
        return !(*this == other);
    }
};

class MessageQueue {
public:
    struct Message {
        int timestamp;
        void* targetModule;
        GraphData data;

        bool operator<(const Message& other) const {
            return timestamp > other.timestamp;
        }
    };

private:
    std::priority_queue<Message> queue;

public:

    void addMessage(int timestamp, void* targetModule, const GraphData& data) {
        queue.push({timestamp, targetModule, data});
    }

    Message popMessage() {
        Message msg = queue.top();
        queue.pop();
        return msg;
    }

    bool isEmpty() const {
        return queue.empty();
    }
};

#endif