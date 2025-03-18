#ifndef GRAPH_LOADER_H
#define GRAPH_LOADER_H

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

class GraphLoader {
public:
    //load ofs
    static void LoadOffset(const std::string& filename, std::vector<int>& offset) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open offset file: " + filename);
        }
        
        int value;
        while (file >> value) {
            offset.push_back(value);
        }
    }

    //load edge
    static void LoadEdge(const std::string& filename, 
                        std::vector<int>& dst_nodes) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open edge file: " + filename);
        }

        int dst;
        while (file >> dst) {
            dst_nodes.push_back(dst);
        }
    }

    //load weight

    static void LoadWeight(const std::string& filename, std::vector<double>& weights) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open weight file: " + filename);
        }
        
        double weight;
        while (file >> weight) {
            weights.push_back(weight);
        }
    }

    // initial VP
    template <typename T>
    static void InitVertexProperty(std::vector<T>& vertex_prop, 
                                  int num_vertices,
                                  const std::string& algo_type) {
        vertex_prop.resize(num_vertices);
        
        if (algo_type == "PR") {
            T init_val = 1.0 / num_vertices;
            std::fill(vertex_prop.begin(), vertex_prop.end(), init_val);
        } else if (algo_type == "SSSP") {
            std::fill(vertex_prop.begin(), vertex_prop.end(), MAX);
            vertex_prop[0] = 0; // 假设源节点是0
        } else if (algo_type == "SSWP") {
            std::fill(vertex_prop.begin(), vertex_prop.end(), MIN);
            vertex_prop[0] = MAX; // 示例初始化
        }
    }
};

#endif