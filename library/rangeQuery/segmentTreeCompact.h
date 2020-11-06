#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

#include <vector>
#include <functional>

//--------- Compact Segment Tree ----------------------------------------------
// http://codeforces.com/blog/entry/18051

// It's faster than SegmentTree 140x
template <typename T, typename MergeOp = function<T(T, T)>>
struct CompactSegmentTree {
    int       RealN;
    int       N;            // the size of array
    vector<T> tree;         //
    
    MergeOp   mergeOp;
    T         defaultValue;

    explicit CompactSegmentTree(MergeOp op, T dflt = T())
        : RealN(0), N(0), tree(), mergeOp(op), defaultValue(dflt) {
    }

    CompactSegmentTree(int size, MergeOp op, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), defaultValue(dflt) {
        init(size, alignPowerOf2);
    }

    CompactSegmentTree(T value, int n, MergeOp op, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), defaultValue(dflt) {
        build(value, n, alignPowerOf2);
    }

    CompactSegmentTree(const T arr[], int n, MergeOp op, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), defaultValue(dflt) {
        build(arr, n, alignPowerOf2);
    }

    CompactSegmentTree(const vector<T>& v, MergeOp op, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), defaultValue(dflt) {
        build(v, alignPowerOf2);
    }


    void init(int size, bool alignPowerOf2 = false) {
        RealN = size;
        if (alignPowerOf2) {
#ifndef __GNUC__
            N = 1 << (32 - int(_lzcnt_u32(size - 1)));
#else
            N = 1 << (32 - __builtin_clz(size - 1));
#endif
        } else
            N = size + (size & 1);
        tree.assign(N * 2, defaultValue);
    }

    void build(T value, int size, bool alignPowerOf2 = false) {
        init(size, alignPowerOf2);

        for (int i = 0; i < size; i++)
            tree[N + i] = value;

        for (int i = N - 1; i > 0; i--)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    void build(const T arr[], int size, bool alignPowerOf2 = false) {
        init(size, alignPowerOf2);

        for (int i = 0; i < size; i++)
            tree[N + i] = arr[i];

        for (int i = N - 1; i > 0; i--)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    void build(const vector<T>& v, bool alignPowerOf2 = false) {
        build(&v[0], int(v.size()), alignPowerOf2);
    }

    //--- query

    T query(int index) const {
        return tree[index + N];
    }

    // inclusive
    T query(int left, int right) const {
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
    void update(int left, int right, T newValue) {
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
    void add(int left, int right, T value) {
        for (int L = left + N, R = right + N + 1; L < R; L++)
            tree[L] += value;
        
        for (int L = (left + N) >> 1, R = (right + N) >> 1; L > 0; L >>= 1, R >>= 1) {
            for (int i = L; i <= R; i++)
                tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
        }
    }
};

template <typename T, typename MergeOp>
inline CompactSegmentTree<T, MergeOp> makeCompactSegmentTree(int size, MergeOp op, T dfltValue = T(), bool alignPowerOf2 = false) {
    return CompactSegmentTree<T, MergeOp>(size, op, dfltValue, alignPowerOf2);
}

template <typename T, typename MergeOp>
inline CompactSegmentTree<T, MergeOp> makeCompactSegmentTree(const vector<T>& v, MergeOp op, T dfltValue = T(), bool alignPowerOf2 = false) {
    return CompactSegmentTree<T, MergeOp>(v, op, dfltValue, alignPowerOf2);
}

template <typename T, typename MergeOp>
inline CompactSegmentTree<T, MergeOp> makeCompactSegmentTree(const T arr[], int size, MergeOp op, T dfltValue = T(), bool alignPowerOf2 = false) {
    return CompactSegmentTree<T, MergeOp>(arr, size, op, dfltValue, alignPowerOf2);
}

//-----------------------------------------------------------------------------

// PRECONDITION-1: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// PRECONDITION-2: N is aligned to power of 2
// find next position where f(x) is true in [start, N)
//   f(x): xxxxxxxxxxxOOOOOOOO
//         S          ^
// O(logN)
template <typename T, typename MergeOp>
inline int findNext(const CompactSegmentTree<T,MergeOp>& st, int start, const function<bool(T)>& f) {
    int cur = start + st.N;

    while (true) {
        if (f(st.tree[cur])) {
            if (cur < st.N)
                cur <<= 1;
            else
                return cur - st.N;
        } else {
            ++cur;
            if ((cur & (cur - 1)) == 0)
                break;

#ifndef __GNUC__
            cur >>= int(_tzcnt_u32(cur));
#else
            cur >>= __builtin_ctz(cur);
#endif
        }
    }

    return -1;
}

// PRECONDITION-1: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// PRECONDITION-2: N is aligned to power of 2
// find previous position where f(x) is true in [0, start]
//   f(x): OOOOOOOOxxxxxxxxxxx
//                ^          S
// O(logN)
template <typename T, typename MergeOp>
inline int findPrev(const CompactSegmentTree<T, MergeOp>& st, int start, const function<bool(T)>& f) {
    int cur = start + st.N;

    while (true) {
        if (f(st.tree[cur])) {
            if (cur < st.N)
                cur = (cur << 1) | 1;
            else
                return cur - st.N;
        } else {
            if ((cur & (cur - 1)) == 0)
                break;

#ifndef __GNUC__
            cur >>= int(_tzcnt_u32(cur));
#else
            cur >>= __builtin_ctz(cur);
#endif
            cur--;
        }
    }

    return -1;
}
