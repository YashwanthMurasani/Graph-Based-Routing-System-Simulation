#include "Graph.hpp"
#include "Utils.hpp"
#include <fstream>
#include <cmath>
#include <iostream>

void Graph::from_json(const std::string& filename) {
    nlohmann::json j = Utils::load_json(filename);

    // Load nodes
    for (const auto& node : j["nodes"]) {
        int id = node["id"];
        double lat = node["lat"];
        double lon = node["lon"];
        std::vector<std::string> pois;
        if (node.contains("pois")) {
            pois = node["pois"].get<std::vector<std::string>>();
        }
        nodes[id] = Node(id, lat, lon, pois);
    }

    // Load edges
    for (const auto& edge : j["edges"]) {
        int id = edge["id"];
        int u = edge["u"];
        int v = edge["v"];
        double len = edge["length"];
        double avg_t = edge["average_time"];
        bool oneway = edge["oneway"];
        std::string type = edge["road_type"];

        std::vector<double> sp;
        if (edge.contains("speed_profile"))
            sp = edge["speed_profile"].get<std::vector<double>>();

        edges[id] = Edge(id, u, v, len, avg_t, sp, oneway, type);

        // Add adjacency (u → v)
        adj[u].push_back({v, id});
        if (!oneway) {
            adj[v].push_back({u, id});
        }
    }
    return ;
}

bool Graph::remove_edge(int edge_id) {
    auto it = edges.find(edge_id);
    if (it == edges.end()) return false;

    Edge& e = it->second;
    e.enabled = false;
    history[edge_id] = e; // store backup

    return true;
}


bool Graph::modify_edge(int edge_id, const nlohmann::json& patch) {
    // If edge exists
    if (edges.find(edge_id) != edges.end()) {
        Edge& e = edges[edge_id];

        // If it was disabled, and patch empty, restore previous
        if (!e.enabled && patch.empty()) {
            if (history.find(edge_id) != history.end()) {
                e = history[edge_id];
                e.enabled = true;
                return true;
            }
            return false;
        }

        // Apply patch updates
        if (patch.contains("length"))
            e.length_m = patch["length"];
        if (patch.contains("average_time"))
            e.average_time_s = patch["average_time"];
        if (patch.contains("speed_profile"))
            e.speed_profile = patch["speed_profile"].get<std::vector<double>>();
        if (patch.contains("road_type"))
            e.road_type = patch["road_type"];
        if (patch.contains("oneway"))
            e.oneway = patch["oneway"];

        e.enabled = true;
        return true;
    }

    if (history.find(edge_id) != history.end()) {
        Edge e = history[edge_id];
        if (!patch.empty()) {
            if (patch.contains("length"))
                e.length_m = patch["length"];
            if (patch.contains("average_time"))
                e.average_time_s = patch["average_time"];
            if (patch.contains("speed_profile"))
                e.speed_profile = patch["speed_profile"].get<std::vector<double>>();
            if (patch.contains("road_type"))
                e.road_type = patch["road_type"];
        }
        e.enabled = true;
        edges[edge_id] = e;

        // restore adjacency
        adj[e.u].push_back({e.v, e.id});
        if (!e.oneway) adj[e.v].push_back({e.u, e.id});
        return true;
    }

    return false;
}

// --------------------- Access Helpers ---------------------

const Node* Graph::get_node(int id) const {
    auto it = nodes.find(id);
    return (it != nodes.end()) ? &it->second : nullptr;
}

const Edge* Graph::get_edge(int id) const {
    auto it = edges.find(id);
    return (it != edges.end()) ? &it->second : nullptr;
}

const std::vector<std::pair<int, int>>& Graph::neighbors(int id) const {
    static const std::vector<std::pair<int, int>> empty;
    auto it = adj.find(id);
    return (it != adj.end()) ? it->second : empty;
}

// --------------------- Nearest Node by Euclidean Distance ---------------------

int Graph::nearest_node(double lat, double lon) const {
    double best_dist = std::numeric_limits<double>::max();
    int best_id = -1;

    for (const auto& [id, node] : nodes) {
        double d = std::hypot(node.lat - lat, node.lon - lon);
        if (d < best_dist) {
            best_dist = d;
            best_id = id;
        }
    }
    return best_id;
}
