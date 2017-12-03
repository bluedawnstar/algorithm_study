#pragma once

// for undirected graph
template <typename T>
struct ShortestPath {
    static const T INF = T(0x3f3f3f3f);

    int N;
    vector<vector<pair<int, T>>> edges;   // (vertex, weight)
    vector<T> dist;
    vector<int> parent;

    ShortestPath() : N(0) {
    }

    ShortestPath(int n) : N(n), edges(N), dist(N), parent(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<pair<int, T>>>(N);
        dist = vector<T>(N);
        parent = vector<T>(N);
    }

    // add edges to undirected graph
    void addEdge(int u, int v, int w) {
        edges[u].emplace_back(v, w);
        edges[v].emplace_back(u, w);
    }

    // O(E*logV)
    void dijkstra(int start) {
        fill(dist.begin(), dist.end(), INF);
        fill(parent.begin(), parent.end(), -1);

        priority_queue<pair<int, int>> pq;   // (-weight, vertex)

        pq.push(make_pair(0, start));
        dist[start] = 0;
        parent[start] = -1;
        while (!pq.empty()) {
            int w = -pq.top().first;        // weight
            int u = pq.top().second;        // vertex u

            pq.pop();
            if (dist[u] < w)
                continue;

            for (int i = 0; i < (int)edges[u].size(); i++) {
                int v = edges[u][i].first;
                int vDist = edges[u][i].second + w;
                if (dist[v] > vDist) {
                    pq.push(make_pair(-vDist, v));
                    dist[v] = vDist;
                    parent[v] = u;
                }
            }
        }
    }

    // O(VE)
    bool bellmanFord(int start) {
        fill(dist.begin(), dist.end(), INF);
        fill(parent.begin(), parent.end(), -1);

        dist[start] = 0;
        parent[start] = -1;

        bool updated = false;
        for (int i = 0; i < N; i++) {
            updated = false;
            for (int u = 0; u < N; u++) {
                for (int j = 0; j < (int)edges[u].size(); j++) {
                    int v = edges[u][j].first;
                    int w = edges[u][j].second;
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
    void spfa(int start) {
        fill(dist.begin(), dist.end(), INF);
        fill(parent.begin(), parent.end(), -1);

        vector<bool> inQ(N);

        dist[start] = 0;
        parent[start] = -1;

        queue<int> Q;
        Q.push(start);
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
                        inQ[v.first] = true;
                    }
                }
            }

            inQ[u] = false;
        }
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