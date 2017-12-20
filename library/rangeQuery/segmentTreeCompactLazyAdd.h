#pragma once

#include <vector>
#include <functional>

//--------- Compact Segment Tree - Lazy Add -----------------------------------
// http://codeforces.com/blog/entry/18051

// It's slower than FenwickTree 10x
template <typename T, typename BinOp = function<T(T, T)>>
struct CompactSegmentTreeLazyAdd {
    int       N;            // the size of array
    int       H;            // the height of the tree
    vector<T> tree;         //
    vector<T> treeLazy;     // 0 means "not lazy value"

    T         defaultValue;
    BinOp     mergeOp;

    CompactSegmentTreeLazyAdd(int size, T dflt = T())
        : N(size + (size & 1)), tree(N * 2, dflt), treeLazy(N, dflt), mergeOp(), defaultValue(dflt) {
        H = 0;
        for (int i = N; i; i >>= 1)
            H++;
    }

    CompactSegmentTreeLazyAdd(int size, BinOp op, T dflt = T())
        : N(size + (size & 1)), tree(N * 2, dflt), treeLazy(N, dflt), mergeOp(op), defaultValue(dflt) {
        H = 0;
        for (int i = N; i; i >>= 1)
            H++;
    }

    void init(T value, int size) {
        for (int i = 0; i < size; i++)
            tree[N + i] = value;

        for (int i = N - 1; i > 0; i--)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    void build(const vector<T>& v) {
        for (int i = 0; i < (int)v.size(); i++)
            tree[N + i] = v[i];

        for (int i = N - 1; i > 0; i--)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    void build(const T arr[], int size) {
        for (int i = 0; i < size; i++)
            tree[N + i] = arr[i];

        for (int i = N - 1; i > 0; i--)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    T query(int index) {
        return query(index, index);
    }

    // inclusive
    T query(int left, int right) {
        pushDown(left + N);
        if (left != right)
            pushDown(right + N);

        T resL = defaultValue;
        T resR = defaultValue;
        for (int L = left + N, R = right + N + 1; L < R; L >>= 1, R >>= 1) {
            if (L & 1)
                resL = mergeOp(resL, tree[L++]);
            if (R & 1)
                resR = mergeOp(tree[--R], resR);
        }

        return mergeOp(resL, resR);
    }

    void add(int index, T value) {
        addRange(index, index, value);
    }

    // inclusive
    void addRange(int left, int right, T value) {
        for (int L = left + N, R = right + N + 1; L < R; L >>= 1, R >>= 1) {
            if (L & 1)
                apply(L++, value);
            if (R & 1)
                apply(--R, value);
        }
        pushUp(left + N);
        if (left != right)
            pushUp(right + N);
    }

private:
    void apply(int index, T value) {
        tree[index] += value;
        if (index < N)
            treeLazy[index] += value;
    }

    void pushUp(int index) {
        while (index > 1) {
            index >>= 1;
            tree[index] = mergeOp(tree[index << 1], tree[(index << 1) | 1]) + treeLazy[index];
        }
    }

    void pushDown(int index) {
        for (int shift = H; shift > 0; --shift) {
            int i = index >> shift;
            if (treeLazy[i]) {
                apply((i << 1), treeLazy[i]);
                apply((i << 1) | 1, treeLazy[i]);
                treeLazy[i] = 0;
            }
        }
    }
};

template <typename T, typename BinOp>
CompactSegmentTreeLazyAdd<T, BinOp> makeCompactSegmentTreeLazyAdd(int size, BinOp op, T dfltValue = T()) {
    return CompactSegmentTreeLazyAdd<T, BinOp>(size, op, dfltValue);
}

template <typename T, typename BinOp>
CompactSegmentTreeLazyAdd<T, BinOp> makeCompactSegmentTreeLazyAdd(const vector<T>& v, BinOp op, T dfltValue = T()) {
    auto segTree = CompactSegmentTreeLazyAdd<T, BinOp>((int)v.size(), op, dfltValue);
    segTree.build(v);
    return segTree;
}

template <typename T, typename BinOp>
CompactSegmentTreeLazyAdd<T, BinOp> makeCompactSegmentTreeLazyAdd(const T arr[], int size, BinOp op, T dfltValue = T()) {
    auto segTree = CompactSegmentTreeLazyAdd<T, BinOp>(size, op, dfltValue);
    segTree.build(arr, size);
    return segTree;
}
