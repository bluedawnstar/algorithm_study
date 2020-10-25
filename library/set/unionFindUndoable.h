#pragma once

#pragma once

// undoable union-Find for Disjoint-Set
struct UndoableUnionFind {
    vector<int> parent;

    vector<pair<int,int>> history;  // { (index, prev-value), ... }
    vector<int> historySize;

    UndoableUnionFind() {
    }

    explicit UndoableUnionFind(int n) {
        init(n);
    }

    // time 0 : initial value
    void init(int n) {
        parent.resize(n);
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    //---

    int find(int x) {
        if (parent[x] == x)
            return x;

        int xset = find(parent[x]);
        if (parent[x] != xset)
            set(x, xset);
        return xset;
    }

    int merge(int x, int y) {
        int xset = find(x);
        int yset = find(y);
        if (xset != yset)
            set(yset, xset);
        return xset;
    }

    //---

    int getCurrTime() const {
        return int(history.size());
    }

    void commit() {
        historySize.push_back(int(history.size()));
    }

    void undo() {
        if (historySize.empty())
            return;

        historySize.pop_back();
        int size = historySize.empty() ? int(history.size()) : (int(history.size()) - historySize.back());

        while (size-- > 0) {
            parent[history.back().first] = history.back().second;
            history.pop_back();
        }
    }

private:
    void set(int index, int val) {
        history.emplace_back(index, parent[index]);
        parent[index] = val;
    }
};
