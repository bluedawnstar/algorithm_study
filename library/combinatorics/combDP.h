#pragma once

//--- Calculation with DP -----------------------------------------------------

// PRECONDITION: 0 <= n <= 66
struct CombAllDP {
    vector<vector<long long>> C;

    CombAllDP() {
    }

    CombAllDP(int n) {
        build(n);
    }

    void build(int n) {
        C.assign(n + 1, vector<long long>(n + 1));

        C[0][0] = 1;

        for (int i = 1; i <= n; i++) {
            C[i][0] = 1;
            for (int j = 1; j < i; j++)
                C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
            C[i][i] = 1;
        }
    }

    long long comb(int n, int r) const {
        return C[n][r];
    }
};

// PRECONDITION: 0 <= n <= 66
struct CombCompactDP {
    vector<long long> C;

    CombCompactDP() {
    }

    CombCompactDP(int n) {
        build(n);
    }

    void build(int n) {
        C.assign(n + 1, 0);

        C[0] = 1;

        for (int i = 1; i <= n; i++) {
            C[i] = 1;
            for (int j = i - 1; j > 0; j--)
                C[j] += C[j - 1];
        }
    }

    long long comb(int r) const {
        return C[r];
    }
};
