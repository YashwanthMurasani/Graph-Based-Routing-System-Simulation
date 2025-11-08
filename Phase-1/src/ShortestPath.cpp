#include "ShortestPath.hpp"
#include <queue>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <iostream>

double ShortestPathSolver::travel_time_on_edge(const Edge& e, double t_start) const {
    if (e.speed_profile.empty())
        return e.average_time_s;

    const int SLOT_SECS = 900; 
    double remaining = e.length_m;
    double t_current = t_start;

    for (int iter = 0; iter < 1000 && remaining > 1e-6; ++iter) {
        int slot = int(t_current / SLOT_SECS)%96;
        double v = e.speed_profile[slot];
        if (v <= 1e-9) continue; 
        double slot_end = (slot + 1) * SLOT_SECS;
        double time_left = slot_end - t_current;
        double dist_possible = v * time_left;

        if (dist_possible >= remaining) {
            double time_needed = remaining / v;
            t_current += time_needed;
            remaining = 0;
        } else {
            t_current += time_left;
            remaining -= dist_possible;
        }
    }

    return t_current - t_start;
}

std::vector<int> ShortestPathSolver::reconstruct_path(
    int src, int tgt, const std::unordered_map<int, int>& parent) const
{
    std::vector<int> path;
    int cur = tgt;
    while (cur != src && parent.count(cur)) {
        path.push_back(cur);
        cur = parent.at(cur);
    }
    if (cur == src) path.push_back(src);
    std::reverse(path.begin(), path.end());
    return path;
}

PathResult ShortestPathSolver::shortest_path_distance(
    int src, int tgt, const Constraints& cons)
{
    PathResult res;
    if (!G.has_node(src) || !G.has_node(tgt)) return res;
    if (cons.forbidden_nodes.count(src) || cons.forbidden_nodes.count(tgt)) return res;

    using PDI = std::pair<double, int>;
    std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI>> pq;
    std::unordered_map<int, double> dist;
    std::unordered_map<int, int> parent;

    pq.push({0.0, src});
    dist[src] = 0.0;

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;
        if (u == tgt) break;

        for (auto [v, eid] : G.neighbors(u)) {
            const Edge* e = G.get_edge(eid);
            if (!e || !e->enabled) continue;
            if (cons.forbidden_nodes.count(v)) continue;
            if (cons.forbidden_road_types.count(e->road_type)) continue;

            double nd = d + e->length_m;
            if (!dist.count(v) || nd < dist[v]) {
                dist[v] = nd;
                parent[v] = u;
                pq.push({nd, v});
            }
        }
    }

    if (!dist.count(tgt)) return res;

    res.possible = true;
    res.cost = dist[tgt];
    res.path = reconstruct_path(src, tgt, parent);
    return res;
}

PathResult ShortestPathSolver::shortest_path_time(
    int src, int tgt, const Constraints& cons, double start_time)
{
    PathResult res;
    if (!G.has_node(src) || !G.has_node(tgt)) return res;
    if (cons.forbidden_nodes.count(src) || cons.forbidden_nodes.count(tgt)) return res;

    using PDI = std::pair<double, int>;
    std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI>> pq;
    std::unordered_map<int, double> arr;
    std::unordered_map<int, int> parent;

    pq.push({start_time, src});
    arr[src] = start_time;

    while (!pq.empty()) {
        auto [t_cur, u] = pq.top();
        pq.pop();
        if (t_cur > arr[u]) continue;
        if (u == tgt) break;

        for (auto [v, eid] : G.neighbors(u)) {
            const Edge* e = G.get_edge(eid);
            if (!e || !e->enabled) continue;
            if (cons.forbidden_nodes.count(v)) continue;
            if (cons.forbidden_road_types.count(e->road_type)) continue;

            double travel = travel_time_on_edge(*e, t_cur);
            double t_new = t_cur + travel;
            if (!arr.count(v) || t_new < arr[v]) {
                arr[v] = t_new;
                parent[v] = u;
                pq.push({t_new, v});
            }
        }
    }

    if (!arr.count(tgt)) return res;

    res.possible = true;
    res.cost = arr[tgt] - start_time;
    res.path = reconstruct_path(src, tgt, parent);
    return res;
}
