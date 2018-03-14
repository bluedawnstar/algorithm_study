#pragma once

#include <vector>
#include <functional>

//--------- Compact Segment Tree ----------------------------------------------
// http://codeforces.com/blog/entry/18051

// It's faster than SegmentTree 140x
template <typename T, typename BinOp = function<T(T, T)>>
struct CompactSegmentTree {
    int       RealN;
    int       N;            // the size of array
    vector<T> tree;         //
    
    T         defaultValue;
    BinOp     mergeOp;
    
    explicit CompactSegmentTree(BinOp op, T dflt = T())
        : RealN(0), N(0), tree(), mergeOp(op), defaultValue(dflt) {
    }

    CompactSegmentTree(int size, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        init(size);
    }
    
    CompactSegmentTree(const T arr[], int n, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(arr, n);
    }

    CompactSegmentTree(const vector<T>& v, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(v);
    }


    void init(int size) {
        RealN = size;
        N = size + (size & 1);
        tree.assign(N * 2, defaultValue);
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
        return tree[index + N];
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
    
    //--- update

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

    //--- add

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

    //--- find

    // find next position where f(x) is true in [start, N)
    //   f(x): xxxxxxxxxxxOOOOOOOO
    //         S          ^
    int findNext(int start, const function<bool(int)>& f) {
        int shiftN = 0;
        int cur = start + N, R = RealN - 1 + N;

        while (true) {
            if (f(tree[cur])) {
                if (cur < N) {
                    cur <<= 1;
                    shiftN--;
                } else {
                    return cur - N;
                }
            } else {
                if (++cur >(R >> shiftN))
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
        int shiftN = 0;
        int cur = start + N, L = N;

        while (true) {
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
    int ctz(unsigned x) const {
#ifndef __GNUC__
        return (int)_tzcnt_u32(x);
#else
        return __builtin_ctz(x);
#endif
    }
};

template <typename T, typename BinOp>
CompactSegmentTree<T, BinOp> makeCompactSegmentTree(int size, BinOp op, T dfltValue = T()) {
    return CompactSegmentTree<T, BinOp>(size, op, dfltValue);
}

template <typename T, typename BinOp>
CompactSegmentTree<T, BinOp> makeCompactSegmentTree(const vector<T>& v, BinOp op, T dfltValue = T()) {
    auto segTree = CompactSegmentTree<T, BinOp>((int)v.size(), op, dfltValue);
    segTree.build(v);
    return segTree;
}

template <typename T, typename BinOp>
CompactSegmentTree<T, BinOp> makeCompactSegmentTree(const T arr[], int size, BinOp op, T dfltValue = T()) {
    auto segTree = CompactSegmentTree<T, BinOp>(size, op, dfltValue);
    segTree.build(arr, size);
    return segTree;
}
