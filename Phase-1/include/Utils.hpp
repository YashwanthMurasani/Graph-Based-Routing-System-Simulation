#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

// Utility namespace for file and JSON helpers
namespace Utils {

    // Load JSON from file
    inline nlohmann::json load_json(const std::string& filename) {
        std::ifstream in(filename);
        if (!in.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
        nlohmann::json j;
        in >> j;
        return j;
    }

    // Save JSON to file
    inline void save_json(const std::string& filename, const nlohmann::json& j) {
        std::ofstream out(filename);
        if (!out.is_open()) {
            throw std::runtime_error("Cannot open file for writing: " + filename);
        }
        out << j.dump(4);
    }

    // Simple Euclidean distance utility (used in KNN and Graph)
    inline double euclidean(double lat1, double lon1, double lat2, double lon2) {
        double dx = lat1 - lat2;
        double dy = lon1 - lon2;
        return std::sqrt(dx * dx + dy * dy);
    }

}

#endif // UTILS_HPP
