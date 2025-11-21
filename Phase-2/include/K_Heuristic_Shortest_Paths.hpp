#ifndef KNN_HPP
#define KNN_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <cmath>
#include <limits>
#include "Graph.hpp"
#include "ShortestPath.hpp"
#include "Utils.hpp"

class KHSP {
private:
    const Graph& G;
    int src;
    int dest;
    int k;
    std::map<int,double>dist;
    std::map<int,int>par;

public:
    std::pair<double, std::vector<int>> dijkstra_path(const Graph& G, int src, int dst);
    std::vector<std::pair<double,std::vector<int>>> yen_k_shortest_paths(Graph& G, int src, int dst, int K);

    KHSP(const Graph& graph, int _src,int _dest, int _k) : G(graph),src(_src),dest(_dest),k(_k){};

};

#endif
