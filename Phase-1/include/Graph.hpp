#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "Node.hpp"
#include "Edge.hpp"

class Graph {
private:
    std::unordered_map<int, Node> nodes;           // id -> Node
    std::unordered_map<int, Edge> edges;           // id -> Edge
    std::unordered_map<int, std::vector<std::pair<int, int>>> adj; // node_id -> [(neighbor, edge_id)]
    std::unordered_map<int, Edge> history;         // for restoring removed edges

public:
    Graph() = default;

    // ---------- Construction ----------
    void from_json(const std::string& filename);

    // ---------- Updates ----------
    bool remove_edge(int edge_id);                           // disable
    bool modify_edge(int edge_id, const nlohmann::json& patch); // modify or restore

    // ---------- Access ----------
    const Node* get_node(int id) const;
    const Edge* get_edge(int id) const;
    const std::vector<std::pair<int, int>>& neighbors(int id) const;

    // ---------- Utilities ----------
    size_t num_nodes() const { return nodes.size(); }
    size_t num_edges() const { return edges.size(); }
    bool has_node(int id) const { return nodes.find(id) != nodes.end(); }
    bool has_edge(int id) const { return edges.find(id) != edges.end(); }
        // ---------- Iterators for read-only access ----------
    const std::unordered_map<int, Node>& get_all_nodes() const { return nodes; }
    const std::unordered_map<int, Edge>& get_all_edges() const { return edges; }


    // ---------- Nearest Node Helper ----------
    int nearest_node(double lat, double lon) const; // Euclidean nearest neighbor
};

#endif // GRAPH_HPP
