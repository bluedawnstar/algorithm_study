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

    // x = index, next = x's next index (pointer)
    // 0 <= x, next <= N
    void add(int x, int next) {
        mP[0][x] = next;
        for (int i = 1; i < mLogN; i++) {
            int pp = mP[i - 1][x];
            mP[i][x] = pp < 0 ? pp : mP[i - 1][pp];
        }
    }

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
};
