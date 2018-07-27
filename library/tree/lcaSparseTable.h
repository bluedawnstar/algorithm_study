#pragma once

/*
<How to use>
    init() -> build() -> climb() / findLCA()
*/

struct LcaSparseTable {
    int                 N;          // the number of vertex
    int                 logN;       // 
    vector<vector<int>> P;          // P[0][n] points to the parent
                                    // parent & acestors
    vector<int>         level;      // level

    LcaSparseTable() {
        N = 0;
        logN = 0;
    }

    LcaSparseTable(int n) {
        init(n);
    }

    LcaSparseTable(const vector<int>& parent, const vector<int>& level) {
        build(parent, level);
    }

    LcaSparseTable(const int parent[], const int level[], int n) {
        build(parent, level, n);
    }


    void init(int n) {
        N = n;
        logN = int(log2(N)) + 1;
        P.resize(logN);
        for (int i = 0; i < logN; i++)
            P[i].resize(N);
        level.resize(N);
    }

    // O(NlogN)
    void build(const vector<int>& parent, const vector<int>& level) {
        init(int(parent.size()));
        copy(parent.begin(), parent.end(), P[0].begin());
        copy(level.begin(), level.end(), this->level.begin());
        build();
    }

    // O(NlogN)
    void build(const int parent[], const int level[], int N) {
        init(N);
        copy(parent, parent + N, P[0].begin());
        copy(level, level + N, this->level.begin());
        build();
    }

    // O(NlogN)
    void build() {
        for (int i = 1; i < logN; i++) {
            for (int j = 0; j < N; j++) {
                int pp = P[i - 1][j];
                P[i][j] = pp < 0 ? pp : P[i - 1][pp];
            }
        }
    }


    // Level ancestor
    // O(logD)
    int climb(int x, int dist) const {
        if (dist <= 0)
            return x;

        for (int i = 0; dist > 0; i++) {
            if (dist & 1)
                x = P[i][x];
            dist >>= 1;
        }

        return x;
    }

    // O(logN)
    int lca(int A, int B) const {
        if (level[A] < level[B])
            swap(A, B);

        A = climb(A, level[A] - level[B]);

        if (A == B)
            return A;

        int bitCnt = 0;
        for (int x = level[A]; x; x >>= 1)
            bitCnt++;

        for (int i = bitCnt - 1; i >= 0; i--) {
            if (P[i][A] > 0 && P[i][A] != P[i][B]) {
                A = P[i][A];
                B = P[i][B];
            }
        }

        return P[0][A];
    }
};
