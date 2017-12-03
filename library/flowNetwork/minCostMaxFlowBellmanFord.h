#pragma once

template <typename T>
struct MinCostMaxFlowBellmanFord {
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

    MinCostMaxFlowBellmanFord() : N(0) {
    }

    MinCostMaxFlowBellmanFord(int n) : N(n), edges(N + 1) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<Edge>>(N + 1);
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

    // time complexity: O(min(V^2 * E^2, V * E * flow))
    // (flow, cost)
    pair<T, T> calcMinCostMaxFlow(int s, int t, T maxFlow = INF) {
        //clearFlow();

        pair<T, T> res;
        while (res.first < maxFlow) {
            vector<T> dist(N + 1, INF);
            vector<T> currFlow(N + 1);
            vector<pair<int, int>> parent(N + 1);  // (u, edge index in u's edges)

            bellmanFord(s, dist, currFlow, parent);
            if (dist[t] == INF)
                break;

            T flow = min(currFlow[t], maxFlow - res.first);
            res.first += flow;
            for (int v = t; v != s; v = parent[v].first) {
                int u = parent[v].first;
                Edge& e = edges[u][parent[v].second];
                
                e.flow += flow;
                edges[e.to][e.revIndex].flow -= flow;

                res.second += flow * e.cost;
            }
        }

        return res;
    }

private:
    void bellmanFord(int s, vector<T>& dist, vector<T>& currFlow, vector<pair<int, int>>& parent) {
        dist[s] = 0;
        currFlow[s] = INF;

        vector<bool> inQ(N + 1);

        queue<int> Q;
        Q.push(s);
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();
            inQ[u] = false;

            for (int i = 0; i < (int)edges[u].size(); i++) {
                auto& e = edges[u][i];
                if (e.flow >= e.capacity)
                    continue;

                int v = e.to;
                if (dist[v] > dist[u] + e.cost) {
                    dist[v] = dist[u] + e.cost;

                    parent[v] = make_pair(u, i);
                    currFlow[v] = min(currFlow[u], e.capacity - e.flow);
                    if (!inQ[v]) {
                        Q.push(v);
                        inQ[v] = true;
                    }
                }
            }
        }
    }
};