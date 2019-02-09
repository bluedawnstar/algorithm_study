#pragma once

struct UnionFindMap {
    // first is 'parent', second is 'rank'
    unordered_map<int, pair<int, int>> elemMap;

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

    int find(int x) {
        if (!exist(x)) {
            add(x);
            return x;
        }

        if (elemMap[x].first == x)
            return x;
        return elemMap[x].first = find(elemMap[x].first);
    }

    int merge(int x, int y) {
        int xset = find(x);
        int yset = find(y);
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
};

// Simplified Union-Find for Disjoint-Set
struct DSUMap {
    unordered_map<int, int> M;  // parent

    DSUMap() : M() {
    }

    void init() {
        M.clear();
    }

    void add(int x) {
        M[x] = x;
    }

    bool exist(int x) const {
        return M.find(x) != M.end();
    }

    int find(int x) {
        if (!exist(x)) {
            add(x);
            return x;
        }

        if (M[x] == x)
            return x;
        return M[x] = find(M[x]);
    }

    int merge(int x, int y) {
        int xset = find(x);
        int yset = find(y);
        if (xset != yset)
            M[yset] = xset;
        return xset;
    }
};
