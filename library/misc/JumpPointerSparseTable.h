#pragma once

// Sparse table for random access in a linked list
struct JumpPointerSparseTable {
    int                 N;         // the number of vertex
    int                 logN;      // 
    vector<vector<int>> P;         // P[0][n] points to the parent
                                    // parent & acestors

    JumpPointerSparseTable(int N) {
        init(N);
    }

    void init(int n) {
        N = n;
        logN = (int)log2(N) + 1;
        P.resize(logN);
        for (int i = 0; i < logN; i++)
            P[i].resize(N);
    }

    // for batch building
    // O(NlogN)
    void build() {
        for (int i = 1; i < logN; i++) {
            for (int j = 0; j < N; j++) {
                int pp = P[i - 1][j];
                P[i][j] = pp < 0 ? pp : P[i - 1][pp];
            }
        }
    }

    void build(const int A[], int n) {
        copy(A, A + n, P[0].begin());
        build();
    }

    void build(const vector<int>& A) {
        copy(A.begin(), A.end(), P[0].begin());
        build();
    }

    // for incremental building
    // x = index, next = x's next index (pointer)
    // 0 <= x < N, -1 <= next < N
    void add(int x, int next) {
        P[0][x] = next;
        for (int i = 1; i < logN; i++) {
            int pp = P[i - 1][x];
            P[i][x] = pp < 0 ? pp : P[i - 1][pp];
        }
    }

    // get next[next[next[...[x]]]]
    // O(logD)
    int query(int x, int dist) const {
        if (dist <= 0)
            return x;

        for (int i = 0; dist > 0; i++) {
            if (dist & 1)
                x = P[i][x];
            dist >>= 1;
        }

        return x;
    }

    //--- find

    // get last value from start in range [leftLimit, N)
    int findFirst(int start, int leftLimit) const {
        if (start >= leftLimit)
            return start;

        int res = start;
        for (int i = logN - 1; i >= 0; i--) {
            int next = P[i][res];
            if (0 <= next && next < leftLimit)
                res = next;
        }
        return P[0][res] >= leftLimit ? P[0][res] : res;
    }

    // find first position where f(x) is true in [start, N)
    //   f(x): xxxxxxxxxxxOOOOOOOO
    //         S          ^
    int findFirst(int start, const function<bool(int)>& f) const {
        if (f(start))
            return start;

        int res = start;
        for (int i = logN - 1; i >= 0; i--) {
            int next = P[i][res];
            if (0 <= next && !f(next))
                res = next;
        }
        return f(P[0][res]) ? P[0][res] : res;
    }

    // get last value from start in range [start, rightLimit]
    int findLast(int start, int rightLimit) const {
        int res = start;
        for (int i = logN - 1; i >= 0; i--) {
            int next = P[i][res];
            if (0 <= next && next <= rightLimit)
                res = next;
        }

        return res;
    }

    // find last position where f(x) is true [start, N)
    //   f(x): OOOOOOOOOOOxxxxxxxx
    //         S         ^
    int findLast(int start, const function<bool(int)>& f) const {
        int res = start;
        for (int i = logN - 1; i >= 0; i--) {
            int next = P[i][res];
            if (0 <= next && f(next))
                res = next;
        }

        return res;
    }
};
