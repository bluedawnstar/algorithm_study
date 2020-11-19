#pragma once

/*
  Algebraic Algorithm - general matching for undirected graph
  - O(N^3)

  <Ref>
  - https://uoj.ac/submission/125456
*/
struct AlgebraicGeneralMaxMatching {
    static const int MOD = 1'000'000'007;

    int N;
    vector<vector<int>> adj;

    AlgebraicGeneralMaxMatching() : N(0) {
    }

    AlgebraicGeneralMaxMatching(int n) {
        init(n);
    }

    void init(int n) {
        N = n;
        adj = vector<vector<int>>(N, vector<int>(N));
    }

    // add edges to a undirected graph
    void addEdge(int u, int v) {
        if (u > v)
            swap(u, v);
        adj[v][u] = -(adj[u][v] = rand() % (MOD - 1) + 1);
    }

    // O(N^3)
    int calcMaxMatching() {
        A = adj;
        id = vector<int>(N);
        iota(id.begin(), id.end(), 0);
        gauss(false);

        vector<int> temp(N, -1);
        int res = 0;
        for (int i = 0; i < N; i++) {
            if (A[id[i]][id[i]])
                temp[res++] = i;
        }
        res /= 2;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++)
                A[i][j] = (temp[i] >= 0) ? adj[temp[i]][temp[j]] : 0;
        }
        gauss(true);

        //--- assign
        match = vector<int>(N, -1);
        row = vector<bool>(N);
        col = vector<bool>(N);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++)
                A[i][j] = (temp[i] >= 0) ? adj[temp[i]][temp[j]] : 0;
        }

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if ((temp[i] < 0 || match[temp[i]] >= 0) || (temp[j] < 0 || match[temp[j]] >= 0))
                    continue;
                if (A[i][j] && B[j][i]) {
                    eliminate(i, j);
                    eliminate(j, i);
                    match[temp[i]] = temp[j];
                    match[temp[j]] = temp[i];
                }
            }
        }

        return res;
    }

    vector<pair<int, int>> getLastMaxMatchingEdges() {
        vector<pair<int, int>> res;
        for (int u = 0; u < N; u++) {
            if (u < match[u])
                res.emplace_back(u, match[u]);
        }
        return res;
    }

private:
    vector<vector<int>> A, B;
    vector<int> id;
    vector<int> match;

    vector<bool> row, col;

    int modPow(int x, int n) {
        long long res = 1;

        long long t = x % MOD;
        for (; n; n >>= 1) {
            if (n & 1)
                res = res * t % MOD;
            t = t * t % MOD;
        }

        return int(res);
    }

    void gauss(bool updateB) {
        B = vector<vector<int>>(N, vector<int>(N));
        for (int i = 0; i < N; i++)
            B[i][i] = 1;

        for (int i = 0; i < N; i++) {
            for (int j = i; j < N; j++) {
                if (A[j][i]) {
                    swap(id[i], id[j]);
                    for (int k = i; k < N; k++)
                        swap(A[i][k], A[j][k]);
                    if (updateB)
                        swap(B[i], B[j]);
                    break;
                }
            }
            if (!A[i][i])
                continue;
            for (int j = 0, inv = modPow(A[i][i], MOD - 2); j < N; j++) {
                if (j != i && A[j][i]) {
                    int t = int(1ll * A[j][i] * inv % MOD);
                    for (int k = i; k < N; k++)
                        A[j][k] = int((A[j][k] - 1ll * t * A[i][k]) % MOD);
                    if (updateB) {
                        for (int k = 0; k < N; k++) {
                            if (B[i][k])
                                B[j][k] = int((B[j][k] - 1ll * t * B[i][k]) % MOD);
                        }
                    }
                }
            }
        }
        if (updateB) {
            for (int i = 0; i < N; ++i) {
                int t = modPow(A[i][i], MOD - 2);
                for (int j = 0; j < N; j++)
                    B[i][j] = int(1ll * B[i][j] * t % MOD);
            }
        }
    }

    void eliminate(int x, int y) {
        row[x] = col[y] = true;
        for (int i = 0, inv = modPow(B[x][y], MOD - 2); i < N; i++) {
            if (row[i])
                continue;
            int t = int(1ll * B[i][y] * inv % MOD);
            for (int j = 0; j < N; ++j) {
                if (!col[j])
                    B[i][j] = int((B[i][j] - 1ll * B[x][j] * t) % MOD);
            }
        }
    }
};
