#pragma once

//--------- Generalized Binary Indexed Tree (Generalized Fenwick Tree) --------------------------------

// This data structure was invented by Youngman Ro. (youngman.ro@gmail.com, 2018/3)
template <typename T, typename MergeOp = function<T(T, T)>>
struct GeneralizedBIT {
    int N;                  // 
    vector<T> tree;         // forward BIT
    vector<T> treeR;        // backward BIT

    T       defaultValue;
    MergeOp mergeOp;

    explicit GeneralizedBIT(MergeOp op, T dflt = T())
        : N(0), mergeOp(op), defaultValue(dflt) {
    }

    GeneralizedBIT(int n, MergeOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        init(n);
    }

    GeneralizedBIT(const T value, int n, MergeOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(value, n);
    }

    GeneralizedBIT(const T arr[], int n, MergeOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(arr, n);
    }

    GeneralizedBIT(const vector<T>& v, MergeOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(v);
    }


    void init(int n) {
        N = n;
        tree = vector<T>(N + 1, defaultValue);
        treeR = vector<T>(N + 1, defaultValue);
    }
    
    void build(T value, int n) {
        init(n);
        for (int i = 0; i < n; i++)
            initUpdate(i, value);
    }

    void build(const T arr[], int n) {
        init(n);
        for (int i = 0; i < n; i++)
            initUpdate(i, arr[i]);
    }

    void build(const vector<T>& v) {
        build(&v[0], (int)v.size());
    }


    void clear() {
        fill(tree.begin(), tree.end(), defaultValue);
        fill(treeR.begin(), treeR.end(), defaultValue);
    }


    // inclusive (0 <= pos < N)
    void add(int pos, T val) {
        update(pos, query(pos) + val);
    }

    // inclusive (0 <= pos < N)
    void update(int pos, T val) {
        int curr = pos & ~1;
        int prev = pos | 1;

        if (pos & 1)
            treeR[prev] = val;
        else
            tree[prev] = val;

        int mask = 2;
        for (curr |= mask; curr <= N; prev = curr, curr = (curr & (curr - 1)) | mask) {
            if (prev & mask)
                treeR[curr] = mergeOp(tree[prev], treeR[prev]);
            else
                tree[curr] = mergeOp(tree[prev], treeR[prev]);
            mask <<= 1;
        }
    }

    void add(int left, int right, T val) {
        for (int L = (left + 1) | 1, R = right + 1; L <= R; L += 2)
            tree[L] += val;

        for (int L = left | 1, R = right; L <= R; L += 2)
            treeR[L] += val;

        rebuild(left, right);
    }

    void update(int left, int right, T val) {
        for (int L = (left + 1) | 1, R = right + 1; L <= R; L += 2)
            tree[L] = val;

        for (int L = left | 1, R = right; L <= R; L += 2)
            treeR[L] = val;

        rebuild(left, right);
    }


    // inclusive (0 <= pos < N)
    T query(int pos) const {
        return (pos & 1) ? treeR[pos] : tree[pos + 1];
    }

    // inclusive (0 <= left <= right < N)
    T query(int left, int right) const {
        T res = defaultValue;
        if (left == 0) {
            for (int R = right + 1; R > 0; R &= R - 1)
                res = mergeOp(res, tree[R]);
        } else {
            int R = right + 1;
            for (int next = R & (R - 1); left <= next; R = next, next = R & (R - 1))
                res = mergeOp(res, tree[R]);

            for (int L = left; L < R; L += L & -L)
                res = mergeOp(res, treeR[L]);
        }
        return res;
    }

private:
    void initUpdate(int pos, T val) {
        for (int i = pos + 1; i <= N; i += i & -i)
            tree[i] = mergeOp(tree[i], val);

        for (int i = pos; i > 0; i &= i - 1)
            treeR[i] = mergeOp(treeR[i], val);
    }

    void rebuild(int left, int right) {
        int mask = 2;

        int leftMask = left << 1;
        int rightMask = ~right << 1;

        int L = (left & ~1) | mask;
        int R = (right & ~1) | mask;
        while (L <= N) {
            int half = mask >> 1;
            mask <<= 1;

            int maxR = min(R, N);
            for (int i = L + (leftMask & mask), j = i - half; i <= maxR; i += mask, j += mask)
                tree[i] = mergeOp(tree[j], treeR[j]);

            maxR = min(R - (rightMask & mask), N - half);
            for (int i = L, j = i + half; i <= maxR; i += mask, j += mask)
                treeR[i] = mergeOp(tree[j], treeR[j]);

            L = (L & (L - 1)) | mask;
            R = (R & (R - 1)) | mask;
        }
    }
};

