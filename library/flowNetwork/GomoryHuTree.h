#pragma once

template <typename T>
struct GomoryHuTree {
    static const T INF = T(0x3f3f3f3f);

    int N;
    vector<vector<pair<int, T>>> tree; // (v, flow)

    GomoryHuTree(int n) : N(n), tree(N) {
    }

    // O(N) --> O(logN) if RMQ on tree is used
    T calcMaxFlow(int s, int t, T maxFlow = INF) {
        return dfs(s, t, -1, maxFlow);
    }

private:
    T dfs(int u, int t, int parent, T maxFlow) {
        if (u == t)
            return maxFlow;

        T flow = INF;
        for (auto& e : tree[u]) {
            int v = e.first;
            if (v != parent)
                flow = min(flow, dfs(v, t, u, min(maxFlow, e.second)));
        }

        return flow;
    }
};

// for undirected graph
template <typename T>
struct GomoryHuTreeBuilder {
    static const T INF = T(0x3f3f3f3f);

    struct Edge {
        int to;                 // v
        int revIndex;           // for (v -> u)

        T   flow;
        T   capacity;           // 
    };

    int N;                      // the number of vertices
    vector<vector<Edge>> edges; // graph

    GomoryHuTreeBuilder() : N(0) {
    }

    GomoryHuTreeBuilder(int n) : N(n), edges(N + 1) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<Edge>>(N + 1);
    }

    // add edges to a undirected graph
    void addEdge(int u, int v, T capacity) {
        int uN = (int)edges[u].size();
        int vN = (int)edges[v].size();
        edges[u].push_back(Edge{ v, vN, 0, capacity });
        edges[v].push_back(Edge{ u, uN, 0, capacity });
    }

    void clearFlow() {
        for (auto& vec : edges) {
            for (auto& e : vec)
                e.flow = 0;
        }
    }

    // O(V^3 * flow) or O(VE * flow)
    GomoryHuTree<T> build() {
        vector<int> parent(N, 0);
        vector<T> maxFlow(N);
        vector<pair<int, int>> prev(N); // (u, edge index in u's edges)

        for (int s = 1; s < N; ++s) {
            T totalFlow = 0;
            clearFlow();

            int t = parent[s];
            while (true) {
                queue<int> Q;
                Q.push(s);

                fill(prev.begin(), prev.end(), make_pair(-1, -1));
                prev[s].first = -2;
                while (!Q.empty() && prev[t].first == -1) {
                    int u = Q.front();
                    Q.pop();

                    for (int i = 0; i < (int)edges[u].size(); i++) {
                        Edge& e = edges[u][i];
                        int v = e.to;
                        if (prev[v].first == -1 && (e.capacity - e.flow) > 0) {
                            prev[v] = make_pair(u, i);
                            Q.push(v);
                        }
                    }
                }
                if (prev[t].first == -1)
                    break;

                T flow = INF;
                for (int v = t; v != s; v = prev[v].first) {
                    int u = prev[v].first;
                    Edge& e = edges[u][prev[v].second];
                    flow = min(flow, e.capacity - e.flow);
                }
                for (int v = t; v != s; v = prev[v].first) {
                    int u = prev[v].first;
                    Edge& e = edges[u][prev[v].second];
                    e.flow += flow;
                    edges[e.to][e.revIndex].flow += flow;   //+
                }
                totalFlow += flow;
            }

            maxFlow[s] = totalFlow;
            for (int u = 0; u < N; ++u) {
                if (u != s && prev[u].first != -1 && parent[u] == t)
                    parent[u] = s;
            }
            if (prev[parent[t]].first != -1) {
                parent[s] = parent[t];
                parent[t] = s;
                maxFlow[s] = maxFlow[t];
                maxFlow[t] = totalFlow;
            }
        }

        GomoryHuTree<T> res(N);
        for (int u = 0; u < N; ++u) {
            if (u != parent[u]) {
                res.tree[u].emplace_back(parent[u], maxFlow[u]);
                res.tree[parent[u]].emplace_back(u, maxFlow[u]);
            }
        }

        return move(res);
    }
};
