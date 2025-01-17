#include <iostream>
#include <vector>

class Message {
public:
    int timestamp;  // 消息的时间戳，用于排序和调度
    virtual ~Message() = default;
    virtual void print() const = 0;  // 输出消息内容
};

class ReadVertex_to_ReadEdgeID_Message : public Message { //src id, Uprop
public:
    int srcid;   //source id
    int Uprop;   //source property

    ReadVertex_to_ReadEdgeID_Message(int ts, const int uid, const int uprop) {
        timestamp = ts;
        srcid = uid;
        Uprop = uprop;
    }

    void print() const override {
        std::cout << "ReadVertex_to_ReadEdgeID_Message - Timestamp: " << timestamp << std::endl;
        std::cout << "Node IDs: ";
        for (int id : srcid) {
            std::cout << id << " ";
        }
        std::cout << "\nNode Values: ";
        for (int value : nodeValues) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
};

class ReadEdgeID_to_ReadEdge_Message : public Message {//srcid, Uprop, dstid[]
public:
    std::vector<int> sourceNodes;  // 存储源节点的ID数组
    std::vector<int> targetNodes;  // 存储目标节点的ID数组

    EdgeDataMessage(int ts, const std::vector<int>& src, const std::vector<int>& tgt) {
        timestamp = ts;
        sourceNodes = src;
        targetNodes = tgt;
    }

    void print() const override {
        std::cout << "EdgeDataMessage - Timestamp: " << timestamp << std::endl;
        std::cout << "Source Nodes: ";
        for (int src : sourceNodes) {
            std::cout << src << " ";
        }
        std::cout << "\nTarget Nodes: ";
        for (int tgt : targetNodes) {
            std::cout << tgt << " ";
        }
        std::cout << std::endl;
    }
};

// 用于权值数据的消息（包含多个边的权值数组）
class WeightDataMessage : public Message {
public:
    std::vector<int> edgeWeights;  // 存储每条边的权值数组

    WeightDataMessage(int ts, const std::vector<int>& weights) {
        timestamp = ts;
        edgeWeights = weights;
    }

    void print() const override {
        std::cout << "WeightDataMessage - Timestamp: " << timestamp << std::endl;
        std::cout << "Edge Weights: ";
        for (int weight : edgeWeights) {
            std::cout << weight << " ";
        }
        std::cout << std::endl;
    }
};

