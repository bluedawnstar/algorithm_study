#pragma once

// PersistentSegmentTreeLazy supports full persistent functions,
//  but the upgrade functions is very slower than PartiallyPersistentSegmentTreeLazy.
//
// It is available when queries and updates are significantly more than upgrades.


// The first 'node' number is 1, not 0
// Others('left', 'right', 'nodeLeft', 'nodeRight', 'index') are started from 0
template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct PersistentSegmentTreeLazy {
    struct Node {
        int     id;
        T       value;
        int     left;
        int     right;
    };

    int             N;              // the size of array
    vector<Node>    nodes;
    vector<int>     trees;          // roots
    vector<unordered_map<int, T>> treesLazy;

    T               defaultValue;
    MergeOp         mergeOp;
    BlockOp         blockOp;

    PersistentSegmentTreeLazy(MergeOp mop, BlockOp bop, T dflt = T())
        : N(0), nodes(), trees(), treesLazy(), defaultValue(dflt), mergeOp(mop), blockOp(bop) {
    }

    PersistentSegmentTreeLazy(T value, int n, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(value, n);
    }

    PersistentSegmentTreeLazy(const T arr[], int n, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(arr, n);
    }

    PersistentSegmentTreeLazy(const vector<T>& v, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(v);
    }


    int getHistorySize() const {
        return int(trees.size());
    }


    // O(NlogN)
    T build(T value, int n) {
        init(n);

        treesLazy.resize(1);
        auto t = initSub(value, 0, n - 1, 1);
        trees.push_back(t.second);

        return t.first;
    }

    // O(NlogN)
    T build(const T arr[], int n) {
        init(n);

        treesLazy.resize(1);
        auto t = buildSub(arr, 0, n - 1, 1);
        trees.push_back(t.second);

        return t.first;
    }

    // O(NlogN)
    T build(const vector<T>& v) {
        return build(&v[0], int(v.size()));
    }


    // inclusive, O(logN)
    T query(int left, int right) {
        return querySub(int(treesLazy.size()) - 1, trees.back(), left, right, 0, N - 1);
    }

    // inclusive, O(logN)
    T update(int index, T newValue) {
        return updateSub(int(treesLazy.size()) - 1, trees.back(), index, newValue, 0, N - 1);
    }

    // inclusive, O(klogN)
    T updateRange(int left, int right, T newValue) {
        return updateRangeSub(int(treesLazy.size()) - 1, trees.back(), left, right, newValue, 0, N - 1);
    }

    // inclusive, O(logN)
    T upgrade(int index, T newValue) {
        treesLazy.push_back(treesLazy.back());
        auto t = upgradeSub(int(treesLazy.size()) - 1, trees.back(), index, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
    }

    // inclusive, O(klogN)
    T upgradeRange(int left, int right, T newValue) {
        treesLazy.push_back(treesLazy.back());
        auto t = upgradeRangeSub(int(treesLazy.size()) - 1, trees.back(), left, right, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
    }

    // PRECONDITION: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
    // lower bound where f(x) is true in [0, N)
    //   f(x): xxxxxxxxxxxOOOOOOOO
    //         S          ^
    // O(logN)
    int lowerBound(const function<bool(T)>& f) {
        return lowerBoundSub(int(treesLazy.size()) - 1, f, T(0), trees.back(), 0, N - 1);
    }

    //--- with history ---

    // inclusive, O(logN)
    T query(int historyIndex, int left, int right) {
        return querySub(historyIndex, trees[historyIndex], left, right, 0, N - 1);
    }

    // inclusive, O(logN)
    T update(int historyIndex, int index, T newValue) {
        return updateSub(historyIndex, trees[historyIndex], index, newValue, 0, N - 1);
    }

    // inclusive, O(klogN)
    T updateRange(int historyIndex, int left, int right, T newValue) {
        return updateRangeSub(historyIndex, trees[historyIndex], left, right, newValue, 0, N - 1);
    }

    // inclusive, O(logN)
    T upgrade(int historyIndex, int index, T newValue) {
        treesLazy.push_back(treesLazy[historyIndex]);
        auto t = upgradeSub(int(treesLazy.size()) - 1, trees[historyIndex], index, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
    }

    // inclusive, O(klogN)
    T upgradeRange(int historyIndex, int left, int right, T newValue) {
        treesLazy.push_back(treesLazy[historyIndex]);
        auto t = upgradeeRangeSub(int(treesLazy.size()) - 1, trees[historyIndex], left, right, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
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
        treesLazy.clear();
        trees.reserve(N);

        nodes.clear();
        nodes.reserve(N * 4);
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

        auto it = treesLazy[historyIndex].find(nodes[node].id);
        if (it != treesLazy[historyIndex].end()) {
            pushDown(historyIndex, nodes[node].left, it->second, nodeLeft, mid);
            pushDown(historyIndex, nodes[node].right, it->second, mid + 1, nodeRight);
            treesLazy[historyIndex].erase(it);
        }
        return mergeOp(querySub(historyIndex, nodes[node].left, left, right, nodeLeft, mid),
                       querySub(historyIndex, nodes[node].right, left, right, mid + 1, nodeRight));
    }

    T updateSub(int historyIndex, int node, int index, T newValue, int nodeLeft, int nodeRight) {
        if (index < nodeLeft || nodeRight < index)
            return node >= 0 ? nodes[node].value : defaultValue;

        if (nodeLeft == nodeRight)
            return nodes[node].value = newValue;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;

        auto it = treesLazy[historyIndex].find(nodes[node].id);
        if (it != treesLazy[historyIndex].end()) {
            pushDown(historyIndex, nodes[node].left, it->second, nodeLeft, mid);
            pushDown(historyIndex, nodes[node].right, it->second, mid + 1, nodeRight);
            treesLazy[historyIndex].erase(it);
        }
        return nodes[p].value = mergeOp(updateSub(historyIndex, nodes[node].left, index, newValue, nodeLeft, mid),
                                        updateSub(historyIndex, nodes[node].right, index, newValue, mid + 1, nodeRight));
    }

    T updateRangeSub(int historyIndex, int node, int left, int right, T newValue, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return node >= 0 ? nodes[node].value : defaultValue;

        if (nodeLeft == nodeRight)
            return nodes[node].value = newValue;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;

        auto it = treesLazy[historyIndex].find(nodes[node].id);
        if (it != treesLazy[historyIndex].end()) {
            pushDown(historyIndex, nodes[node].left, it->second, nodeLeft, mid);
            pushDown(historyIndex, nodes[node].right, it->second, mid + 1, nodeRight);
            treesLazy[historyIndex].erase(it);
        }

        if (left <= nodeLeft && nodeRight <= right) {
            treesLazy[historyIndex][nodes[node].id] = newValue;
            return nodes[node].value = blockOp(newValue, nodeRight - nodeLeft + 1);
        }

        return nodes[node].value = mergeOp(updateRangeSub(historyIndex, nodes[node].left, left, right, newValue, nodeLeft, mid),
                                           updateRangeSub(historyIndex, nodes[node].right, left, right, newValue, mid + 1, nodeRight));
    }

    pair<T, int> upgradeSub(int historyIndex, int node, int index, T newValue, int nodeLeft, int nodeRight) {
        if (index < nodeLeft || nodeRight < index)
            return make_pair(node >= 0 ? nodes[node].value : defaultValue, node);

        if (nodeLeft == nodeRight)
            return make_pair(newValue, addNode(nodes[node].id, newValue, nodes[node].left, nodes[node].right));

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;

        auto it = treesLazy[historyIndex].find(nodes[node].id);
        if (it != treesLazy[historyIndex].end()) {
            auto L = pushDownUpgrade(historyIndex, nodes[node].left, it->second, nodeLeft, mid);
            auto R = pushDownUpgrade(historyIndex, nodes[node].right, it->second, mid + 1, nodeRight);
            node = addNode(nodes[node].id, nodes[node].value, L.second, R.second);
            treesLazy[historyIndex].erase(it);
        }

        auto L = upgradeSub(historyIndex, nodes[node].left, index, newValue, nodeLeft, mid);
        auto R = upgradeSub(historyIndex, nodes[node].right, index, newValue, mid + 1, nodeRight);
        T value = mergeOp(L.first, R.first);
        return make_pair(value, addNode(nodes[node].id, value, L.second, R.second));
    }

    pair<T, int> upgradeRangeSub(int historyIndex, int node, int left, int right, T newValue, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return make_pair(node >= 0 ? nodes[node].value : defaultValue, node);

        if (nodeLeft == nodeRight)
            return make_pair(newValue, addNode(nodes[node].id, newValue, nodes[node].left, nodes[node].right));

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;

        auto it = treesLazy[historyIndex].find(nodes[node].id);
        if (it != treesLazy[historyIndex].end()) {
            auto L = pushDownUpgrade(historyIndex, nodes[node].left, it->second, nodeLeft, mid);
            auto R = pushDownUpgrade(historyIndex, nodes[node].right, it->second, mid + 1, nodeRight);
            node = addNode(nodes[node].id, nodes[node].value, L.second, R.second);
            treesLazy[historyIndex].erase(it);
        }

        if (left <= nodeLeft && nodeRight <= right) {
            treesLazy[historyIndex][nodes[node].id] = newValue;
            T value = blockOp(newValue, nodeRight - nodeLeft + 1);
            return make_pair(value, addNode(nodes[node].id, value, nodes[node].left, nodes[node].right));
        }

        auto L = upgradeRangeSub(historyIndex, nodes[node].left, left, right, newValue, nodeLeft, mid);
        auto R = upgradeRangeSub(historyIndex, nodes[node].right, left, right, newValue, mid + 1, nodeRight);
        T value = mergeOp(L.first, R.first);
        return make_pair(value, addNode(nodes[node].id, value, L.second, R.second));
    }

    T pushDown(int historyIndex, int p, T newValue, int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight)
            return nodes[p].value = newValue;

        treesLazy[historyIndex][nodes[p].id] = newValue;
        return nodes[p].value = blockOp(newValue, nodeRight - nodeLeft + 1);
    }

    pair<T, int> pushDownUpgrade(int historyIndex, int p, T newValue, int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight)
            return make_pair(newValue, addNode(nodes[p].id, newValue, nodes[p].left, nodes[p].right));

        treesLazy[historyIndex][nodes[p].id] = newValue;
        T value = blockOp(newValue, nodeRight - nodeLeft + 1);
        return make_pair(value, addNode(nodes[p].id, value, nodes[p].left, nodes[p].right));
    }


    int lowerBoundSub(int historyIndex, const function<bool(T)>& f, T delta, int node, int nodeLeft, int nodeRight) {
        if (nodeLeft >= nodeRight)
            return nodeLeft;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;

        auto it = treesLazy[historyIndex].find(nodes[node].id);
        if (it != treesLazy[historyIndex].end()) {
            pushDown(historyIndex, nodes[node].left, it->second, nodeLeft, mid);
            pushDown(historyIndex, nodes[node].right, it->second, mid + 1, nodeRight);
            treesLazy[historyIndex].erase(it);
        }

        auto val = mergeOp(delta, nodes[nodes[node].left].value);
        if (f(val))
            return lowerBoundSub(historyIndex, f, delta, nodes[node].left, nodeLeft, mid);
        else
            return lowerBoundSub(historyIndex, f, val, nodes[node].right, mid + 1, nodeRight);
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline PersistentSegmentTreeLazy<T, MergeOp, BlockOp> makePersistentSegmentTreeLazy(MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return PersistentSegmentTreeLazy<T, MergeOp, BlockOp>(mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline PersistentSegmentTreeLazy<T, MergeOp, BlockOp> makePersistentSegmentTreeLazy(const vector<T>& v, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return PersistentSegmentTreeLazy<T, MergeOp, BlockOp>(v, mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline PersistentSegmentTreeLazy<T, MergeOp, BlockOp> makePersistentSegmentTreeLazy(const T arr[], int size, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return PersistentSegmentTreeLazy<T, MergeOp, BlockOp>(arr, size, mop, bop, dfltValue);
}
