#pragma once

/*
<How to use>
    init() -> build() -> climb() / findLCA()
*/

struct LcaSparseTable {
    int                 mN;         // the number of vertex
    int                 mLogN;      // 
    vector<vector<int>> mP;         // mP[0][n] points to the parent
                                    // parent & acestors
    vector<int>         mLevel;     // level

    LcaSparseTable() {
        mN = 0;
        mLogN = 0;
    }

    LcaSparseTable(int N) {
        init(N);
    }

    LcaSparseTable(const vector<int>& parent, const vector<int>& level) {
        init(parent, level);
    }

    LcaSparseTable(const int parent[], const int level[], int N) {
        init(parent, level, N);
    }


    void init(int N) {
        mN = N;
        mLogN = (int)log2(N) + 1;
        mP.resize(mLogN);
        for (int i = 0; i < mLogN; i++)
            mP[i].resize(mN);
        mLevel.resize(N);
    }

    void init(const vector<int>& parent, const vector<int>& level) {
        init((int)parent.size());
        copy(parent.begin(), parent.end(), mP[0].begin());
        copy(level.begin(), level.end(), mLevel.begin());
    }

    void init(const int parent[], const int level[], int N) {
        init(N);
        copy(parent, parent + N, mP[0].begin());
        copy(level, level + N, mLevel.begin());
    }

    // O(NlogN)
    void build() {
        for (int i = 1; i < mLogN; i++) {
            for (int j = 0; j < mN; j++) {
                int pp = mP[i - 1][j];
                mP[i][j] = pp < 0 ? pp : mP[i - 1][pp];
            }
        }
    }

    // O(logD)
    int climb(int x, int dist) const {
        if (dist <= 0)
            return x;

        for (int i = 0; dist > 0; i++) {
            if (dist & 1)
                x = mP[i][x];
            dist >>= 1;
        }

        return x;
    }

    // O(logN)
    int lca(int A, int B) const {
        if (mLevel[A] < mLevel[B])
            swap(A, B);

        A = climb(A, mLevel[A] - mLevel[B]);

        if (A == B)
            return A;

        int bitCnt = 0;
        for (int x = mLevel[A]; x; x >>= 1)
            bitCnt++;

        for (int i = bitCnt - 1; i >= 0; i--) {
            if (mP[i][A] > 0 && mP[i][A] != mP[i][B]) {
                A = mP[i][A];
                B = mP[i][B];
            }
        }

        return mP[0][A];
    }
};
