#pragma once

#include <vector>
#include <functional>

//--------- Simple Persistent Segment Tree ------------------------------------

template <typename T, typename MergeOp = function<T(T, T)>>
struct PersistentSegmentTree {
    struct Node {
        T       value;
        int     left;
        int     right;
    };

    int         N;              // the size of array
    vector<Node> nodes;         // 
    vector<int> trees;          // roots

    MergeOp     mergeOp;        // 
    T           defaultValue;   // 

    explicit PersistentSegmentTree(MergeOp op, T dflt = T())
        : N(0), nodeN(0), mergeOp(op), defaultValue(dflt) {
    }

    PersistentSegmentTree(T value, int n, MergeOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(value, n);
    }

    PersistentSegmentTree(const T arr[], int n, MergeOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(arr, n);
    }

    PersistentSegmentTree(const vector<T>& v, MergeOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(v);
    }


    int getHistorySize() const {
        return int(trees.size());
    }


    // O(N)
    T build(T value, int n) {
        init(n);

        auto t = buildSub(value, 0, N - 1);
        trees.push_back(t.second);

        return t.first;
    }

    // O(N)
    T build(const T arr[], int n) {
        init(n);

        auto t = buildSub(arr, 0, n - 1);
        trees.push_back(t.second);
        return t.first;
    }

    // O(N)
    T build(const vector<T>& v) {
        return build(&v[0], int(v.size()));
    }


    // inclusive, O(logN)
    T query(int left, int right) const {
        return querySub(trees.back(), left, right, 0, N - 1);
    }

    // inclusive, O(logN)
    T update(int index, T newValue) {
        return updateSub(trees.back(), index, newValue, 0, N - 1);
    }

    // inclusive, O(|right - left| + logN)
    T updateRange(int left, int right, T newValue) {
        return updateRangeSub(trees.back(), left, right, newValue, 0, N - 1);
    }

