#include <iostream>
#include <vector>
#include <queue>
#include <functional>

// 消息结构体
struct Message {
    int timestamp;
    void* targetModule;
    std::vector<int> data;   // carry data

    bool operator<(const Message& other) const {
        return timestamp > other.timestamp;
    }
};

// 消息队列类
class MessageQueue {
private:
    std::priority_queue<Message> queue;

public:
    // 向消息队列添加消息
    void addMessage(int timestamp, void* targetModule, const std::vector<int>& data) {
        queue.push({timestamp, targetModule, data});
    }

    // 从消息队列中取出消息
    Message popMessage() {
        if (!queue.empty()) {
            Message msg = queue.top();
            queue.pop();
            return msg;
        }
        return Message{-1, nullptr, {}};  // 返回一个空的消息
    }

    bool isEmpty() const {
        return queue.empty();
    }
};

// 图数据结构，包含节点和边的相关信息
struct GraphData {
    int nodeID;
    int nodeValue;   // 节点值
    int edgenum;     // 边的数量
    std::vector<int> targetNodeIDs;  // 边的目标节点ID
    std::vector<int> edgeWeights;    // 边权重
};

// 基础模块类
class BaseModule {
protected:
    std::vector<int> inputData;   // 输入数据
    std::vector<int> outputData;  // 输出数据

public:
    virtual ~BaseModule() {}

    virtual void receiveData(const std::vector<int>& data) {
        inputData = data;
    }

    virtual void process() = 0;

    virtual void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) = 0;

    std::vector<int> getOutputData() const {
        return outputData;
    }
};

// 读入激活节点模块
class ReadActiveNodeModule : public BaseModule {
public:
    void process() override {
        // 处理数据：假设inputData为节点ID和节点值
        if (inputData.size() == 2) {
            int nodeID = inputData[0];
            int nodeValue = inputData[1];

            // 输出节点信息
            std::cout << "Read Active Node: ID = " << nodeID << ", Value = " << nodeValue << std::endl;

            // 假设把节点ID存入输出数据
            outputData.push_back(nodeID);
        }
    }

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        Message msg = {timestamp, targetModule, outputData};  // 发送的是outputData
        queue.addMessage(timestamp, targetModule, outputData);
    }
};

// 读入边模块
class ReadEdgeModule : public BaseModule {
public:
    void process() override {
        // 处理数据：假设inputData为目标节点ID和边权重
        if (inputData.size() == 2) {
            int targetNodeID = inputData[0];
            int edgeWeight = inputData[1];

            // 输出边信息
            std::cout << "Read Edge: Target Node ID = " << targetNodeID << ", Weight = " << edgeWeight << std::endl;

            // 将目标节点ID和边权重存入输出数据
            outputData.push_back(targetNodeID);
            outputData.push_back(edgeWeight);
        }
    }

    void sendMessage(MessageQueue& queue, int timestamp, void* targetModule) override {
        Message msg = {timestamp, targetModule, outputData};  // 发送的是outputData
        queue.addMessage(timestamp, targetModule, outputData);
    }
};

// 测试用例
int main() {
    // 创建消息队列
    MessageQueue messageQueue;

    // 创建两个模块
    ReadActiveNodeModule readActiveNodeModule;
    ReadEdgeModule readEdgeModule;

    // 测试数据：假设激活节点为节点ID=1，节点值=10，边数据目标节点ID=2，边权重=5
    GraphData activeNodeData = {1, 10, 1, {2}, {5}};
    GraphData edgeData = {2, 20, 1, {3}, {10}};

    // 给模块输入数据：读入激活节点
    readActiveNodeModule.receiveData({activeNodeData.nodeID, activeNodeData.nodeValue});

    // 处理数据
    readActiveNodeModule.process();

    // 发送消息给下游模块
    readActiveNodeModule.sendMessage(messageQueue, 1, &readEdgeModule);

    // 给模块输入数据：读入边
    readEdgeModule.receiveData({edgeData.targetNodeIDs[0], edgeData.edgeWeights[0]});

    // 处理数据
    readEdgeModule.process();

    // 输出结果
    std::vector<int> activeNodeOutput = readActiveNodeModule.getOutputData();
    std::vector<int> edgeOutput = readEdgeModule.getOutputData();

    std::cout << "Active Node Output: ";
    for (int val : activeNodeOutput) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "Edge Output: ";
    for (int val : edgeOutput) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // 从消息队列中取出消息并输出
    Message msg = messageQueue.popMessage();
    if (msg.targetModule != nullptr) {
        std::cout << "Message sent to target module at timestamp: " << msg.timestamp << std::endl;
    }

    return 0;
}
