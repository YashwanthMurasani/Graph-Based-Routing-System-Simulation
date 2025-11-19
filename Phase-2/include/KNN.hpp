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

class KNN {
private:
    const Graph& G;
    ShortestPathSolver sp_solver;

    struct NodeDist {
        int id;
        double dist;
        bool operator<(const NodeDist& other) const {
            return dist > other.dist; 
        }
    };

    static double euclidean(double lat1, double lon1, double lat2, double lon2) {
        double dx = lat1 - lat2;
        double dy = lon1 - lon2;
        return std::sqrt(dx * dx + dy * dy);
    }

public:
    explicit KNN(const Graph& graph) : G(graph), sp_solver(graph) {}

    std::vector<int> knn_euclidean(const std::string& poi, double lat, double lon, int k) const;

    std::vector<int> knn_shortest_path(const std::string& poi, double lat, double lon, int k);
};

#endif
