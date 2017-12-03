#pragma once

template <typename T>
struct MinCostMaxFlowSPFA {
    static const T INF = T(0x3f3f3f3f);

    struct Edge {
        int to;                 // v
        int revIndex;           // for (v -> u)

        T   flow;
        T   cost;

        T   capacity;           // capacity for flow
    };

    int N;                      // the number of vertices
    vector<vector<Edge>> edges;

    MinCostMaxFlowSPFA() : N(0) {
    }

    MinCostMaxFlowSPFA(int n) : N(n), edges(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<Edge>>(N);
    }

    // add edges to a directed graph
    void addEdge(int u, int v, T capacity, T cost) {
        int uN = (int)edges[u].size();
        int vN = (int)edges[v].size();
        edges[u].push_back(Edge{ v, vN, 0, cost, capacity });
        edges[v].push_back(Edge{ u, uN, 0, -cost, 0 });
    }

    void clearFlow() {
        for (auto& vec : edges) {
            for (auto& e : vec)
                e.flow = 0;
        }
    }

    // time complexity: O(V * E * flow) => practical O((V + E) * flow)
    // (flow, cost)
    pair<T, T> calcMinCostMaxFlow(int s, int t, T maxFlow = INF) {
        //clearFlow();

        vector<bool> inQ(N);

        pair<T, T> res;
        while (res.first < maxFlow) {
            vector<pair<int, int>> parent(N, make_pair(-1, -1));  // (u, edge index in u's edges)

            vector<T> dist(N, INF);
            dist[s] = 0;

            queue<int> Q;
            Q.push(s);
            while (!Q.empty()) {
                int u = Q.front();
                Q.pop();
                inQ[u] = false;

                for (int i = 0; i < (int)edges[u].size(); i++) {
                    Edge& e = edges[u][i];
                    if (e.flow >= e.capacity)
                        continue;

                    int v = e.to;
                    if (dist[v] > dist[u] + e.cost) {
                        dist[v] = dist[u] + e.cost;
                        parent[v] = make_pair(u, i);
                        if (!inQ[v]) {
                            Q.push(v);
                            inQ[v] = true;
                        }
                    }
                }
            }
            if (parent[t].first < 0)
                break;

            T flow = maxFlow - res.first;
            T cost = 0;
            for (int v = t; v != s; v = parent[v].first) {
                int u = parent[v].first;
                Edge& e = edges[u][parent[v].second];
                if (flow > e.capacity - e.flow)
                    flow = e.capacity - e.flow;
                cost += e.cost;
            }
            for (int v = t; v != s; v = parent[v].first) {
                int u = parent[v].first;
                Edge& e = edges[u][parent[v].second];
                e.flow += flow;
                edges[e.to][e.revIndex].flow -= flow;
            }
            res.first += flow;
            res.second += flow * cost;
        }

        return res;
    }
};