template <typename T, typename MergeOp>
inline GeneralizedBIT<T, MergeOp> makeGeneralizedBIT(int size, MergeOp op, T dfltValue = T()) {
    return GeneralizedBIT<T, MergeOp>(size, op, dfltValue);
}

template <typename T, typename MergeOp>
inline GeneralizedBIT<T, MergeOp> makeGeneralizedBIT(const T arr[], int size, MergeOp op, T dfltValue = T()) {
    return GeneralizedBIT<T, MergeOp>(arr, size, op, dfltValue);
}

template <typename T, typename MergeOp>
inline GeneralizedBIT<T, MergeOp> makeGeneralizedBIT(const vector<T>& v, MergeOp op, T dfltValue = T()) {
    return GeneralizedBIT<T, MergeOp>(v, op, dfltValue);
}

//-----------------------------------------------------------------------------

// PRECONDITION: tree's range operation is monotonically increasing
// return min(x | query(left, i) >= value, left <= i <= right)
//    xxxxxxxOOOOOOoooooo
//    L      ^          R
template <typename T, typename MergeOp>
inline int lowerBound(const GeneralizedBIT<T, MergeOp>& st, int left, int right, T value) {
    int lo = left, hi = right;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (st.query(left, mid) < value)
            lo = mid + 1;
        else
            hi = mid - 1;
    }

    return lo;
}

// PRECONDITION: tree's range operation is monotonically increasing
// return min(x | query(left, i) > value, left <= i <= right)
//    xxxxxxxOOOOOOoooooo
//    L            ^    R
template <typename T, typename MergeOp>
inline int upperBound(const GeneralizedBIT<T, MergeOp>& st, int left, int right, T value) {
    int lo = left, hi = right;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (st.query(left, mid) <= value)
            lo = mid + 1;
        else
            hi = mid - 1;
    }

    return lo;
}

// PRECONDITION: tree's range operation is monotonically increasing
// return max(x | query(left, i) >= value, left <= i <= right)
//    oooooooOOOOOOxxxxxx
//    L           ^     R
template <typename T, typename MergeOp>
inline int lowerBoundBackward(const GeneralizedBIT<T, MergeOp>& st, int left, int right, T value) {
    int lo = left, hi = right;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (st.query(mid, right) < value)
            hi = mid - 1;
        else
            lo = mid + 1;
    }

    return hi;
}

// PRECONDITION: tree's range operation is monotonically increasing
// return min(x | query(left, i) > value, left <= i <= right)
//    oooooooOOOOOOxxxxxx
//    L     ^           R
template <typename T, typename MergeOp>
inline int upperBoundBackward(const GeneralizedBIT<T, MergeOp>& st, int left, int right, T value) {
    int lo = left, hi = right;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (st.query(mid, right) <= value)
            hi = mid - 1;
        else
            lo = mid + 1;
    }

    return hi;
}


// PRECONDITION: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// find next position where f(x) is true in [start, N)
//   f(x): xxxxxxxxxxxOOOOOOOO
//         S          ^
template <typename T, typename MergeOp>
inline int findNext(const GeneralizedBIT<T, MergeOp>& gbit, int start, const function<bool(T)>& f) {
    int pos = start;
    while (pos < gbit.N) {
        if ((pos & 1) == 0) {
            pos++;
            if (f(gbit.tree[pos]))
                return pos - 1;
        }
        if (pos < gbit.N && f(gbit.treeR[pos])) {
            return pos;
            pos += pos & -pos;
        }

        while (pos < gbit.N && !f(gbit.treeR[pos]))
            pos += pos & -pos;
    }

    return -1;
}

// PRECONDITION: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// find previous position where f(x) is true in [0, start]
//   f(x): OOOOOOOOxxxxxxxxxxx
//                ^          S
template <typename T, typename MergeOp>
inline int findPrev(const GeneralizedBIT<T, MergeOp>& gbit, int start, const function<bool(T)>& f) {
    int pos = start + 1;
    while (pos > 0) {
        if ((pos & 1) == 0) {
            pos--;
            if (f(gbit.treeR[pos]))
                return pos;
        }
        if (pos > 0 && f(gbit.tree[pos]))
            return pos - 1;

        while (pos > 0 && !f(gbit.tree[pos]))
            pos &= pos - 1;
    }

    return -1;
}