    // inclusive, O(logN)
    T upgrade(int index, T newValue) {
        auto t = upgradeSub(trees.back(), index, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
    }

    // inclusive, O(|right - left| + logN)
    T upgradeRange(int left, int right, T newValue) {
        auto t = upgradeRangeSub(trees.back(), left, right, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
    }

    // PRECONDITION: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
    // lower bound where f(x) is true in [0, N)
    //   f(x): xxxxxxxxxxxOOOOOOOO
    //         S          ^
    // O(logN)
    int lowerBound(const function<bool(T)>& f) const {
        return lowerBoundSub(f, defaultValue, trees.back(), 0, N - 1);
    }

    //--- with history ---

    // inclusive, O(logN)
    T query(int historyIndex, int left, int right) const {
        return querySub(trees[historyIndex], left, right, 0, N - 1);
    }

    // inclusive, O(logN)
    T update(int historyIndex, int index, T newValue) {
        return updateSub(trees[historyIndex], index, newValue, 0, N - 1);
    }

    // inclusive, O(|right - left| + logN)
    T updateRange(int historyIndex, int left, int right, T newValue) {
        return updateRangeSub(trees[historyIndex], left, right, newValue, 0, N - 1);
    }

    // inclusive, O(logN)
    T upgrade(int historyIndex, int index, T newValue) {
        auto t = upgradeSub(trees[historyIndex], index, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
    }

    // inclusive, O(|right - left| + logN)
    T upgradeRange(int historyIndex, int left, int right, T newValue) {
        auto t = upgradeRangeSub(trees[historyIndex], left, right, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
    }

    // PRECONDITION: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
    // lower bound where f(x) is true in [0, N)
    //   f(x): xxxxxxxxxxxOOOOOOOO
    //         S          ^
    // O(logN)
    int lowerBound(int historyIndex, const function<bool(T)>& f) const {
        return lowerBoundSub(f, defaultValue, trees[historyIndex], 0, N - 1);
    }

private:
    void init(int n) {
        N = n;
        trees.clear();
        trees.reserve(N);

        nodes.clear();
        nodes.reserve(N * 4);
    }

    int addNode(T value) {
        int i = int(nodes.size());
        nodes.push_back({ value, -1, -1 });
        return i;
    }

    int addNode(T value, int left, int right) {
        int i = int(nodes.size());
        nodes.push_back({ value, left, right });
        return i;
    }


    // inclusive
    pair<T, int> buildSub(T initValue, int left, int right) {
        if (left > right)
            return make_pair(defaultValue, -1);

        if (left == right)
            return make_pair(initValue, addNode(initValue));

        int mid = left + (right - left) / 2;
        auto L = buildSub(initValue, left, mid);
        auto R = buildSub(initValue, mid + 1, right);
        auto value = mergeOp(L.first, R.first);
        return make_pair(value, addNode(value, L.second, R.second));
    }

    // inclusive
    pair<T, int> buildSub(const T arr[], int left, int right) {
        if (left > right)
            return make_pair(defaultValue, -1);

        if (left == right)
            return make_pair(arr[left], addNode(arr[left]));

        int mid = left + (right - left) / 2;
        auto L = buildSub(arr, left, mid);
        auto R = buildSub(arr, mid + 1, right);
        auto value = mergeOp(L.first, R.first);
        return make_pair(value, addNode(value, L.second, R.second));
    }

    T querySub(int node, int left, int right, int nodeLeft, int nodeRight) const {
        if (right < nodeLeft || nodeRight < left)
            return defaultValue;

        if (left <= nodeLeft && nodeRight <= right)
            return nodes[node].value;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return mergeOp(querySub(nodes[node].left, left, right, nodeLeft, mid),
                       querySub(nodes[node].right, left, right, mid + 1, nodeRight));
    }

    T updateSub(int node, int index, T newValue, int nodeLeft, int nodeRight) {
        if (index < nodeLeft || nodeRight < index)
            return node >= 0 ? nodes[node].value : defaultValue;

        if (nodeLeft == nodeRight)
            return nodes[node].value = newValue;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return nodes[node].value = mergeOp(updateSub(nodes[node].left, index, newValue, nodeLeft, mid),
                                           updateSub(nodes[node].right, index, newValue, mid + 1, nodeRight));
    }

    T updateRangeSub(int node, int left, int right, T newValue, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return node >= 0 ? nodes[node].value : defaultValue;

        if (nodeLeft == nodeRight)
            return nodes[node].value = newValue;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return nodes[node].value = mergeOp(updateRangeSub(nodes[node].left, left, right, newValue, nodeLeft, mid),
                                           updateRangeSub(nodes[node].right, left, right, newValue, mid + 1, nodeRight));
    }

    pair<T, int> upgradeSub(int node, int index, T newValue, int nodeLeft, int nodeRight) {
        if (index < nodeLeft || nodeRight < index)
            return make_pair(node >= 0 ? nodes[node].value : defaultValue, node);

        if (nodeLeft == nodeRight)
            return make_pair(newValue, addNode(newValue, nodes[node].left, nodes[node].right));

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        auto L = upgradeSub(nodes[node].left, index, newValue, nodeLeft, mid);
        auto R = upgradeSub(nodes[node].right, index, newValue, mid + 1, nodeRight);
        T value = mergeOp(L.first, R.first);
        return make_pair(value, addNode(value, L.second, R.second));
    }

    pair<T, int> upgradeRangeSub(int node, int left, int right, T newValue, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return make_pair(node >= 0 ? nodes[node].value : defaultValue, node);

        if (nodeLeft == nodeRight)
            return make_pair(newValue, addNode(newValue, nodes[node].left, nodes[node].right));

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        auto L = upgradeRangeSub(nodes[node].left, left, right, newValue, nodeLeft, mid);
        auto R = upgradeRangeSub(nodes[node].right, left, right, newValue, mid + 1, nodeRight);
        T value = mergeOp(L.first, R.first);
        return make_pair(value, addNode(value, L.second, R.second));
    }

    int lowerBoundSub(const function<bool(T)>& f, T delta, int node, int nodeLeft, int nodeRight) const {
        if (nodeLeft > nodeRight)
            return nodeLeft;

        if (nodeLeft == nodeRight)
            return nodeLeft + (f(mergeOp(delta, nodes[node].value)) ? 0 : 1);

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        auto val = mergeOp(delta, nodes[nodes[node].left].value);
        if (f(val))
            return lowerBoundSub(f, delta, nodes[node].left, nodeLeft, mid);
        else
            return lowerBoundSub(f, val, nodes[node].right, mid + 1, nodeRight);
    }
};

template <typename T, typename MergeOp>
inline PersistentSegmentTree<T, MergeOp> makePersistentSegmentTree(MergeOp op, T dfltValue = T()) {
    return PersistentSegmentTree<T, MergeOp>(op, dfltValue);
}

template <typename T, typename MergeOp>
inline PersistentSegmentTree<T, MergeOp> makePersistentSegmentTree(const vector<T>& v, MergeOp op, T dfltValue = T()) {
    return PersistentSegmentTree<T, MergeOp>(v, op, dfltValue);
}

template <typename T, typename MergeOp>
inline PersistentSegmentTree<T, MergeOp> makePersistentSegmentTree(const T arr[], int size, MergeOp op, T dfltValue = T()) {
    return PersistentSegmentTree<T, MergeOp>(arr, size, op, dfltValue);
}
