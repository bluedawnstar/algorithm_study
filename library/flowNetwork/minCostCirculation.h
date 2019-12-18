#pragma once

// from https://github.com/koosaga/DeobureoMinkyuParty/blob/master/teamnote.pdf
// Cycle-cancelling algorithm

template <typename T, const T INF = 0x3f3f3f3f>
struct MinCostCirculation {
    struct Edge {
        int to;                     // v
        int revIndex;               // for (v -> u)

        T   capacity;
        T   cost;
    };

    int N;
    vector<vector<Edge>> edges;

    MinCostCirculation() : N(0) {
    }

    explicit MinCostCirculation(int n) {
        init(n);
    }


    void init(int n) {
        N = n;
        edges = vector<vector<Edge>>(N);

        dist.resize(N);
        parent.resize(N);
        parentEdge.resize(N);
    }

    void clear() {
        for (int i = 0; i < N; i++)
            edges[i].clear();
    }

    void addEdge(int u, int v, T capacity, T cost) {
        int uN = int(edges[u].size());
        int vN = int(edges[v].size());
        edges[u].push_back(Edge{ v, vN, capacity,  cost });
        edges[v].push_back(Edge{ u, uN,        0, -cost });
    }

    //---

    // return (total cost of negative cycles), O(flow * C * VE^2)
    // -> side-effect: capacity values of edges are updated (negative-cycles cancelled)
    T solve() {
        T ans = 0;

        int root = -1;
        while ((root = negativeCycle()) != -1) {
            vector<bool> mark(N);

            // make a negative-cycle path
            vector<pair<int,int>> cycle;
            while (!mark[root]) {
                cycle.emplace_back(parent[root], parentEdge[root]);
                mark[root] = true;
                root = parent[root];
            }
            reverse(cycle.begin(), cycle.end());

            T cap = INF;
            for (auto& it : cycle)
                cap = min(cap, edges[it.first][it.second].capacity);

            for (auto& it : cycle) {
                auto& e = edges[it.first][it.second];

                e.capacity -= cap;
                edges[e.to][e.revIndex].capacity += cap;
                ans += e.cost * cap;
            }
        }

        return ans;
    }

private:
    vector<T> dist;
    vector<T> parent;
    vector<T> parentEdge;

    int negativeCycle() {
        vector<bool> mark(N);
        fill(dist.begin(), dist.end(), 0);

        for (int i = 0; i <= N; i++) {
            for (int u = 0; u < N; u++) {
                for (int j = 0; j < int(edges[u].size()); j++) {
                    auto& e = edges[u][j];
                    if (e.capacity > 0 && dist[e.to] > dist[u] + e.cost) {
                        dist[e.to] = dist[u] + e.cost;
                        parent[e.to] = u;
                        parentEdge[e.to] = j;
                        if (i == N) {
                            int node = u;
                            while (!mark[node]) {
                                mark[node] = true;
                                node = parent[node];
                            }
                            return node;    // cycle start
                        }
                    }
                }
            }
        }

        return -1;
    }
};
