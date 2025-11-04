#include "QueryProcessor.hpp"
#include "Utils.hpp"
#include <iostream>
#include <unordered_set>

// --------------------- Process Query/Event ---------------------

nlohmann::json QueryProcessor::process(const nlohmann::json& ev) {
    nlohmann::json out;

    std::string type = ev["type"];

    // ---------- 1. REMOVE EDGE ----------
    if (type == "remove_edge") {
        int eid = ev["edge_id"];
        bool ok = G.remove_edge(eid);
        out["done"] = ok;
        return out;
    }

    // ---------- 2. MODIFY EDGE ----------
    if (type == "modify_edge") {
        int eid = ev["edge_id"];
        nlohmann::json patch = ev.contains("patch") ? ev["patch"] : nlohmann::json::object();
        bool ok = G.modify_edge(eid, patch);
        out["done"] = ok;
        return out;
    }

    // ---------- 3. SHORTEST PATH ----------
    if (type == "shortest_path") {
        int qid = ev["id"];
        int src = ev["source"];
        int tgt = ev["target"];
        std::string mode = ev["mode"];

        Constraints cons;
        if (ev.contains("constraints")) {
            auto c = ev["constraints"];
            if (c.contains("forbidden_nodes")) {
                for (int n : c["forbidden_nodes"])
                    cons.forbidden_nodes.insert(n);
            }
            if (c.contains("forbidden_road_types")) {
                for (const auto& s : c["forbidden_road_types"])
                    cons.forbidden_road_types.insert(s);
            }
        }

        PathResult res;
        if (mode == "distance")
            res = sp_solver.shortest_path_distance(src, tgt, cons);
        else
            res = sp_solver.shortest_path_time(src, tgt, cons, 0.0);

        out["id"] = qid;
        out["possible"] = res.possible;
        if (res.possible) {
            if (mode == "distance") out["minimum_distance"] = res.cost;
            else out["minimum_time"] = res.cost;
            out["path"] = res.path;
        }
        return out;
    }

    // ---------- 4. KNN QUERY ----------
    if (type == "knn") {
        int qid = ev["id"];
        std::string poi = ev["poi"];
        double lat = ev["query_point"]["lat"];
        double lon = ev["query_point"]["lon"];
        int k = ev["k"];
        std::string metric = ev["metric"];

        std::vector<int> nodes;
        if (metric == "euclidean")
            nodes = knn_solver.knn_euclidean(poi, lat, lon, k);
        else
            nodes = knn_solver.knn_shortest_path(poi, lat, lon, k);

        out["id"] = qid;
        out["nodes"] = nodes;
        return out;
    }

    // ---------- Unknown type ----------
    out["error"] = "Unknown query type";
    return out;
}
