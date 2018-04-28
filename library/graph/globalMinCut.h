#pragma once

// for undirected and connected graph
template <typename T>
struct GlobalMinCut {
    static const T INF = T(0x3f3f3f3f);

    struct Edge {
        int to;                     // v
        int revIndex;               // for (v -> u)

        T   weight;                 // > 0
    };

    int N;                          // the number of vertices
    vector<vector<Edge>> edges;

    GlobalMinCut() : N(0) {
    }

    explicit GlobalMinCut(int n) : N(n), edges(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<Edge>>>(N);
    }

    // add edges to undirected graph
    void addEdge(int u, int v, T w) {
        int uN = (int)edges[u].size();
        int vN = (int)edges[v].size();
        edges[u].push_back(Edge{ v, vN, w });
        edges[v].push_back(Edge{ u, uN, w });
    }

    // O(V*E * logV)
    // find global minimum cut
    T doStoerWagner(vector<int>& cutGroup) {
        unordered_set<int> vertices;
        vector<vector<int>> group(N);               // vertex group
        vector<unordered_map<int,T>> groupEdges(N); // (v, weight)

        for (int i = 0; i < N; i++) {
            vertices.insert(i);
            group[i].push_back(i);

            auto& s = groupEdges[i];
            for (auto& e : edges[i])
                s[e.to] += e.weight;
        }

        T minCut = INF;
        int minCutGroup;

        while ((int)vertices.size() >= 2 && minCut > 0) {
            auto r = minCutPhase(vertices, group, groupEdges);
            if (minCut > r.first) {
                minCut = r.first;
                minCutGroup = r.second;
            }
        }
        cutGroup = group[minCutGroup];

        return minCut;
    }

private:
    // merge v's edges to u
    static T merge(vector<unordered_map<int,T>>& edges, int u, int v) {
        T res = 0;

        for (auto& e : edges[v]) {
            res += e.second;
            if (e.first == u)
                edges[e.first].erase(v);
            else {
                edges[e.first][u] += e.second;
                edges[e.first].erase(v);

                edges[u][e.first] += e.second;
            }
        }

        return res;
    }

    // Prim's Algorithm : O(E*logV)
    static pair<T, int> minCutPhase(unordered_set<int>& vertices, vector<vector<int>>& group, vector<unordered_map<int,T>>& edges) {
        int N = (int)edges.size();

        int start = *vertices.begin();

        int last = -1;
        vector<int> parent(N, -1);
        vector<bool> visited(N);
        vector<T> cost(N, 0);

        priority_queue<pair<T, int>> Q;
        Q.emplace(T(0), start);
        while (!Q.empty()) {
            int u = Q.top().second;
            Q.pop();

            if (visited[u])
                continue;
            visited[u] = true;
            last = u;

            for (auto& e : edges[u]) {
                int v = e.first;
                if (visited[v])
                    continue;

                cost[v] += e.second;
                parent[v] = u;
                Q.emplace(cost[v], v);
            }
        }

        pair<T, int> res(0, last);
        if (parent[last] >= 0) {
            int u = parent[last];
            int v = last;

            res.first = merge(edges, u, v);
            res.second = v;

            group[u].insert(group[u].end(), group[v].begin(), group[v].end());
            vertices.erase(v);
        }
        return res;
    }
};
