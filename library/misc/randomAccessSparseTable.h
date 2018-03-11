#pragma once

// Sparse table for random access in a linked list
struct RandomAccessSparseTable {
    int                 mN;         // the number of vertex
    int                 mLogN;      // 
    vector<vector<int>> mP;         // mP[0][n] points to the parent
                                    // parent & acestors

    RandomAccessSparseTable(int N) {
        init(N);
    }

    void init(int N) {
        mN = N;
        mLogN = (int)log2(N) + 1;
        mP.resize(mLogN);
        for (int i = 0; i < mLogN; i++)
            mP[i].resize(mN);
    }

    // for batch building
    // O(NlogN)
    void build() {
        for (int i = 1; i < mLogN; i++) {
            for (int j = 0; j < mN; j++) {
                int pp = mP[i - 1][j];
                mP[i][j] = pp < 0 ? pp : mP[i - 1][pp];
            }
        }
    }

    void build(const int A[], int n) {
        copy(A, A + n, mP[0].begin());
        build();
    }

    void build(const vector<int>& A) {
        copy(A.begin(), A.end(), mP[0].begin());
        build();
    }

    // for incremental building
    // x = index, next = x's next index (pointer)
    // 0 <= x < N, -1 <= next < N
    void add(int x, int next) {
        mP[0][x] = next;
        for (int i = 1; i < mLogN; i++) {
            int pp = mP[i - 1][x];
            mP[i][x] = pp < 0 ? pp : mP[i - 1][pp];
        }
    }

    // get next[next[next[...[x]]]]
    // O(logD)
    int query(int x, int dist) const {
        if (dist <= 0)
            return x;

        for (int i = 0; dist > 0; i++) {
            if (dist & 1)
                x = mP[i][x];
            dist >>= 1;
        }

        return x;
    }

    // get last value from start in range [start, rightLimit]
    int findLast(int start, int rightLimit) const {
        int res = start;
        for (int i = mLogN - 1; i >= 0; i--) {
            int next = mP[i][res];
            if (0 <= next && next <= rightLimit)
                res = next;
        }

        return res;
    }

    // find last position where f(x) is true
    //   f(x): OOOOOOOOOOOxxxxxxxx
    //         S         ^
    int findLast(int start, const function<bool(int)>& f) const {
        int res = start;
        for (int i = mLogN - 1; i >= 0; i--) {
            int next = mP[i][res];
            if (0 <= next && f(next))
                res = next;
        }

        return res;
    }

    // get last value from start in range [leftLimit, N)
    int findFirst(int start, int leftLimit) const {
        if (start >= leftLimit)
            return start;

        int res = start;
        for (int i = mLogN - 1; i >= 0; i--) {
            int next = mP[i][res];
            if (0 <= next && next < leftLimit)
                res = next;
        }
        return mP[0][res] >= leftLimit ? mP[0][res] : res;
    }

    // find first position where f(x) is true in [start, N)
    //   f(x): xxxxxxxxxxxOOOOOOOO
    //         S          ^
    int findFirst(int start, const function<bool(int)>& f) const {
        if (f(start))
            return start;

        int res = start;
        for (int i = mLogN - 1; i >= 0; i--) {
            int next = mP[i][res];
            if (0 <= next && !f(next))
                res = next;
        }
        return f(mP[0][res]) ? mP[0][res] : res;
    }
};
