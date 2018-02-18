#pragma once

class UnionFindMap {
public:
    UnionFindMap() : mElemMap() {
    }

    void init() {
        mElemMap.clear();
    }

    void add(int x) {
        mElemMap[x] = pair<int, int>(x, 0);
    }

    bool exist(int x) const {
        return mElemMap.find(x) != mElemMap.end();
    }

    int findSet(int x) {
        if (!exist(x)) {
            add(x);
            return x;
        }

        if (mElemMap[x].first == x)
            return x;
        return mElemMap[x].first = findSet(mElemMap[x].first);
    }

    int unionSet(int x, int y) {
        int xset = findSet(x);
        int yset = findSet(y);
        if (xset == yset)
            return xset;

        if (mElemMap[xset].second < mElemMap[yset].second) {
            mElemMap[xset].first = yset;
            return yset;
        } else {
            mElemMap[yset].first = xset;
            if (mElemMap[xset].second == mElemMap[yset].second)
                mElemMap[xset].second++;
            return xset;
        }
    }

private:
    // first is 'parent', second is 'rank'
    unordered_map<int, pair<int, int>> mElemMap;
};
