#pragma once

/*
<How to use>
    init() -> build() -> climb() / findLCA()
*/

struct SparseTableLCA {
    int                 mN;         // the number of vertex
    int                 mLogN;      // 
    vector<vector<int>> mP;         // mP[0][n] points to the parent
                                    // parent & acestors

    SparseTableLCA() {
        mN = 0;
        mLogN = 0;
    }

    SparseTableLCA(int N) {
        init(N);
    }

    SparseTableLCA(const vector<int>& A) {
        init(A);
    }

    SparseTableLCA(const int A[], int N) {
        init(A, N);
    }


    void init(int N) {
        mN = N;
        mLogN = (int)log2(N) + 1;
        mP.resize(mLogN);
        for (int i = 0; i < mLogN; i++)
            mP[i].resize(mN);
    }

    void init(const vector<int>& A) {
        init((int)A.size());
        copy(A.begin(), A.end(), mP[0].begin());
    }

    void init(const int A[], int N) {
        init(N);
        copy(A, A + N, mP[0].begin());
    }


    void build() {
        for (int i = 1; i < mLogN; i++) {
            for (int j = 0; j < mN; j++) {
                int pp = mP[i - 1][j];
                mP[i][j] = pp < 0 ? pp : mP[i - 1][pp];
            }
        }
    }


    int climb(int x, int dist) {
        if (dist <= 0)
            return x;

        for (int i = 0; dist > 0; i++) {
            if (dist & 1)
                x = mP[i][x];
            dist >>= 1;
        }

        return x;
    }

    // PRECONDITION: The depth from the root of A and B is the same.
    int findLCA(int A, int B) {
        if (A == B)
            return A;

        for (int i = mLogN - 1; i >= 0; i--) {
            if (mP[i][A] >= 0 && mP[i][A] != mP[i][B]) {
                A = mP[i][A];
                B = mP[i][B];
            }
        }

        return mP[0][A];
    }
};
