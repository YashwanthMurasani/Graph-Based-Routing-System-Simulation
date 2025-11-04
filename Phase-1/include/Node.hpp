#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include <vector>

struct Node {
    int id;
    double lat;
    double lon;
    std::vector<std::string> pois; // Points of Interest tags

    Node() = default;
    Node(int _id, double _lat, double _lon, const std::vector<std::string>& _pois = {})
        : id(_id), lat(_lat), lon(_lon), pois(_pois) {}
};

#endif // NODE_HPP