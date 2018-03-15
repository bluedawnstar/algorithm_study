#pragma once

class UnionFind {
public:
    UnionFind() {
    }

    UnionFind(int N) : mParent(N, 0), mRank(N, 0) {
        for (int i = 0; i < N; i++)
            mParent[i] = i;
    }

    void init(int N) {
        mParent.resize(N);
        mRank.assign(N, 0);
        for (int i = 0; i < N; i++)
            mParent[i] = i;
    }


    int findSet(int x) {
        if (mParent[x] == x)
            return x;
        return mParent[x] = findSet(mParent[x]);
    }

    int unionSet(int x, int y) {
        int xset = findSet(x);
        int yset = findSet(y);
        if (xset == yset)
            return xset;

        if (mRank[xset] < mRank[yset]) {
            mParent[xset] = yset;
            return yset;
        } else {
            mParent[yset] = xset;
            if (mRank[xset] == mRank[yset])
                mRank[xset]++;
            return xset;
        }
    }

private:
    vector<int> mParent;
    vector<int> mRank;
};
