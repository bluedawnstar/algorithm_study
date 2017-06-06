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
    int       N;            // the size of array
    vector<T> tree;         // 

    T         defaultValue;
    BinOp     mergeOp;

    SegmentTree(int size, T dflt = T())
        : N(size), tree(size * 4, dflt), mergeOp(), defaultValue(dflt) {
    }

    SegmentTree(int size, BinOp op, T dflt = T())
        : N(size), tree(size * 4, dflt), mergeOp(op), defaultValue(dflt) {
    }

    // inclusive
    T build(const vector<T>& v, int left, int right) {
        return buildSub(v, left, right, 1);
    }
    // inclusive
    T build(const T arr[], int left, int right) {
        return buildSub(arr, left, right, 1);
    }

    // inclusive
    T query(int left, int right) {
        return querySub(left, right, 1, 0, N - 1);
    }

    // inclusive
    T update(int index, T newValue) {
        return updateSub(index, newValue, 1, 0, N - 1);
    }

    // inclusive
    T updateRange(int left, int right, T newValue) {
        return updateRangeSub(left, right, newValue, 1, 0, N - 1);
    }

private:
    // inclusive
    template <typename U>
    T buildSub(const U& arr, int left, int right, int node) {
        if (left > right)
            return defaultValue;

        if (left == right)
            return tree[node] = arr[left];

        int mid = left + (right - left) / 2;
        T leftSum = buildSub<U>(arr, left, mid, node * 2);
        T rightSum = buildSub<U>(arr, mid + 1, right, node * 2 + 1);

        return tree[node] = mergeOp(leftSum, rightSum);
    }

    T querySub(int left, int right, int node, int nodeLeft, int nodeRight) {
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
};

template <typename T, typename BinOp>
SegmentTree<T, BinOp> makeSegmentTree(int size, BinOp op, T dfltValue = T()) {
    return std::move(SegmentTree<T, BinOp>(size, op, dfltValue));
}

template <typename T, typename BinOp>
SegmentTree<T, BinOp> makeSegmentTree(const vector<T>& v, BinOp op, T dfltValue = T()) {
    auto segTree = SegmentTree<T, BinOp>((int)v.size(), op, dfltValue);
    segTree.build(v, 0, (int)v.size() - 1);
    return std::move(segTree);
}

template <typename T, typename BinOp>
SegmentTree<T, BinOp> makeSegmentTree(const T arr[], int size, BinOp op, T dfltValue = T()) {
    auto segTree = SegmentTree<T, BinOp>(size, op, dfltValue);
    segTree.build(arr, 0, size - 1);
    return std::move(segTree);
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
        init(size);
    }

    RMQ(int v[], int size) {
        init(size);
        
        for (int i = 0; i < size; i++)
            update(i, v[i]);
    }

    RMQ(const vector<int>& v) {
        init((int)v.size());

        for (int i = 0; i < (int)v.size(); i++)
            update(i, v[i]);
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

private:
    void init(int size) {
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
};
