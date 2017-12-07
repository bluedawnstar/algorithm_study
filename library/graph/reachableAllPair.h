#pragma once

// connectivity of all pairs
struct ReachableAllPair {
    // Floyd-Warshal algorithm : O(V^3)
    static void doFloydWarshal(vector<vector<bool>>& D, int N) {
        for (int i = 0; i < N; i++)
            D[i][i] = true;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (D[i][j])
                    continue;
                for (int k = 0; k < N; k++)
                    D[i][j] = D[i][k] && D[k][j];
            }
        }
    }
};
