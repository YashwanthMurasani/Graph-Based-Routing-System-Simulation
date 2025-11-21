#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "Node.hpp"
#include "Edge.hpp"

class Graph {
public:
    std::unordered_map<int, Node> nodes;          
    std::unordered_map<int, Edge> edges;           
    std::unordered_map<int, std::vector<std::pair<int, int>>> adj;
    std::unordered_map<int, Edge> history;        


    Graph() = default;

    void from_json(const nlohmann::json& filename);

    bool remove_edge(int edge_id);                         
    bool modify_edge(int edge_id, const nlohmann::json& patch); 

    const Node* get_node(int id) const;
    const Edge* get_edge(int id) const;
    const std::vector<std::pair<int, int>>& neighbors(int id) const;

    size_t num_nodes() const { return nodes.size(); }
    size_t num_edges() const { return edges.size(); }
    bool has_node(int id) const { return nodes.find(id) != nodes.end(); }
    bool has_edge(int id) const { return edges.find(id) != edges.end(); }
    const std::unordered_map<int, Node>& get_all_nodes() const { return nodes; }
    const std::unordered_map<int, Edge>& get_all_edges() const { return edges; }


    int nearest_node(double lat, double lon) const;
};

#endif 
