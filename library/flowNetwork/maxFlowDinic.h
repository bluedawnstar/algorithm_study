#pragma once

// Dinic Algorithm
template <typename T, const T INF = 0x3f3f3f3f>
struct MaxFlowDinic {
    struct Edge {
        int to;                     // v
        int revIndex;               // for (v -> u)

        T   flow;
        T   capacity;

        T   edgeIndex;              // 
    };

    int N;                          // the number of vertices
    vector<vector<Edge>> edges;
    vector<int> levels;             // 

    MaxFlowDinic() : N(0) {
    }

    explicit MaxFlowDinic(int n) : N(n), edges(N), levels(N, -1) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<Edge>>(N);
        levels = vector<int>(N, -1);
    }

    // add edges to a directed graph
    void addEdge(int u, int v, T capacity, T capacityRev) {
        int uN = int(edges[u].size());
        int vN = int(edges[v].size());
        edges[u].push_back(Edge{ v, vN, 0, capacity, -1 });
        edges[v].push_back(Edge{ u, uN, 0, capacityRev, -1 });
    }

    void addEdge(int u, int v, T capacity, T capacityRev, int edgeIndex) {
        int uN = int(edges[u].size());
        int vN = int(edges[v].size());
        edges[u].push_back(Edge{ v, vN, 0, capacity, edgeIndex });
        edges[v].push_back(Edge{ u, uN, 0, capacityRev, -1 });
    }

    void clearFlow() {
        for (auto& vec : edges) {
            for (auto& e : vec)
                e.flow = 0;
        }
    }

    // O(min(V^2 * E, E * flow))
    T calcMaxFlow(int s, int t) {
        //clearFlow();

        T res = 0;

        while (bfs(s, t)) {
            vector<int> start(N);
            while (true) {
                T flow = dfs(s, t, INF, start);
                if (flow <= 0)
                    break;
                res += flow;
            }
        }

        return res;
    }

private:
    bool bfs(int s, int t) {
        fill(levels.begin(), levels.end(), -1);
        //memset(&levels[0], -1, sizeof(levels[0]) * levels.size());

        queue<int> q;

        q.push(s);
        levels[s] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int i = 0; i < int(edges[u].size()); i++) {
                auto& e = edges[u][i];
                if (levels[e.to] < 0 && (e.capacity - e.flow) > 0) {
                    q.push(e.to);
                    levels[e.to] = levels[u] + 1;
                }
            }
        }

        return levels[t] >= 0;
    }

    T dfs(int u, int t, T flow, vector<int>& start) {
        if (u == t)
            return flow;

        for (; start[u] < int(edges[u].size()); start[u]++) {
            auto& e = edges[u][start[u]];

            if (levels[e.to] == levels[u] + 1 && (e.capacity - e.flow) > 0) {
                T currFlow = min(flow, e.capacity - e.flow);

                T tempFlow = dfs(e.to, t, currFlow, start);
                if (tempFlow > 0) {
                    e.flow += tempFlow;
                    edges[e.to][e.revIndex].flow -= tempFlow;
                    return tempFlow;
                }
            }
        }

        return 0;
    }
};
