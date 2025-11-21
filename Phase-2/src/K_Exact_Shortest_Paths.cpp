std::pair<double, std::vector<int>> KESP::dijkstra_path(const Graph& G, int src, int dst){
        const double INF = std::numeric_limits<double>::infinity();

        std::unordered_map<int,double> dist;
        std::unordered_map<int,int> parent;

        for (auto& p : G.nodes)
            dist[p.first] = INF;

        std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d > dist[u]) continue;
            if (u == dst) break;

            for (auto& [nbr, eid] : G.adj.at(u)) {
                const Edge& e = G.edges.at(eid);
                double nd = d + e.length_m;

                if (nd < dist[nbr]) {
                    dist[nbr] = nd;
                    parent[nbr] = u;
                    pq.push({nd, nbr});
                }
            }
        }

        if (dist[dst] == INF)
            return {INF, {}};

        // reconstruct path
        std::vector<int> path;
        for (int cur = dst; cur != src; cur = parent[cur])
            path.push_back(cur);
        path.push_back(src);
        std::reverse(path.begin(), path.end());
        return {dist[dst], path};
    }


struct State {
    double d;
    int node;
    bool operator>(const State& o) const { return d > o.d; }
};

std::vector<std::pair<double,std::vector<int>>> KESP::yen_k_shortest_paths(Graph& G, int src, int dst, int K){
        std::vector<std::pair<double,std::vector<int>>> A;  
        using Candidate = std::pair<double, std::vector<int>>;
        std::set<Candidate> B;             

        auto [d0, P0] = dijkstra_path(G, src, dst);
        if (P0.empty()) return {};
        A.push_back({d0,P0});

        for (int k = 1; k < K; k++) {
            const auto& prev_path = A[k-1].second;

            for (int i = 0; i + 1 < prev_path.size(); i++) {

                int spur_node = prev_path[i];
                std::vector<int> root_path(prev_path.begin(), prev_path.begin() + i + 1);

                std::unordered_map<int, std::vector<std::pair<int,int>>> adj_backup = G.adj;

                for (auto& [qw,P] : A) {
                    if (P.size() > i &&
                        std::equal(P.begin(), P.begin() + i, root_path.begin()))
                    {
                        int u = P[i];
                        int v = P[i+1];

                        auto& nbrs = G.adj[u];
                        nbrs.erase(
                            std::remove_if(
                                nbrs.begin(), nbrs.end(),
                                [&](auto& pr){ return pr.first == v; }),
                            nbrs.end()
                        );
                    }
                }
                for (int j = 0; j < i; j++) {
                    int node = root_path[j];
                    G.adj[node].clear();    
                }

                auto [spur_dist, spur_path] = dijkstra_path(G, spur_node, dst);

                G.adj = adj_backup;

                if (spur_path.empty()) continue;
                std::vector<int> total_path = root_path;
                total_path.pop_back();
                total_path.insert(total_path.end(), spur_path.begin(), spur_path.end());

                double total_cost = 0;
                for (int p = 0; p + 1 < total_path.size(); p++) {
                    int u = total_path[p];
                    int v = total_path[p+1];
                    for (auto& [nbr, eid] : G.adj[u]) {
                        if (nbr == v) {
                            total_cost += G.edges.at(eid).length_m;
                            break;
                        }
                    }
                }

                B.insert({total_cost, total_path});
            }

            if (B.empty()) break;

            auto it = B.begin();
            A.push_back({it->first,it->second});
            B.erase(it);
        }

        return A;
    }