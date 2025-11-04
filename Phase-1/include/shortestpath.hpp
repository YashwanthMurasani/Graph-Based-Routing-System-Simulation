#ifndef SHORTEST_PATH_HPP
#define SHORTEST_PATH_HPP

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <limits>
#include <cmath>
#include <string>
#include "Graph.hpp"

struct Constraints {
    std::unordered_set<int> forbidden_nodes;
    std::unordered_set<std::string> forbidden_road_types;
};

struct PathResult {
    bool possible = false;
    double cost = 0.0;              // distance (m) or time (s)
    std::vector<int> path;
};

// ------------------- Shortest Path Solver -------------------

class ShortestPathSolver {
private:
    const Graph& G;

    // Internal helper to compute time-dependent edge travel time
    double travel_time_on_edge(const Edge& e, double t_start) const;

    // Helper to reconstruct the path from parent map
    std::vector<int> reconstruct_path(
        int src, int tgt, const std::unordered_map<int, int>& parent) const;

public:
    explicit ShortestPathSolver(const Graph& graph) : G(graph) {}

    PathResult shortest_path_distance(
        int src, int tgt, const Constraints& cons = {});

    PathResult shortest_path_time(
        int src, int tgt, const Constraints& cons = {}, double start_time = 0.0);
};

#endif // SHORTEST_PATH_HPP
