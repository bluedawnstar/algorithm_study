#pragma once

template <typename T>
struct UnionFindWithValues {
    vector<int> parent;
    vector<vector<T>> values;

    UnionFindWithValues() {
    }

    explicit UnionFindWithValues(int n) {
        init(n);
    }

    void init(int n) {
        values.resize(n);
        parent.resize(n);
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] == x)
            return x;
        return parent[x] = find(parent[x]);
    }

    int merge(int u, int v) {
        u = find(u);
        v = find(v);
        if (values[u].size() > values[v].size())
            swap(u, v);
        parent[u] = v;

        values[v].insert(values[v].end(), values[u].begin(), values[u].end());
        values[u].clear();

        return v;
    }

    template <typename MergeOp = function<void(int, int)>>
    int merge(int u, int v, const MergeOp& customMergeOp) {
        u = find(u);
        v = find(v);
        if (values[u].size() > values[v].size())
            swap(u, v);
        parent[u] = v;

        customMergeOp(v, u);    // merge u to v

        values[v].insert(values[v].end(), values[u].begin(), values[u].end());
        values[u].clear();

        return v;
    }
};
