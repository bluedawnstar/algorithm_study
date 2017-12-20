#pragma once

#include <vector>
#include <functional>

//--------- Compact Segment Tree - Lazy Update --------------------------------

// It's faster than SegmentTreeLazy 2x
template <typename T, typename BinOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct CompactSegmentTreeLazyUpdate {
    int         N;          // the size of array
    int         H;          // the height of the tree
    vector<T>   tree;       //
    vector<T>   treeLazy;   // 
    vector<bool> lazyExist; // 

    T           defaultValue;
    BinOp       mergeOp;
    BlockOp     blockOp;

    CompactSegmentTreeLazyUpdate(int size, T dflt = T())
        : N(size + (size & 1)), tree(N * 2, dflt), treeLazy(N, dflt), lazyExist(N), mergeOp(), blockOp(), defaultValue(dflt) {
        H = 0;
        for (int i = N; i; i >>= 1)
            H++;
    }

    CompactSegmentTreeLazyUpdate(int size, BinOp op, BlockOp bop, T dflt = T())
        : N(size + (size & 1)), tree(N * 2, dflt), treeLazy(N, dflt), lazyExist(N), mergeOp(op), blockOp(bop), defaultValue(dflt) {
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
        pushDown(left, left);
        if (left != right)
            pushDown(right, right);

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
        updateRange(index, index, newValue);
    }

    // inclusive
    void updateRange(int left, int right, T newValue) {
        pushDown(left, left);
        if (left != right)
            pushDown(right, right);
        int k = 1;
        for (int L = left + N, R = right + N + 1; L < R; L >>= 1, R >>= 1, k <<= 1) {
            if (L & 1)
                apply(L++, newValue, k);
            if (R & 1)
                apply(--R, newValue, k);
        }
        pushUp(left, left);
        if (left != right)
            pushUp(right, right);
    }

private:
    void calc(int index, int k) {
        if (!lazyExist[index])
            tree[index] = mergeOp(tree[index << 1], tree[(index << 1) | 1]);
        else
            tree[index] = blockOp(treeLazy[index], k);
    }

    void apply(int index, T value, int k) {
        tree[index] = blockOp(value, k);
        if (index < N) {
            treeLazy[index] = value;
            lazyExist[index] = true;
        }
    }

    void pushUp(int left, int right) {
        int k = 2;
        for (int L = left + N, R = right + N; L > 1; k <<= 1) {
            L >>= 1;
            R >>= 1;
            for (int i = R; i >= L; --i)
                calc(i, k);
        }
    }

    void pushDown(int left, int right) {
        int shift = H, k = 1 << (H - 1);
        for (int L = left + N, R = right + N; shift > 0; --shift, k >>= 1) {
            for (int i = L >> shift; i <= R >> shift; ++i) {
                if (lazyExist[i]) {
                    apply((i << 1), treeLazy[i], k);
                    apply((i << 1) | 1, treeLazy[i], k);
                    lazyExist[i] = false;
                }
            }
        }
    }
};

template <typename T, typename BinOp, typename BlockOp>
CompactSegmentTreeLazyUpdate<T, BinOp, BlockOp>
makeCompactSegmentTreeLazyUpdate(int size, BinOp op, BlockOp bop, T dfltValue = T()) {
    return CompactSegmentTreeLazyUpdate<T, BinOp, BlockOp>(size, op, bop, dfltValue);
}

template <typename T, typename BinOp, typename BlockOp>
CompactSegmentTreeLazyUpdate<T, BinOp, BlockOp>
makeCompactSegmentTreeLazyUpdate(const vector<T>& v, BinOp op, BlockOp bop, T dfltValue = T()) {
    auto segTree = CompactSegmentTreeLazyUpdate<T, BinOp, BlockOp>((int)v.size(), op, bop, dfltValue);
    segTree.build(v);
    return segTree;
}

template <typename T, typename BinOp, typename BlockOp>
CompactSegmentTreeLazyUpdate<T, BinOp, BlockOp>
makeCompactSegmentTreeLazyUpdate(const T arr[], int size, BinOp op, BlockOp bop, T dfltValue = T()) {
    auto segTree = CompactSegmentTreeLazyUpdate<T, BinOp, BlockOp>(size, op, bop, dfltValue);
    segTree.build(arr, size);
    return segTree;
}
