#ifndef QUERY_PROCESSOR_HPP
#define QUERY_PROCESSOR_HPP

#include <nlohmann/json.hpp>
#include <string>
#include "Graph.hpp"
#include "ShortestPath.hpp"
#include "KNN.hpp"

class QueryProcessor {
private:
    Graph& G;
    ShortestPathSolver sp_solver;
    KNN knn_solver;

public:
    explicit QueryProcessor(Graph& graph)
        : G(graph), sp_solver(graph), knn_solver(graph) {}

    nlohmann::json process(const nlohmann::json& ev);
};

#endif 
