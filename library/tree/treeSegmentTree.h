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
template <typename T, typename BinOp = function<T(T,T)>>
struct SegmentTree {
    int       n;            // the size of array
    vector<T> rangeValue;   // 
    T         defaultValue;
    BinOp     mergeOp;

    SegmentTree(int size, T dflt = T())
        : mergeOp(), defaultValue(dflt) {

        n = size;
        rangeValue.resize(n * 4);
        _init(0, n - 1, 1);
    }

    SegmentTree(int size, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {

        n = size;
        rangeValue.resize(n * 4);
        _init(0, n - 1, 1);
    }

    SegmentTree(const vector<T>& arr, T dflt = T())
        : mergeOp(), defaultValue(dflt) {

        n = (int)arr.size();
        rangeValue.resize(n * 4);
        _init(arr, 0, n - 1, 1);
    }

    SegmentTree(const T arr[], int size, T dflt = T())
        : mergeOp(), defaultValue(dflt) {

        n = size;
        rangeValue.resize(n * 4);
        _init(arr, 0, n - 1, 1);
    }

    SegmentTree(const vector<T>& arr, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {

        n = (int)arr.size();
        rangeValue.resize(n * 4);
        _init(arr, 0, n - 1, 1);
    }

    SegmentTree(const T arr[], int size, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {

        n = size;
        rangeValue.resize(n * 4);
        _init(arr, 0, n - 1, 1);
    }

    SegmentTree(SegmentTree&& rhs)
        : n(rhs.n), rangeValue(std::move(rhs.rangeValue)),
          defaultValue(rhs.defaultValue), mergeOp(std::move(rhs.mergeOp)) {
    }

    // inclusive
    T _init(int left, int right, int node) {
        if (left > right)
            return defaultValue;

        if (left == right)
            return rangeValue[node] = defaultValue;

        int mid = left + (right - left) / 2;
        int leftSum = _init(left, mid, node * 2);
        int rightSum = _init(mid + 1, right, node * 2 + 1);

        return rangeValue[node] = mergeOp(leftSum, rightSum);
    }

    // inclusive
    template <typename U>
    T _init(const U& arr, int left, int right, int node) {
        if (left > right)
            return defaultValue;

        if (left == right)
            return rangeValue[node] = arr[left];

        int mid = left + (right - left) / 2;
        int leftSum = _init<U>(arr, left, mid, node * 2);
        int rightSum = _init<U>(arr, mid + 1, right, node * 2 + 1);

        return rangeValue[node] = mergeOp(leftSum, rightSum);
    }

    T _querySub(int left, int right, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return defaultValue;

        if (left <= nodeLeft && nodeRight <= right)
            return rangeValue[node];

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return mergeOp(_querySub(left, right, node * 2, nodeLeft, mid),
                       _querySub(left, right, node * 2 + 1, mid + 1, nodeRight));
    }
    // inclusive
    T query(int left, int right) {
        return _querySub(left, right, 1, 0, n - 1);
    }

    T _updateSub(int index, T newValue, int node, int nodeLeft, int nodeRight) {
        if (index < nodeLeft || nodeRight < index)
            return rangeValue[node];

        if (nodeLeft == nodeRight)
            return rangeValue[node] = newValue;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return rangeValue[node] = mergeOp(_updateSub(index, newValue, node * 2, nodeLeft, mid),
                                          _updateSub(index, newValue, node * 2 + 1, mid + 1, nodeRight));
    }
    T update(int index, T newValue) {
        return _updateSub(index, newValue, 1, 0, n - 1);
    }

    T _updateRangeSub(int left, int right, T newValue, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return rangeValue[node];

        if (nodeLeft == nodeRight)
            return rangeValue[node] = newValue;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return rangeValue[node] = mergeOp(_updateRangeSub(left, right, newValue, node * 2, nodeLeft, mid),
                                          _updateRangeSub(left, right, newValue, node * 2 + 1, mid + 1, nodeRight));
    }
    // inclusive
    T updateRange(int left, int right, T newValue) {
        return _updateRangeSub(left, right, newValue, 1, 0, n - 1);
    }
};

template <typename T, typename BinOp>
SegmentTree<T, BinOp> makeSegmentTree(int size, BinOp op, T dfltValue = T()) {
    return std::move(SegmentTree<T, BinOp>(size, op, dfltValue));
}

template <typename T, typename BinOp>
SegmentTree<T, BinOp> makeSegmentTree(const vector<T>& arr, BinOp op, T dfltValue = T()) {
    return std::move(SegmentTree<T, BinOp>(arr, op, dfltValue));
}

template <typename T, typename BinOp>
SegmentTree<T, BinOp> makeSegmentTree(const T arr[], int size, BinOp op, T dfltValue = T()) {
    return std::move(SegmentTree<T, BinOp>(arr, size, op, dfltValue));
}

/* example
    1) Min Segment Tree (RMQ)
        auto segTree = makeSegmentTree<int>(N, [](int a, int b) { return min(a, b); }, INT_MAX);
    2) Max Segment Tree 
        auto segTree = makeSegmentTree<int>(N, [](int a, int b) { return max(a, b); });
    3) GCD Segment Tree
        auto segTree = makeSegmentTree<int>(N, [](int a, int b) { return gcd(a, b); });
*/

//--------- RMQ (Range Minimum Query) - Min Segment Tree ----------------------

// RMQ (Range Minimum Query)
struct RMQ {
    int       N;
    vector<int> value;
    vector<int> valueId;

    RMQ(int size) {
        _init(size);
    }

    RMQ(int v[], int size) {
        _init(size);
        
        for (int i = 0; i < size; i++)
            update(i, v[i]);
    }

    RMQ(const vector<int>& v) {
        _init((int)v.size());

        for (int i = 0; i < (int)v.size(); i++)
            update(i, v[i]);
    }

    void _init(int size) {
        N = 1;
        while (N <= size)
            N <<= 1;

        value.resize(N * 2);
        valueId.resize(N * 2);
        for (int i = 0; i < N; i++)
            valueId[N + i] = i;

        for (int i = 0; i < size; i++)
            update(i, INT_MAX);
    }

    void update(int x, int val) {
        x += N;
        value[x] = val;

        while (x > 1) {
            x >>= 1;
            int left = value[x << 1];               // left = value[x * 2]
            int right = value[(x << 1) | 1];        // right = value[x * 2 + 1]
            if (left <= right) {
                value[x] = left;
                valueId[x] = valueId[x << 1];       // valueId = valueId[x * 2]
            } else {
                value[x] = right;
                valueId[x] = valueId[(x << 1) | 1]; // valueId = valueId[x * 2 + 1]
            }
        }
    }

    // inclusive
    int query(int left, int right) {
        left += N;
        right += N;
        int ret = INT_MAX;
        while (left <= right) {
            if ((left & 1) == 1) {
                ret = min(ret, value[left]);
                left++;
            }
            if ((right & 1) == 0) {
                ret = min(ret, value[right]);
                right--;
            }
            left >>= 1;
            right >>= 1;
        }
        return ret;
    }
};
