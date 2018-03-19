#pragma once

class UnionFindMap {
public:
    UnionFindMap() : elemMap() {
    }

    void init() {
        elemMap.clear();
    }

    void add(int x) {
        elemMap[x] = pair<int, int>(x, 0);
    }

    bool exist(int x) const {
        return elemMap.find(x) != elemMap.end();
    }

    int findSet(int x) {
        if (!exist(x)) {
            add(x);
            return x;
        }

        if (elemMap[x].first == x)
            return x;
        return elemMap[x].first = findSet(elemMap[x].first);
    }

    int unionSet(int x, int y) {
        int xset = findSet(x);
        int yset = findSet(y);
        if (xset == yset)
            return xset;

        if (elemMap[xset].second < elemMap[yset].second) {
            elemMap[xset].first = yset;
            return yset;
        } else {
            elemMap[yset].first = xset;
            if (elemMap[xset].second == elemMap[yset].second)
                elemMap[xset].second++;
            return xset;
        }
    }

private:
    // first is 'parent', second is 'rank'
    unordered_map<int, pair<int, int>> elemMap;
};
