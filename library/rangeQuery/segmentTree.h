#pragma once

#include <vector>
#include <functional>

//--------- Operations --------------------------------------------------------

template <typename T>
struct MaxOp {
    T operator()(T a, T b) const {
        return max(a, b);
    }
};

template <typename T>
struct MinOp {
    T operator()(T a, T b) const {
        return min(a, b);
    }
};

template <typename T>
struct SumOp {
    T operator()(T a, T b) const {
        return a + b;
    }
};

template <typename T>
struct GcdOp {
    T operator()(T a, T b) const {
        return gcd(a, b);
    }
};

template <typename T>
struct LcmOp {
    T operator()(T a, T b) const {
        return lcm(a, b);
    }
};

//--------- General Segment Tree ----------------------------------------------

// The first 'node' number is 1, not 0
// Others('left', 'right', 'nodeLeft', 'nodeRight', 'index') are started from 0
template <typename T, typename MergeOp = function<T(T,T)>>
struct SegmentTree {
    int       N;            // the size of array
    vector<T> tree;         // 

    MergeOp   mergeOp;
    T         defaultValue;

    explicit SegmentTree(MergeOp op, T dflt = T())
        : N(0), tree(), mergeOp(op), defaultValue(dflt) {
    }

    SegmentTree(int size, MergeOp op, T dflt = T())
        : N(size), tree(size * 4, dflt), mergeOp(op), defaultValue(dflt) {
    }

    SegmentTree(T value, int n, MergeOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(value, n);
    }

    SegmentTree(const T arr[], int n, MergeOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(arr, n);
    }

    SegmentTree(const vector<T>& v, MergeOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(v);
    }


    // O(N)
    void init(int size) {
        N = size;
        tree.assign(size * 4, defaultValue);
    }

    // O(NlogN)
    T build(T value, int n) {
        init(n);
        return buildSub(value, 0, n - 1, 1);
    }

    // O(NlogN)
    T build(const T arr[], int n) {
        init(n);
        return buildSub(arr, 0, n - 1, 1);
    }

    // O(NlogN)
    T build(const vector<T>& v) {
        return build(&v[0], int(v.size()));
    }


    // inclusive, O(logN)
    T query(int left, int right) const {
        return querySub(left, right, 1, 0, N - 1);
    }

    // inclusive, O(logN)
    T update(int index, T newValue) {
        return updateSub(index, newValue, 1, 0, N - 1);
    }

    // inclusive, O(KlogN)
    T updateRange(int left, int right, T newValue) {
        return updateRangeSub(left, right, newValue, 1, 0, N - 1);
    }


    // PRECONDITION: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
    // lower bound where f(x) is true in [0, N)
    //   f(x): xxxxxxxxxxxOOOOOOOO
    //         S          ^
    // O(logN)
    int lowerBound(const function<bool(T)>& f) const {
        return lowerBoundSub(f, T(0), 1, 0, N - 1);
    }

private:
    // inclusive
    T buildSub(const T arr[], int left, int right, int node) {
        if (left > right)
            return defaultValue;

        if (left == right)
            return tree[node] = arr[left];

        int mid = left + (right - left) / 2;
        T leftSum = buildSub(arr, left, mid, node * 2);
        T rightSum = buildSub(arr, mid + 1, right, node * 2 + 1);

        return tree[node] = mergeOp(leftSum, rightSum);
    }

    // inclusive
    T buildSub(T value, int left, int right, int node) {
        if (left > right)
            return defaultValue;

        if (left == right)
            return tree[node] = value;

        int mid = left + (right - left) / 2;
        T leftSum = buildSub(value, left, mid, node * 2);
        T rightSum = buildSub(value, mid + 1, right, node * 2 + 1);

        return tree[node] = mergeOp(leftSum, rightSum);
    }

    T querySub(int left, int right, int node, int nodeLeft, int nodeRight) const {
        if (right < nodeLeft || nodeRight < left)
            return defaultValue;

        if (left <= nodeLeft && nodeRight <= right)
            return tree[node];

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return mergeOp(querySub(left, right, node * 2, nodeLeft, mid),
                       querySub(left, right, node * 2 + 1, mid + 1, nodeRight));
    }

    T updateSub(int index, T newValue, int node, int nodeLeft, int nodeRight) {
        if (index < nodeLeft || nodeRight < index)
            return tree[node];

        if (nodeLeft == nodeRight)
            return tree[node] = newValue;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return tree[node] = mergeOp(updateSub(index, newValue, node * 2, nodeLeft, mid),
                                    updateSub(index, newValue, node * 2 + 1, mid + 1, nodeRight));
    }

    T updateRangeSub(int left, int right, T newValue, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return tree[node];

        if (nodeLeft == nodeRight)
            return tree[node] = newValue;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return tree[node] = mergeOp(updateRangeSub(left, right, newValue, node * 2, nodeLeft, mid),
                                    updateRangeSub(left, right, newValue, node * 2 + 1, mid + 1, nodeRight));
    }


    int lowerBoundSub(const function<bool(T)>& f, T delta, int node, int nodeLeft, int nodeRight) const {
        if (nodeLeft >= nodeRight)
            return nodeLeft;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        auto val = mergeOp(delta, tree[node * 2]);
        if (f(val))
            return lowerBoundSub(f, delta, node * 2, nodeLeft, mid);
        else
            return lowerBoundSub(f, val, node * 2 + 1, mid + 1, nodeRight);
    }
};

template <typename T, typename MergeOp>
inline SegmentTree<T, MergeOp> makeSegmentTree(int size, MergeOp op, T dfltValue = T()) {
    return SegmentTree<T, MergeOp>(size, op, dfltValue);
}

template <typename T, typename MergeOp>
inline SegmentTree<T, MergeOp> makeSegmentTree(const T arr[], int size, MergeOp op, T dfltValue = T()) {
    return SegmentTree<T, MergeOp>(arr, size, op, dfltValue);
}

template <typename T, typename MergeOp>
inline SegmentTree<T, MergeOp> makeSegmentTree(const vector<T>& v, MergeOp op, T dfltValue = T()) {
    return SegmentTree<T, MergeOp>(v, op, dfltValue);
}

/* example
    1) Min Segment Tree (RMQ)
        auto segTree = makeSegmentTree<int>(N, [](int a, int b) { return min(a, b); }, INT_MAX);
    2) Max Segment Tree 
        auto segTree = makeSegmentTree<int>(N, [](int a, int b) { return max(a, b); });
    3) GCD Segment Tree
        auto segTree = makeSegmentTree<int>(N, [](int a, int b) { return gcd(a, b); });
*/
