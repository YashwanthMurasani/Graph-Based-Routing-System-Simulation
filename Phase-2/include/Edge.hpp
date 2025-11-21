#ifndef EDGE_HPP
#define EDGE_HPP

#include <string>
#include <vector>

struct Edge {
    int id;
    int u;                 //source 
    int v;                 //destination
    double length_m;       
    // double average_time_s; 
    // std::vector<double> speed_profile; 
    // bool oneway;           
    // std::string road_type; 
    // bool enabled;   

    Edge() = default;
    Edge(int _id, int _u, int _v, double _len): id(_id), u(_u), v(_v), length_m(_len){}
};

#endif 
