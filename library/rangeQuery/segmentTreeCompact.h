#pragma once

#include <vector>
#include <functional>

//--------- Compact Segment Tree ----------------------------------------------
// http://codeforces.com/blog/entry/18051

// It's faster than SegmentTree 140x
template <typename T, typename BinOp = function<T(T, T)>>
struct CompactSegmentTree {
    int       N;            // the size of array
    vector<T> tree;         //
    
    T         defaultValue;
    BinOp     mergeOp;
    
    CompactSegmentTree(int size, T dflt = T())
        : N(size + (size & 1)), tree((size + (size & 1)) * 2, dflt), mergeOp(), defaultValue(dflt) {
    }
    
    CompactSegmentTree(int size, BinOp op, T dflt = T())
        : N(size + (size & 1)), tree((size + (size & 1)) * 2, dflt), mergeOp(op), defaultValue(dflt) {
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
        T res = defaultValue;
        for (index += N; index > 0; index >>= 1)
            res = mergeOp(res, tree[index]);
        return res;
    }

    // inclusive
    T query(int left, int right) {
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
    
    void update(int index, T newValue) {
        tree[index + N] = newValue;
        
        for (int i = (index + N) >> 1; i > 0; i >>= 1)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    // inclusive
    void updateRange(int left, int right, T newValue) {
        for (int L = left + N, R = right + N + 1; L < R; L++)
            tree[L] = newValue;

        for (int L = (left + N) >> 1, R = (right + N) >> 1; L > 0; L >>= 1, R >>= 1) {
            for (int i = L; i <= R; i++)
                tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
        }
    }

    void add(int index, T value) {
        tree[index + N] += value;
        
        for (int i = (index + N) >> 1; i > 0; i >>= 1)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    // inclusive
    void addRange(int left, int right, T value) {
        for (int L = left + N, R = right + N + 1; L < R; L++)
            tree[L] += value;
        
        for (int L = (left + N) >> 1, R = (right + N) >> 1; L > 0; L >>= 1, R >>= 1) {
            for (int i = L; i <= R; i++)
                tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
        }
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
    segTree.build(arr, size);
    return std::move(segTree);
}
