#ifndef EDGE_HPP
#define EDGE_HPP

#include <string>
#include <vector>

struct Edge {
    int id;
    int u;                 // source node id
    int v;                 // destination node id
    double length_m;       // in meters
    double average_time_s; // in seconds
    std::vector<double> speed_profile; // 96 × 15-min slot speeds (m/s)
    bool oneway;           // true = u->v only
    std::string road_type; // primary, secondary, etc.
    bool enabled;          // false when edge removed

    Edge() = default;
    Edge(int _id, int _u, int _v, double _len, double _avg_t, 
         const std::vector<double>& _profile, bool _oneway,
         const std::string& _type)
        : id(_id), u(_u), v(_v), length_m(_len), average_time_s(_avg_t),
          speed_profile(_profile), oneway(_oneway), road_type(_type), enabled(true) {}
};

#endif // EDGE_HPP
