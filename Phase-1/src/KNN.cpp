#include "KNN.hpp"
#include <unordered_set>
#include <queue>
#include <cmath>
#include <iostream>

// --------------------- Euclidean KNN ---------------------

std::vector<int> KNN::knn_euclidean(const std::string& poi, double lat, double lon, int k) const {
    std::priority_queue<NodeDist> pq;

    for (const auto& [id, node] : G.get_all_nodes()) {
        bool has_poi = false;
        for (const auto& p : node.pois) {
            if (strcasecmp(p.c_str(), poi.c_str()) == 0) {
                has_poi = true;
                break;
            }
        }
        if (!has_poi) continue;

        double d = euclidean(lat, lon, node.lat, node.lon);
        pq.push({id, d});
        if ((int)pq.size() > k) pq.pop();
    }

    std::vector<int> result;
    while (!pq.empty()) {
        result.push_back(pq.top().id);
        pq.pop();
    }
    std::reverse(result.begin(), result.end());
    return result;
}

// --------------------- Shortest-Path KNN ---------------------

std::vector<int> KNN::knn_shortest_path(const std::string& poi, double lat, double lon, int k) {
    int src = G.nearest_node(lat, lon);
    if (src == -1) return {};

    // Dijkstra from source
    using PDI = std::pair<double, int>; // (dist, node)
    std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI>> pq;
    std::unordered_map<int, double> dist;

    pq.push({0.0, src});
    dist[src] = 0.0;

    std::vector<int> found;

    while (!pq.empty() && (int)found.size() < k) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;

        const Node* node = G.get_node(u);
        if (!node) continue;

        for (const auto& tag : node->pois) {
            if (strcasecmp(tag.c_str(), poi.c_str()) == 0) {
                found.push_back(u);
                break;
            }
        }
        if ((int)found.size() >= k) break;

        for (auto [v, eid] : G.neighbors(u)) {
            const Edge* e = G.get_edge(eid);
            if (!e || !e->enabled) continue;

            double nd = d + e->length_m;
            if (!dist.count(v) || nd < dist[v]) {
                dist[v] = nd;
                pq.push({nd, v});
            }
        }
    }

    return found;
}
