#pragma once

// partially persistent union-Find for Disjoint-Set
struct PersistentUnionFind {
    int N;
    vector<map<int,int>> parent;    // parent[x] = { (time, value), ... }
    int currTime;

    PersistentUnionFind() {
    }

    explicit PersistentUnionFind(int n) {
        init(n);
    }

    // time 0 : initial value
    void init(int n) {
        this->N = n;
        parent = vector<map<int,int>>(n);
        for (int i = 0; i < N; i++)
            parent[i][0] = i;
        currTime = 1;
    }

    //---

    int getCurrTime() const {
        return currTime;
    }

    int upgradeTime() {
        return currTime++;
    }

    // O(logN)
    int find(int x) {
        int currY = getLast(x);
        if (currY == x)
            return x;

        int nextY = find(currY);
        if (nextY != currY)
            set(x, nextY);
        return nextY;
    }

    // O(logN)
    int merge(int x, int y) {
        int xset = find(x);
        int yset = find(y);
        if (xset != yset)
            set(yset, xset);
        return xset;
    }

    //---

    // O(logN)
    int find(int time, int x) {
        int currY = get(time, x);
        if (currY == x)
            return x;

        int nextY = find(time, currY);
        if (nextY != currY)
            set(time, x, nextY);
        return nextY;
    }

private:
    // O(logN)
    // 0 <= time (time 0 is initial state), 0 <= index < N
    int get(int time, int index) const {
        auto it = parent[index].upper_bound(time);
        --it;
        return it->second;
    }

    // O(logN)
    void set(int time, int index, int val) {
        parent[index][time] = val;
    }


    // O(1)
    int getLast(int index) const {
        return parent[index].rbegin()->second;
    }

    // O(1)
    void set(int index, int val) {
        parent[index].emplace_hint(parent[index].end(), currTime, val);
    }
};
