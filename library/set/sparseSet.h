#pragma once

struct SparseSet {
    int maxValue;
    vector<int> sparse;
    vector<int> dense;

    explicit SparseSet(int maxVal, int cap = 0) : sparse(maxVal + 1) {
        dense.reserve(cap);
        maxValue = maxVal;
    }

    bool exist(int x) const {
        return (0 <= x) && (x <= maxValue) && (sparse[x] < int(dense.size())) && (dense[sparse[x]] == x);
    }

    pair<int,bool> find(int x) const {
        if ((0 <= x) && (x <= maxValue) && (sparse[x] < int(dense.size())) && (dense[sparse[x]] == x))
            return make_pair(sparse[x], true);
        return make_pair(-1, false);
    }

    bool set(int x) {
        if (x < 0 || x > maxValue || exist(x))
            return false;

        sparse[x] = int(dense.size());
        dense.push_back(x);

        return true;
    }

    bool reset(int x) {
        if (!exist(x))
            return false;

        int lastX = dense.back();
        dense[sparse[x]] = lastX;
        sparse[lastX] = sparse[x];

        dense.pop_back();

        return true;
    }

    void clear() {
        dense.clear();
    }


    SparseSet operator &(const SparseSet& rhs) const {
        SparseSet res(max(maxValue, rhs.maxValue), int(min(dense.capacity(), rhs.dense.capacity())));
        if (dense.size() < rhs.dense.size()) {
            for (auto x : dense) {
                if (rhs.exist(x))
                    res.set(x);
            }
        } else {
            for (auto x : rhs.dense) {
                if (exist(x))
                    res.set(x);
            }
        }
        return res;
    }

    SparseSet operator |(const SparseSet& rhs) const {
        SparseSet res(max(maxValue, rhs.maxValue), int(dense.size() + rhs.dense.size()));

        for (auto x : dense)
            res.set(x);

        for (auto x : rhs.dense)
            res.set(x);

        return res;
    }


    vector<int> toVector() const {
        vector<int> res(dense);
        sort(res.begin(), res.end());
        return res;
    }
};
