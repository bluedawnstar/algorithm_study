#pragma once

template <typename T>
struct Hamiltonian {
    static const T INF = 0x3f3f3f3f;

    // Shortest Hamiltonian Cycle : O(2^N * N^2)
    // 'dist' is adjacent array
    // returns (distance, vertex order)
    static pair<T, vector<int>> getShortestCycle(const vector<vector<T>>& dist) {
        int N = (int)dist.size();
        vector<vector<T>> dp(1 << N, vector<T>(N, INF / 2));

        dp[1][0] = 0;
        for (int mask = 1; mask < (1 << N); mask += 2) {
            for (int i = 1; i < N; i++) {
                if (mask & (1 << i)) {
                    for (int j = 0; j < N; j++) {
                        if (mask & (1 << j)) {
                            dp[mask][i] = min(dp[mask][i], dp[mask ^ (1 << i)][j] + dist[j][i]);
                        }
                    }
                }
            }
        }

        pair<T, vector<int>> res(INF, vector<int>(N));
        for (int i = 1; i < N; i++) {
            res.first = min(res.first, dp[(1 << N) - 1][i] + dist[i][0]);
        }

        // reconstruct path
        int cur = (1 << N) - 1;
        int last = 0;
        for (int i = N - 1; i >= 1; i--) {
            int bj = -1;
            for (int j = 1; j < N; j++) {
                if ((cur & (1 << j)) && (bj == -1 || (dp[cur][bj] + dist[bj][last] > dp[cur][j] + dist[j][last])))
                    bj = j;
            }
            res.second[i] = bj;
            cur ^= 1 << bj;
            last = bj;
        }
        return res;
    }

    // Shortest Hamiltonian Path : O(2^N * N^2)
    // 'dist' is adjacent array
    // returns (distance, vertex order)
    static pair<T, vector<int>> getShortestPath(const vector<vector<T>>& dist) {
        int N = (int)dist.size();

        vector<vector<T>> dp(1 << N, vector<T>(N, INF / 2));

        for (int i = 0; i < N; i++)
            dp[1 << i][i] = 0;

        for (int mask = 0; mask < (1 << N); mask++) {
            for (int i = 0; i < N; i++) {
                if (mask & (1 << i)) {
                    for (int j = 0; j < N; j++) {
                        if (mask & (1 << j)) {
                            dp[mask][i] = min(dp[mask][i], dp[mask ^ (1 << i)][j] + dist[j][i]);
                        }
                    }
                }
            }
        }

        pair<T, vector<int>> res(INF, vector<int>(N));
        for (int i = 0; i < N; i++) {
            res.first = min(res.first, dp[(1 << N) - 1][i]);
        }

        // reconstruct path
        int cur = (1 << N) - 1;
        int last = -1;
        for (int i = N - 1; i >= 0; i--) {
            int bj = -1;
            for (int j = 0; j < N; j++) {
                if ((cur & (1 << j)) &&
                    (bj == -1 || (dp[cur][bj] + (last == -1 ? 0 : dist[bj][last]) > dp[cur][j] + (last == -1 ? 0 : dist[j][last]))))
                    bj = j;
            }
            res.second[i] = bj;
            cur ^= 1 << bj;
            last = bj;
        }
        return res;
    }
};
