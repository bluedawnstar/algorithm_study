#pragma once

// It's not handle level (or depth) information
struct LcaSparseTableSimple {
    int                 N;          // the number of vertex
    int                 logN;       // 
    vector<vector<int>> P;          // P[0][n] points to the parent
                                    // parent & acestors
    LcaSparseTableSimple() {
        N = 0;
        logN = 0;
    }

    LcaSparseTableSimple(int N) {
        init(N);
    }

    LcaSparseTableSimple(const vector<int>& A) {
        build(A);
    }

    LcaSparseTableSimple(const int A[], int N) {
        build(A, N);
    }


    void init(int N) {
        N = N;
        logN = int(log2(N)) + 1;
        P.resize(logN);
        for (int i = 0; i < logN; i++)
            P[i].resize(N);
    }

    // O(NlogN)
    void build(const vector<int>& A) {
        init(int(A.size()));
        copy(A.begin(), A.end(), P[0].begin());
        build();
    }

    // O(NlogN)
    void build(const int A[], int N) {
        init(N);
        copy(A, A + N, P[0].begin());
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


    // O(logD)
    int climb(int x, int dist) {
        if (dist <= 0)
            return x;

#if 0
        for (int i = 0; dist > 0; i++) {
            if (dist & 1)
                x = P[i][x];
            dist >>= 1;
        }
#else
        for (; dist; dist &= dist - 1) {
#ifndef __GNUC__
            int i = static_cast<int>(_tzcnt_u32(static_cast<unsigned>(dist)));
#else
            int i = __builtin_ctz(static_cast<unsigned>(dist));
#endif
            x = P[i][x];
        }
#endif

        return x;
    }

    // PRECONDITION: The depth from the root of A and B is the same.
    int findLCA(int A, int B) {
        if (A == B)
            return A;

        for (int i = logN - 1; i >= 0; i--) {
            if (P[i][A] > 0 && P[i][A] != P[i][B]) {
                A = P[i][A];
                B = P[i][B];
            }
        }

        return P[0][A];
    }
};
