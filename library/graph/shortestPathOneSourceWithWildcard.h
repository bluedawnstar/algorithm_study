#pragma once

// https://www.hackerearth.com/practice/algorithms/graphs/shortest-path-algorithms/practice-problems/algorithm/shortest-path-revisited-9e1091ea/description/

// 'Wildcard' means an edge that does not pay. (free pass edge?)

template <typename T, const T INF = 0x3f3f3f3f>
struct ShortestPathWithWildcard {
    int N;                                  // the number of vertexes
    int K;                                  // the maximum number of wildcards to be used in a path
    vector<vector<pair<int, T>>> edges;     // (vertex, weight)
    vector<vector<T>> dist;
    vector<vector<int>> parent;

    ShortestPathWithWildcard() : N(0), K(0) {
    }

    ShortestPathWithWildcard(int n, int k) {
        init(n, k);
    }

    void init(int n, int k) {
        N = n;
        K = k;
        edges = vector<vector<pair<int, T>>>(N);
        dist = vector<vector<T>>(N, vector<T>(K + 1, INF));
        parent = vector<vector<int>>(N, vector<int>(K + 1, -1));
    }

    // add edges to directed graph
    void addEdge(int u, int v, int w) {
        edges[u].emplace_back(v, w);
        edges[v].emplace_back(u, w);
    }

    // O(K*E*logV)
    void dijkstra(int start) {
        //dist = vector<vector<T>>(N, vector<T>(K + 1, INF));
        //parent = vector<vector<int>>(N, vector<int>(K + 1, -1));

        priority_queue<tuple<T, int, int>> pq;    // (-weight, vertex, level)

        pq.emplace(0, start, 0);
        dist[start][0] = 0;
        parent[start][0] = -1;
        while (!pq.empty()) {
            T w = -get<0>(pq.top());        // weight
            int u = get<1>(pq.top());       // vertex u
            int levelU = get<2>(pq.top());

            pq.pop();
            if (dist[u][levelU] < w)
                continue;

            for (int i = 0; i < int(edges[u].size()); i++) {
                int v = edges[u][i].first;
                int vw = edges[u][i].second;

                // move on same level
                {
                    T vDist = w + vw;
                    if (dist[v][levelU] > vDist) {
                        pq.emplace(-vDist, v, levelU);
                        dist[v][levelU] = vDist;
                        parent[v][levelU] = u;
                    }
                }
                // move to next level
                if (levelU < K) {
                    if (dist[v][levelU + 1] > w) {
                        pq.emplace(-w, v, levelU + 1);
                        dist[v][levelU + 1] = w;
                        parent[v][levelU + 1] = u;
                    }
                }
            }
        }
    }

    T getDistance(int v) {
        return *min_element(dist[v].begin(), dist[v].end());
    }
};
