#include <iostream>
#include <string>
#include <chrono>
#include "Graph.hpp"
#include "QueryProcessor.hpp"
#include "Utils.hpp"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: ./phase1 <graph.json> <queries.json> <output.json>\n";
        return 1;
    }

    std::string graph_file = argv[1];
    std::string query_file = argv[2];
    std::string output_file = argv[3];

    try {
        // --------- Load graph ----------
        std::cout << "Loading graph...\n";
        Graph G = Graph::from_json(graph_file);

        // --------- Load queries ----------
        nlohmann::json query_json = Utils::load_json(query_file);
        if (!query_json.contains("events")) {
            std::cerr << "Error: queries.json must contain 'events' array.\n";
            return 1;
        }

        QueryProcessor processor(G);
        std::vector<nlohmann::json> outputs;

        // --------- Process each query ----------
        for (const auto& ev : query_json["events"]) {
            auto start = std::chrono::high_resolution_clock::now();

            nlohmann::json result = processor.process(ev);

            auto end = std::chrono::high_resolution_clock::now();
            double ms = std::chrono::duration<double, std::milli>(end - start).count();

            // As per spec: SampleDriver adds processing_time to output
            result["processing_time_ms"] = ms;

            outputs.push_back(result);
        }

        // --------- Write output ----------
        nlohmann::json out_json;
        out_json["results"] = outputs;
        Utils::save_json(output_file, out_json);

        std::cout << "✅ Done! Output written to " << output_file << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
