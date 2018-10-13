#pragma once

// The first 'node' number is 1, not 0
// Others('left', 'right', 'nodeLeft', 'nodeRight', 'index') are started from 0
template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct PartiallyPersistentSegmentTreeLazy {
    struct PartiallyPersistentArray {
        int N;
        vector<vector<int>> times;
        vector<vector<T>> values;

        PartiallyPersistentArray() {
        }

        // time 0 : initial value
        PartiallyPersistentArray(int n, T initValue) {
            init(n, initValue);
        }

        void init(int n, T initValue) {
            N = n;
            times.assign(n, vector<int>(1));
            values.assign(n, vector<T>(1, initValue));
        }

        // O(logN)
        // 0 <= time (time 0 is initial state), 0 <= index < N
        const T& get(int time, int index) const {
            int i = int(upper_bound(times[index].begin(), times[index].end(), time) - times[index].begin());
            return values[index][i - 1];
        }

        const T& getLast(int index) const {
            return values[index].back();
        }

        // O(1)
        // return current time
        void set(int time, int index, const T& val) {
            if (times[index].back() == time)
                values[index].back() = val;
            else {
                times[index].push_back(time);
                values[index].push_back(val);
            }
        }
    };

    struct Node {
        int     id;
        T       value;
        int     left;
        int     right;
    };

    int             N;              // the size of array
    vector<Node>    nodes;          // 
    vector<int>     trees;          // roots
    PartiallyPersistentArray treesLazy;

    T               defaultValue;
    MergeOp         mergeOp;
    BlockOp         blockOp;

    PartiallyPersistentSegmentTreeLazy(MergeOp mop, BlockOp bop, T dflt = T())
        : N(0), nodes(), trees(), treesLazy(), defaultValue(dflt), mergeOp(mop), blockOp(bop) {
    }

    PartiallyPersistentSegmentTreeLazy(T value, int n, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(value, n);
    }

    PartiallyPersistentSegmentTreeLazy(const T arr[], int n, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(arr, n);
    }

    PartiallyPersistentSegmentTreeLazy(const vector<T>& v, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(v);
    }


    int getHistorySize() const {
        return int(trees.size());
    }


    // O(NlogN)
    T build(T value, int n) {
        init(n);

        auto t = initSub(value, 0, n - 1, 1);
        trees.push_back(t.second);

        return t.first;
    }

    // O(NlogN)
    T build(const T arr[], int n) {
        init(n);

        auto t = buildSub(arr, 0, n - 1, 1);
        trees.push_back(t.second);

        return t.first;
    }

    // O(NlogN)
    T build(const vector<T>& v) {
        return build(&v[0], int(v.size()));
    }

    //--- update (= upgrade)

    // inclusive, O(logN)
    T update(int index, T newValue) {
        auto t = updateSub(trees.back(), index, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
    }

    // inclusive, O(klogN)
    T updateRange(int left, int right, T newValue) {
        auto t = updateRangeSub(trees.back(), left, right, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
    }

    //--- query

    // inclusive, O(logN)
    T query(int left, int right) {
        return querySub(int(trees.size()) - 1, trees.back(), left, right, 0, N - 1);
    }

    // inclusive, O(logN)
    T query(int historyIndex, int left, int right) {
        return querySub(historyIndex, trees[historyIndex], left, right, 0, N - 1);
    }

    // PRECONDITION: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
    // lower bound where f(x) is true in [0, N)
    //   f(x): xxxxxxxxxxxOOOOOOOO
    //         S          ^
    // O(logN)
    int lowerBound(const function<bool(T)>& f) {
        return lowerBoundSub(int(trees.size()) - 1, f, T(0), trees.back(), 0, N - 1);
    }

    // PRECONDITION: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
    // lower bound where f(x) is true in [0, N)
    //   f(x): xxxxxxxxxxxOOOOOOOO
    //         S          ^
    // O(logN)
    int lowerBound(int historyIndex, const function<bool(T)>& f) {
        return lowerBoundSub(historyIndex, f, T(0), trees[historyIndex], 0, N - 1);
    }

private:
    void init(int n) {
        N = n;
        trees.clear();
        trees.reserve(N);

        nodes.clear();
        nodes.reserve(N * 4);

        int lazyN = n * 4;
        if (lazyN & (lazyN - 1)) {
            lazyN <<= 1;
            while (lazyN & (lazyN - 1))
                lazyN &= lazyN - 1;
        }

        treesLazy.init(lazyN, defaultValue);
        //treesLazy.init(N * 4, defaultValue);
    }

    int addNode(int id, T value) {
        int i = int(nodes.size());
        nodes.push_back({ id, value, -1, -1 });
        return i;
    }

    int addNode(int id, T value, int left, int right) {
        int i = int(nodes.size());
        nodes.push_back({ id, value, left, right });
        return i;
    }

    // inclusive
    pair<T, int> initSub(T initValue, int left, int right, int node) {
        if (left > right)
            return make_pair(defaultValue, -1);

        if (left == right)
            return make_pair(initValue, addNode(node, initValue));

        int mid = left + (right - left) / 2;
        auto L = initSub<U>(initValue, left, mid, node * 2);
        auto R = initSub<U>(initValue, mid + 1, right, node * 2 + 1);
        auto value = mergeOp(L.first, R.first);
        return make_pair(value, addNode(node, value, L.second, R.second));
    }

    // inclusive
    pair<T, int> buildSub(const T arr[], int left, int right, int node) {
        if (left > right)
            return make_pair(defaultValue, -1);

        if (left == right)
            return make_pair(arr[left], addNode(node, arr[left]));

        int mid = left + (right - left) / 2;
        auto L = buildSub(arr, left, mid, node * 2);
        auto R = buildSub(arr, mid + 1, right, node * 2 + 1);
        auto value = mergeOp(L.first, R.first);
        return make_pair(value, addNode(node, value, L.second, R.second));
    }

    T querySub(int historyIndex, int node, int left, int right, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return defaultValue;

        if (left <= nodeLeft && nodeRight <= right)
            return nodes[node].value;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;

        int currTime = int(trees.size());
        auto lazy = treesLazy.get(currTime - 1, nodes[node].id);
        if (lazy != defaultValue) {
            pushDown(nodes[node].left, lazy, nodeLeft, mid);
            pushDown(nodes[node].right, lazy, mid + 1, nodeRight);
            treesLazy.set(currTime, nodes[node].id, defaultValue);
        }
        return mergeOp(querySub(historyIndex, nodes[node].left, left, right, nodeLeft, mid),
                       querySub(historyIndex, nodes[node].right, left, right, mid + 1, nodeRight));
    }

    pair<T, int> updateSub(int node, int index, T newValue, int nodeLeft, int nodeRight) {
        if (index < nodeLeft || nodeRight < index)
            return node >= 0 ? make_pair(nodes[node].value, node) : make_pair(defaultValue, -1);

        if (nodeLeft == nodeRight)
            return make_pair(newValue, addNode(nodes[node].id, newValue, nodes[node].left, nodes[node].right));

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;

        int currTime = int(trees.size());
        auto lazy = treesLazy.get(currTime - 1, nodes[node].id);
        if (lazy != defaultValue) {
            auto L = pushDown(nodes[node].left, lazy, nodeLeft, mid);
            auto R = pushDown(nodes[node].right, lazy, mid + 1, nodeRight);
            node = addNode(nodes[node].id, nodes[node].value, L.second, R.second);
            treesLazy.set(currTime, nodes[node].id, defaultValue);
        }

        auto L = updateSub(nodes[node].left, index, newValue, nodeLeft, mid);
        auto R = updateSub(nodes[node].right, index, newValue, mid + 1, nodeRight);
        T value = mergeOp(L.first, R.first);
        return make_pair(value, addNode(nodes[node].id, value, L.second, R.second));
    }

    pair<T, int> updateRangeSub(int node, int left, int right, T newValue, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return node >= 0 ? make_pair(nodes[node].value, node) : make_pair(defaultValue, -1);

        if (nodeLeft == nodeRight)
            return make_pair(newValue, addNode(nodes[node].id, newValue, nodes[node].left, nodes[node].right));

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;

        int currTime = int(trees.size());
        auto lazy = treesLazy.get(currTime - 1, nodes[node].id);
        if (lazy != defaultValue) {
            auto L = pushDown(nodes[node].left, lazy, nodeLeft, mid);
            auto R = pushDown(nodes[node].right, lazy, mid + 1, nodeRight);
            node = addNode(nodes[node].id, nodes[node].value, L.second, R.second);
            treesLazy.set(currTime, nodes[node].id, defaultValue);
        }

        if (left <= nodeLeft && nodeRight <= right) {
            treesLazy.set(currTime, nodes[node].id, newValue);
            T value = blockOp(newValue, nodeRight - nodeLeft + 1);
            return make_pair(value, addNode(nodes[node].id, value, nodes[node].left, nodes[node].right));
        }

        auto L = updateRangeSub(nodes[node].left, left, right, newValue, nodeLeft, mid);
        auto R = updateRangeSub(nodes[node].right, left, right, newValue, mid + 1, nodeRight);
        T value = mergeOp(L.first, R.first);
        return make_pair(value, addNode(nodes[node].id, value, L.second, R.second));
    }

    pair<T, int> pushDown(int p, T newValue, int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight)
            return make_pair(newValue, addNode(nodes[p].id, newValue, nodes[p].left, nodes[p].right));

        treesLazy.set(int(trees.size()), nodes[p].id, newValue);
        T value = blockOp(newValue, nodeRight - nodeLeft + 1);
        return make_pair(value, addNode(nodes[p].id, value, nodes[p].left, nodes[p].right));
    }

    int lowerBoundSub(int historyIndex, const function<bool(T)>& f, T delta, int node, int nodeLeft, int nodeRight) {
        if (nodeLeft >= nodeRight)
            return nodeLeft;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;

        int currTime = int(trees.size());
        auto lazy = treesLazy.get(currTime - 1, nodes[node].id);
        if (lazy != defaultValue) {
            pushDown(nodes[node].left, lazy, nodeLeft, mid);
            pushDown(nodes[node].right, lazy, mid + 1, nodeRight);
            treesLazy.set(currTime, nodes[node].id, defaultValue);
        }

        auto val = mergeOp(delta, nodes[nodes[node].left].value);
        if (f(val))
            return lowerBoundSub(historyIndex, f, delta, nodes[node].left, nodeLeft, mid);
        else
            return lowerBoundSub(historyIndex, f, val, nodes[node].right, mid + 1, nodeRight);
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline PartiallyPersistentSegmentTreeLazy<T, MergeOp, BlockOp> makePartiallyPersistentSegmentTreeLazy(MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return PartiallyPersistentSegmentTreeLazy<T, MergeOp, BlockOp>(mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline PartiallyPersistentSegmentTreeLazy<T, MergeOp, BlockOp> makePartiallyPersistentSegmentTreeLazy(const vector<T>& v, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return PartiallyPersistentSegmentTreeLazy<T, MergeOp, BlockOp>(v, mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline PartiallyPersistentSegmentTreeLazy<T, MergeOp, BlockOp> makePartiallyPersistentSegmentTreeLazy(const T arr[], int size, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return PartiallyPersistentSegmentTreeLazy<T, MergeOp, BlockOp>(arr, size, mop, bop, dfltValue);
}
