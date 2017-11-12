#pragma once

#include <vector>
#include <functional>

//--------- Compact Segment Tree ----------------------------------------------

template <typename T, typename BinOp = function<T(T, T)>>
struct CompactSegmentTree {
    int       N;            // the size of array
    vector<T> tree;         // 

    T         defaultValue;
    BinOp     mergeOp;

    CompactSegmentTree(int size, T dflt = T())
        : N(size), tree(size * 2, dflt), mergeOp(), defaultValue(dflt) {
    }

    CompactSegmentTree(int size, BinOp op, T dflt = T())
        : N(size), tree(size * 2, dflt), mergeOp(op), defaultValue(dflt) {
    }

    void init(T value) {
        for (int i = 0; i < N; i++)
            tree[N + i] = value;

        for (int i = N - 1; i > 0; i--)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    void build(const vector<T>& v) {
        for (int i = 0; i < N; i++)
            tree[N + i] = v[i];

        for (int i = N - 1; i > 0; i--)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    void build(const T arr[]) {
        for (int i = 0; i < N; i++)
            tree[N + i] = arr[i];

        for (int i = N - 1; i > 0; i--)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    // inclusive
    T query(int left, int right) {
        T res = defaultValue;

        for (int L = left + N, R = N + right + 1; L < R; L >>= 1, R >>= 1) {
            if (L & 1)
                res = mergeOp(res, tree[L++]);
            if (R & 1)
                res = mergeOp(res, tree[--R]);
        }

        return res;
    }

    // inclusive
    void update(int index, T newValue) {
        index += N;
        tree[index] = newValue;

        for (int i = index; i > 1; i >>= 1)
            tree[i >> 1] = mergeOp(tree[i], tree[i ^ 1]);
    }
};

template <typename T, typename BinOp>
CompactSegmentTree<T, BinOp> makeCompactSegmentTree(int size, BinOp op, T dfltValue = T()) {
    return std::move(CompactSegmentTree<T, BinOp>(size, op, dfltValue));
}

template <typename T, typename BinOp>
CompactSegmentTree<T, BinOp> makeCompactSegmentTree(const vector<T>& v, BinOp op, T dfltValue = T()) {
    auto segTree = CompactSegmentTree<T, BinOp>((int)v.size(), op, dfltValue);
    segTree.build(v);
    return std::move(segTree);
}

template <typename T, typename BinOp>
CompactSegmentTree<T, BinOp> makeCompactSegmentTree(const T arr[], int size, BinOp op, T dfltValue = T()) {
    auto segTree = CompactSegmentTree<T, BinOp>(size, op, dfltValue);
    segTree.build(arr);
    return std::move(segTree);
}
