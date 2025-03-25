#ifndef MSGQ_H
#define MSGQ_H

#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <unordered_set>
#include <unordered_map>

struct GraphData {
    int srcid;
    //SSSP&SSWP
    int Uprop;
    //PR
    //double Uprop;
    int edgenum;

    std::vector<int> dstid;
    std::vector<int> weight;
    std::vector<int> Vprop;
    std::vector<int> Vtprop;
    std::vector<int> res;

    std::vector<double> contributions;
    std::unordered_map<int, double> aggregated_contributions;

    std::unordered_set<int> activeVertices;

    size_t total_processed_edges = 0;

    size_t CalculateSize() const {
        size_t size = 0;
        size += sizeof(srcid);
        size += sizeof(Uprop);
        size += sizeof(edgenum);
        size += dstid.size() * sizeof(int);
        size += weight.size() * sizeof(decltype(weight)::value_type);
        size += Vprop.size() * sizeof(decltype(Vprop)::value_type);
        size += Vtprop.size() * sizeof(decltype(Vtprop)::value_type);
        size += res.size() * sizeof(decltype(res)::value_type);
        return size;
    }

    GraphData()
        : srcid(0), Uprop(0), edgenum(0),
          dstid(), weight(), Vprop(), Vtprop(), res(), contributions(), aggregated_contributions(), activeVertices(), total_processed_edges(0){}

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