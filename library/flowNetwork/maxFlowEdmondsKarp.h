#pragma once

// Edmonds-Karp Algorithm (Ford-Fulkerson method)
template <typename T, const T INF = 0x3f3f3f3f>
struct MaxFlowEdmondsKarp {
    struct Edge {
        int to;                     // v
        int revIndex;               // for (v -> u)

        T   flow;
        T   capacity;
    };

    int N;                          // the number of vertices
    vector<vector<Edge>> edges;
    vector<pair<int, int>> parent;  // (u, edge index in u's edges)

    MaxFlowEdmondsKarp() : N(0) {
    }

    explicit MaxFlowEdmondsKarp(int n) : N(n), edges(N), parent(N, make_pair(-1, -1)) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<Edge>>(N);
        parent = vector<pair<int,int>>(N, make_pair(-1, -1));
    }

    // add edges to a directed graph
    void addEdge(int u, int v, T capacity, T capacityRev) {
        int uN = int(edges[u].size());
        int vN = int(edges[v].size());
        edges[u].push_back(Edge{ v, vN, 0, capacity });
        edges[v].push_back(Edge{ u, uN, 0, capacityRev });
    }

    void clearFlow() {
        for (auto& vec : edges) {
            for (auto& e : vec)
                e.flow = 0;
        }
    }

    // O(min(V * E^2, E * flow))
    T calcMaxFlow(int s, int t) {
        //clearFlow();

        T res = 0;

        while (bfs(s, t)) {
            T amount = INF;
            for (int i = t; i != s; i = parent[i].first) {
                int u = parent[i].first;
                auto& e = edges[u][parent[i].second];
                amount = min(amount, e.capacity - e.flow);
            }

            for (int i = t; i != s; i = parent[i].first) {
                int u = parent[i].first;
                auto& e = edges[u][parent[i].second];
                e.flow += amount;
                edges[e.to][e.revIndex].flow -= amount;
            }
            res += amount;
        }

        return res;
    }

private:
    bool bfs(int s, int t) {
        fill(parent.begin(), parent.end(), make_pair(-1, -1));
        //memset(&parent[0], -1, sizeof(parent[0]) * parent.size());

        queue<int> q;

        q.push(s);
        parent[s] = make_pair(s, -1);
        while (!q.empty() && parent[t].first == -1) {
            int u = q.front();
            q.pop();

            for (int i = 0; i < int(edges[u].size()); i++) {
                auto& e = edges[u][i];
                if (parent[e.to].first == -1 && (e.capacity - e.flow) > 0) {
                    q.push(e.to);
                    parent[e.to] = make_pair(u, i);
                }
            }
        }

        return parent[t].first != -1;
    }
};
