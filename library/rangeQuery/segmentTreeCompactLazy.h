#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

#include <vector>
#include <functional>

//--------- Compact Segment Tree - Lazy Update --------------------------------

// It's faster than SegmentTreeLazy 2x
template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct CompactSegmentTreeLazyUpdate {
    int         RealN;
    int         N;          // the size of array
    int         H;          // the height of the tree
    vector<T>   tree;       //
    vector<T>   treeLazy;   // 
    vector<bool> lazyExist; // 

    MergeOp     mergeOp;
    BlockOp     blockOp;
    T           defaultValue;

    CompactSegmentTreeLazyUpdate(MergeOp op, BlockOp bop, T dflt = T())
        : mergeOp(op), blockOp(bop), defaultValue(dflt) {
    }

    CompactSegmentTreeLazyUpdate(int size, MergeOp op, BlockOp bop, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), blockOp(bop), defaultValue(dflt) {
        init(size, alignPowerOf2);
    }

    CompactSegmentTreeLazyUpdate(T value, int n, MergeOp op, BlockOp bop, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), blockOp(bop), defaultValue(dflt) {
        build(value, n, alignPowerOf2);
    }

    CompactSegmentTreeLazyUpdate(const T arr[], int n, MergeOp op, BlockOp bop, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), blockOp(bop), defaultValue(dflt) {
        build(arr, n, alignPowerOf2);
    }

    CompactSegmentTreeLazyUpdate(const vector<T>& v, MergeOp op, BlockOp bop, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), blockOp(bop), defaultValue(dflt) {
        build(v, alignPowerOf2);
    }


    void init(int size, bool alignPowerOf2 = false) {
        RealN = size;
        if (alignPowerOf2) {
#ifndef __GNUC__
            H = 32 - int(_lzcnt_u32(size - 1));
#else
            H = 32 - __builtin_clz(size - 1);
#endif
            N = 1 << H++;
        } else {
            N = size + (size & 1);
#ifndef __GNUC__
            H = 32 - int(_lzcnt_u32(N));
#else
            H = 32 - __builtin_clz(N);
#endif
        }
        tree.assign(N * 2, defaultValue);
        treeLazy.assign(N, defaultValue);
        lazyExist.assign(N, false);
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

    //--- update

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

//private:
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
            for (int i = L >> shift, j = R >> shift; i <= j; ++i) {
                if (lazyExist[i]) {
                    apply((i << 1), treeLazy[i], k);
                    apply((i << 1) | 1, treeLazy[i], k);
                    lazyExist[i] = false;
                }
            }
        }
    }

    void pushDownOne(int index, int k) {
        if (index < N && lazyExist[index]) {
            apply((index << 1), treeLazy[index], k);
            apply((index << 1) | 1, treeLazy[index], k);
            lazyExist[index] = false;
        }
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline CompactSegmentTreeLazyUpdate<T, MergeOp, BlockOp>
makeCompactSegmentTreeLazyUpdate(int size, MergeOp op, BlockOp bop, T dfltValue = T(), bool alignPowerOf2 = false) {
    return CompactSegmentTreeLazyUpdate<T, MergeOp, BlockOp>(size, op, bop, dfltValue, alignPowerOf2);
}

template <typename T, typename MergeOp, typename BlockOp>
inline CompactSegmentTreeLazyUpdate<T, MergeOp, BlockOp>
makeCompactSegmentTreeLazyUpdate(const vector<T>& v, MergeOp op, BlockOp bop, T dfltValue = T(), bool alignPowerOf2 = false) {
    return CompactSegmentTreeLazyUpdate<T, MergeOp, BlockOp>(v, op, bop, dfltValue, alignPowerOf2);
}

template <typename T, typename MergeOp, typename BlockOp>
inline CompactSegmentTreeLazyUpdate<T, MergeOp, BlockOp>
makeCompactSegmentTreeLazyUpdate(const T arr[], int size, MergeOp op, BlockOp bop, T dfltValue = T(), bool alignPowerOf2 = false) {
    return CompactSegmentTreeLazyUpdate<T, MergeOp, BlockOp>(arr, size, op, bop, dfltValue, alignPowerOf2);
}

//-----------------------------------------------------------------------------

// PRECONDITION-1: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// PRECONDITION-2: N is aligned to power of 2
// find next position where f(x) is true in [start, N)
//   f(x): xxxxxxxxxxxOOOOOOOO
//         S          ^
template <typename T, typename MergeOp, typename BlockOp>
int findNext(CompactSegmentTreeLazyUpdate<T, MergeOp, BlockOp>& st, int start, const function<bool(T)>& f) {
    int shiftN = 0;
    int cur = start + st.N;

    st.pushDown(start, start);
    while (true) {
        st.pushDownOne(cur, 1 << shiftN);
        if (f(st.tree[cur])) {
            if (cur < st.N) {
                cur <<= 1;
                shiftN--;
            } else {
                return cur - st.N;
            }
        } else {
            ++cur;
            if ((cur & (cur - 1)) == 0)
                break;

#ifndef __GNUC__
            int n = int(_tzcnt_u32(cur));
#else
            int n = __builtin_ctz(cur);
#endif
            cur >>= n;
            shiftN += n;
        }
    }

    return -1;
}

// PRECONDITION-1: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// PRECONDITION-2: N is aligned to power of 2
// find previous position where f(x) is true in [0, start]
//   f(x): OOOOOOOOxxxxxxxxxxx
//                ^          S
template <typename T, typename MergeOp, typename BlockOp>
int findPrev(CompactSegmentTreeLazyUpdate<T, MergeOp, BlockOp>& st, int start, const function<bool(T)>& f) {
    int shiftN = 0;
    int cur = start + st.N;

    st.pushDown(start, start);
    while (true) {
        st.pushDownOne(cur, 1 << shiftN);
        if (f(st.tree[cur])) {
            if (cur < st.N) {
                cur = (cur << 1) | 1;
                shiftN--;
            } else {
                return cur - st.N;
            }
        } else {
            if ((cur & (cur - 1)) == 0)
                break;

#ifndef __GNUC__
            int n = int(_tzcnt_u32(cur));
#else
            int n = __builtin_ctz(cur);
#endif
            cur >>= n;
            shiftN += n;

            cur--;
        }
    }

    return -1;
}
