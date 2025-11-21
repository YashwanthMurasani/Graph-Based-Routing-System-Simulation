#include "QueryProcessor.hpp"
#include "Utils.hpp"
#include <iostream>
#include <unordered_set>

using json = nlohmann::json;

json QueryProcessor::process( const json& ev) {
    json out;
    out["id"]=ev["id"];
    std::string type = ev["type"];

    if (type == "remove_edge") {
        int eid = ev["edge_id"];
        bool ok = G.remove_edge(eid);
        out["done"] = ok;
        return out;
    }

    if (type == "modify_edge") {
        int eid = ev["edge_id"];
        json patch = ev.contains("patch") ? ev["patch"] : json::object();
        bool ok = G.modify_edge(eid, patch);
        out["done"] = ok;
        return out;
    }

    if (type == "shortest_path") {
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

        out["possible"] = res.possible;
        if (res.possible) {
            if (mode == "distance") out["minimum_distance"] = res.cost;
            else out["minimum_time"] = res.cost;
            out["path"] = res.path;
        }
        return out;
    }

    if (type == "knn") {
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

        out["nodes"] = nodes;
        return out;
    }

    out["error"] = "Unknown query type";
    return out;
}
