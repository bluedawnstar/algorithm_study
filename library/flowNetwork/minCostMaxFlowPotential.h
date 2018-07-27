#pragma once

template <typename T, const T INF = 0x3f3f3f3f>
struct MinCostMaxFlowPotential {
    struct Edge {
        int to;                 // v
        int revIndex;           // for (v -> u)

        T   flow;
        T   cost;

        T   capacity;           // capacity for flow
    };

    int N;                      // the number of vertices
    vector<vector<Edge>> edges;

    MinCostMaxFlowPotential() : N(0) {
    }

    explicit MinCostMaxFlowPotential(int n) : N(n), edges(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<Edge>>(N);
    }

    // add edges to a directed graph
    void addEdge(int u, int v, T capacity, T cost) {
        int uN = int(edges[u].size());
        int vN = int(edges[v].size());
        edges[u].push_back(Edge{ v, vN, 0, cost, capacity });
        edges[v].push_back(Edge{ u, uN, 0, -cost, 0 });
    }

    void clearFlow() {
        for (auto& vec : edges) {
            for (auto& e : vec)
                e.flow = 0;
        }
    }

    // time complexity: O(min(V * E^2 * log(V), E * log(V) * flow))
    // (flow, cost)
    pair<T, T> calcMinCostMaxFlow(int s, int t, T maxFlow = INF) {
        //clearFlow();

        // bellmanFord can be skipped if edges' costs are non-negative
        vector<T> potential(N, INF);
        bellmanFord(s, potential);

        vector<T> currFlow(N);
        vector<pair<int, int>> parent(N);  // (u, edge index in u's edges)

        pair<T, T> res;
        while (res.first < maxFlow) {
            priority_queue<pair<T, int>> Q;
            Q.emplace(T(0), s);

            vector<T> priority(N, INF);
            priority[s] = 0;

            vector<bool> finished(N);
            currFlow[s] = INF;
            while (!Q.empty() && !finished[t]) {
                auto uPriority = -Q.top().first;
                auto u = Q.top().second;
                Q.pop();
                
                if (uPriority != priority[u])
                    continue;

                finished[u] = true;
                for (int i = 0; i < int(edges[u].size()); i++) {
                    Edge& e = edges[u][i];
                    if (e.flow >= e.capacity)
                        continue;

                    int v = e.to;
                    T newPriority = priority[u] + e.cost + potential[u] - potential[v];
                    if (priority[v] > newPriority) {
                        priority[v] = newPriority;
                        Q.emplace(-newPriority, v);

                        parent[v] = make_pair(u, i);

                        currFlow[v] = min(currFlow[u], e.capacity - e.flow);
                    }
                }
            }
            if (priority[t] == INF)
                break;

            for (int i = 0; i < N; i++) {
                if (finished[i])
                    potential[i] += priority[i] - priority[t];
            }

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
    void bellmanFord(int s, vector<T>& dist) {
        dist[s] = 0;

        vector<bool> inQ(N);

        queue<int> Q;
        Q.push(s);
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();
            inQ[u] = false;

            for (int i = 0; i < int(edges[u].size()); i++) {
                auto& e = edges[u][i];
                if (e.flow >= e.capacity)
                    continue;

                int v = e.to;
                if (dist[v] > dist[u] + e.cost) {
                    dist[v] = dist[u] + e.cost;
                    if (!inQ[v]) {
                        Q.push(v);
                        inQ[v] = true;
                    }
                }
            }
        }
    }
};