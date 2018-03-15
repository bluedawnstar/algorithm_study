#pragma once

#include <vector>
#include <functional>

//--------- Compact Segment Tree - Lazy Add -----------------------------------
// http://codeforces.com/blog/entry/18051

// It's slower than FenwickTree 10x for just summation
// (a.k.a. Starry Sky Tree)
template <typename T, typename BinOp = function<T(T, T)>>
struct CompactSegmentTreeLazyAdd {
    int       RealN;
    int       N;            // the size of array
    int       H;            // the height of the tree
    vector<T> tree;         //
    vector<T> treeLazy;     // 0 means "not lazy value"

    T         defaultValue;
    BinOp     mergeOp;

    explicit CompactSegmentTreeLazyAdd(BinOp op, T dflt = T())
        : mergeOp(), defaultValue(dflt) {
    }

    CompactSegmentTreeLazyAdd(int size, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        init(size);
    }

    CompactSegmentTreeLazyAdd(T value, int n, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(value, n);
    }

    CompactSegmentTreeLazyAdd(const T arr[], int n, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(arr, n);
    }

    CompactSegmentTreeLazyAdd(const vector<T>& v, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(v);
    }


    void init(int size) {
        RealN = size;
        N = size + (size & 1);
        tree.assign(N * 2, defaultValue);
        treeLazy.assign(N, defaultValue);

        H = 0;
        for (int i = N; i; i >>= 1)
            H++;
    }

    void build(T value, int size) {
        init(size);

        for (int i = 0; i < size; i++)
            tree[N + i] = value;

        for (int i = N - 1; i > 0; i--)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    void build(const T arr[], int size) {
        init(size);

        for (int i = 0; i < size; i++)
            tree[N + i] = arr[i];

        for (int i = N - 1; i > 0; i--)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    void build(const vector<T>& v) {
        build(&v[0], (int)v.size());
    }

    //--- query

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

    //--- update

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

    //--- find

    // find next position where f(x) is true in [start, N)
    //   f(x): xxxxxxxxxxxOOOOOOOO
    //         S          ^
    int findNext(int start, const function<bool(int)>& f) {
        pushDown(start + N);

        int shiftN = 0;
        int cur = start + N, R = RealN - 1 + N;

        while (true) {
            pushDownOne(cur);
            if (f(tree[cur])) {
                if (cur < N) {
                    cur <<= 1;
                    shiftN--;
                } else {
                    return cur - N;
                }
            } else {
                if (++cur > (R >> shiftN))
                    break;

                int n = ctz(cur);
                cur >>= n;
                shiftN += n;
            }
        }

        return -1;
    }

    // find previous position where f(x) is true in [0, start]
    //   f(x): OOOOOOOOxxxxxxxxxxx
    //                ^          S
    int findPrev(int start, const function<bool(int)>& f) {
        pushDown(start + N);

        int shiftN = 0;
        int cur = start + N, L = N;

        while (true) {
            pushDownOne(cur);
            if (f(tree[cur])) {
                if (cur < N) {
                    cur = (cur << 1) | 1;
                    shiftN--;
                } else {
                    return cur - N;
                }
            } else {
                if (cur <= (L >> shiftN))
                    break;

                int n = ctz(cur);
                cur >>= n;
                shiftN += n;

                cur--;
            }
        }

        return -1;
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

    void pushDownOne(int index) {
        if (index < N && treeLazy[index]) {
            apply((index << 1), treeLazy[index]);
            apply((index << 1) | 1, treeLazy[index]);
            treeLazy[index] = 0;
        }
    }

    int ctz(unsigned x) const {
#ifndef __GNUC__
        return (int)_tzcnt_u32(x);
#else
        return __builtin_ctz(x);
#endif
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
