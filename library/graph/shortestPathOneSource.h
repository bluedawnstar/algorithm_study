#pragma once

// for directed graph
template <typename T, const T INF = 0x3f3f3f3f>
struct ShortestPath {
    int N;
    vector<vector<pair<int, T>>> edges;   // (vertex, weight)
    vector<T> dist;
    vector<int> parent;

    ShortestPath() : N(0) {
    }

    ShortestPath(int n) : N(n), edges(n), dist(n), parent(n) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<pair<int, T>>>(N);
        dist = vector<T>(N);
        parent = vector<int>(N);
    }

    // add edges to directed graph
    void addEdge(int u, int v, int w) {
        edges[u].emplace_back(v, w);
    }

    void addBiEdge(int u, int v, int w) {
        edges[u].emplace_back(v, w);
        edges[v].emplace_back(u, w);
    }

    // O(E*logV)
    void dijkstra(int start) {
        fill(dist.begin(), dist.end(), INF);
        fill(parent.begin(), parent.end(), -1);

        priority_queue<pair<T, int>> pq;    // (-weight, vertex)

        pq.emplace(0, start);
        dist[start] = 0;
        parent[start] = -1;
        while (!pq.empty()) {
            T w = -pq.top().first;          // weight
            int u = pq.top().second;        // vertex u

            pq.pop();
            if (dist[u] < w)
                continue;

            for (int i = 0; i < int(edges[u].size()); i++) {
                int v = edges[u][i].first;
                T vDist = w + edges[u][i].second;
                if (dist[v] > vDist) {
                    pq.emplace(-vDist, v);
                    dist[v] = vDist;
                    parent[v] = u;
                }
            }
        }
    }

    // O(VE)
    // return false if the graph has negative cycles
    bool bellmanFord(int start) {
        fill(dist.begin(), dist.end(), INF);
        fill(parent.begin(), parent.end(), -1);

        dist[start] = 0;
        parent[start] = -1;

        bool updated = false;
        for (int i = 0; i < N; i++) {
            updated = false;
            for (int u = 0; u < N; u++) {
                for (int j = 0; j < int(edges[u].size()); j++) {
                    int v = edges[u][j].first;
                    T w = edges[u][j].second;
                    if (dist[v] > dist[u] + w) {
                        dist[v] = dist[u] + w;
                        parent[v] = u;
                        updated = true;
                    }
                }
            }
            if (!updated)
                break;
        }
        return !updated;
    }

    // Shortest Path Faster Algorithm
    // time complexity : normal - O(E), worst - O(VE)
    // return false if the graph has negative cycles
    bool spfa(int start) {
        fill(dist.begin(), dist.end(), INF);
        fill(parent.begin(), parent.end(), -1);

        vector<bool> inQ(N);
        vector<int> updated(N);

        dist[start] = 0;
        parent[start] = -1;

        queue<int> Q;
        Q.push(start);
        updated[start]++;
        inQ[start] = true;
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (auto v : edges[u]) {
                if (dist[v.first] > dist[u] + v.second) {
                    dist[v.first] = dist[u] + v.second;
                    parent[v.first] = u;
                    if (!inQ[v.first]) {
                        Q.push(v.first);
                        if (++updated[v.first] >= N)
                            return false;
                        inQ[v.first] = true;
                    }
                }
            }

            inQ[u] = false;
        }

        return true;
    }


    vector<int> getShortestPath(int v) {
        vector<int> res;
        do {
            res.push_back(v);
            v = parent[v];
        } while (v >= 0);

        reverse(res.begin(), res.end());

        return res;
    }
};