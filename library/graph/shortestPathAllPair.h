#pragma once

// Shortest paths of all pairs
template <typename T>
struct ShortestAllPairs {
    static const T INF = T(0x3f3f3f3f);

    // Floyd-Warshal algorithm : O(V^3)
    static void doFloydWarshal(vector<vector<T>>& D, vector<vector<int>>& parent, int N) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                parent[i][j] = (i == j || D[i][j] >= INF) ? -1 : i;

        for (int k = 0; k < N; k++) {
            for (int i = 0; i < N; i++) {
                if (D[i][k] >= INF)
                    continue;
                for (int j = 0; j < N; j++) {
                    if (D[k][j] >= INF)
                        continue;
                    if (D[i][j] > D[i][k] + D[k][j]) {
                        D[i][j] = D[i][k] + D[k][j];
                        // D[i][j] = max(D[i][j], -INF);
                        parent[i][j] = parent[k][j];
                    }
                }
            }
        }
    }

    static vector<int> getPath(const vector<vector<int>>& parent, int N, int u, int v) {
        vector<int> path;
        path.reserve(N);

        while (true) {
            path.push_back(v);
            if (u == v)
                break;
            v = parent[u][v];
        }
        reverse(path.begin(), path.end());

        return path;
    }
};
