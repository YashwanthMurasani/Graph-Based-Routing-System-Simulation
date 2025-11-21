#include "QueryProcessor.hpp"
#include "Utils.hpp"
#include <iostream>
#include <unordered_set>

using json = nlohmann::json;

json QueryProcessor::process( const json& ev) {
    json out;
    out["id"]=ev["id"];
    std::string type = ev["type"];

    if (type =="k_shortest_paths"){
        int src= ev["source"];
        int dest=ev["target"];
        int k=ev["k"];
        std::string mode=ev["mode"];
        

        return out;
    }
    if (type =="k_shortest_paths_heuristic"){
        int src= ev["source"];
        int dest=ev["target"];
        int k=ev["k"];
        int t=ev["overlap_threshold"];
        
        
        return out;
    }
    if(type =="approx_shortest_path"){
        int budget=ev["time_budget_ms"];
        int err_acc=ev["acceptable_error_pct"];

        for(auto i:ev["queries"]){
            int src=i["source"];
            int tar=i["target"];


        }
        return out;
    }

    out["error"] = "Unknown query type";
    return out;
}
