#pragma once

//--------- Generalized Binary Indexed Tree (Generalized Fenwick Tree) --------------------------------

template <typename T, typename MergeOp = function<T(T, T)>>
struct GeneralizedBIT {
    int N;                  // 
    vector<T> tree;         // forward BIT
    vector<T> treeR;        // backward BIT

    MergeOp mergeOp;
    T       defaultValue;

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


    // O(N)
    void build(T value, int n) {
        N = n;

        tree = vector<T>(N + 1, value);
        treeR = vector<T>(N + 1, value);
        tree[0] = defaultValue;
        treeR.back() = defaultValue;
        for (int step = 2; step <= n; step <<= 1) {
            int i, j;
            for (i = step >> 1, j = step; j <= n; i += step, j += step) {
                tree[j] = mergeOp(tree[j], tree[i]);
                treeR[i & (i - 1)] = mergeOp(treeR[i & (i - 1)], treeR[i]);
            }
            if (i < n)
                treeR[i & (i - 1)] = mergeOp(treeR[i & (i - 1)], treeR[i]);
        }
    }

    // O(N)
    void build(const T arr[], int n) {
        N = n;
        tree.clear();
        treeR.clear();

        tree.push_back(defaultValue);
        tree.insert(tree.end(), arr, arr + n);
        treeR.insert(treeR.end(), arr, arr + n);
        treeR.push_back(defaultValue);
        for (int step = 2; step <= n; step <<= 1) {
            int i, j;
            for (i = step >> 1, j = step; j <= n; i += step, j += step) {
                tree[j] = mergeOp(tree[j], tree[i]);
                treeR[i & (i - 1)] = mergeOp(treeR[i & (i - 1)], treeR[i]);
            }
            if (i < n)
                treeR[i & (i - 1)] = mergeOp(treeR[i & (i - 1)], treeR[i]);
        }
    }

    // O(N)
    void build(const vector<T>& v) {
        build(&v[0], int(v.size()));
    }


    void clear() {
        fill(tree.begin(), tree.end(), defaultValue);
        fill(treeR.begin(), treeR.end(), defaultValue);
    }

    //--- update

    // inclusive (0 <= pos < N), O(logN)
    void add(int pos, T val) {
        update(pos, query(pos) + val);
    }

    // inclusive (0 <= pos < N), O(logN)
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

    // inclusive (0 <= pos < N), O(logN)
    // - updateOp(prev value, new value)
    template <typename UpdateOp = function<T(T,T)>>
    void update(int pos, T val, const UpdateOp& updateOp) {
        int curr = pos & ~1;
        int prev = pos | 1;

        if (pos & 1)
            treeR[prev] = updateOp(treeR[prev], val);
        else
            tree[prev] = updateOp(treeR[prev], val);;

        int mask = 2;
        for (curr |= mask; curr <= N; prev = curr, curr = (curr & (curr - 1)) | mask) {
            if (prev & mask)
                treeR[curr] = mergeOp(tree[prev], treeR[prev]);
            else
                tree[curr] = mergeOp(tree[prev], treeR[prev]);
            mask <<= 1;
        }
    }

    // O(|right - left| + logN)
    void add(int left, int right, T val) {
        for (int L = (left + 1) | 1, R = right + 1; L <= R; L += 2)
            tree[L] += val;

        for (int L = left | 1, R = right; L <= R; L += 2)
            treeR[L] += val;

        rebuild(left, right);
    }

    // O(|right - left| + logN)
    void update(int left, int right, T val) {
        for (int L = (left + 1) | 1, R = right + 1; L <= R; L += 2)
            tree[L] = val;

        for (int L = left | 1, R = right; L <= R; L += 2)
            treeR[L] = val;

        rebuild(left, right);
    }

    //--- query

    // inclusive (0 <= pos < N), O(logN)
    T query(int pos) const {
        return (pos & 1) ? treeR[pos] : tree[pos + 1];
    }

    // inclusive (0 <= left <= right < N), O(logN)
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

    //--- lower bound

    // PRECONDITION: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
    // lower bound where f(x) is true in [0, N)
    //   f(x): xxxxxxxxxxxOOOOOOOO
    //         S          ^
    // O(logN)
    int lowerBound(const function<bool(T)>& f) const {
        int N = int(tree.size()) - 1;

        int blockSize = N;
        while (blockSize & (blockSize - 1))
            blockSize &= blockSize - 1;

        T delta = defaultValue;

        int lo = 0;
        for (; blockSize > 0; blockSize >>= 1) {
            int next = lo + blockSize;
            if (next <= N) {
                int val = mergeOp(delta, tree[next]);
                if (!f(val)) {
                    delta = val;
                    lo = next;
                }
            }
        }

        return lo;
    }

    // PRECONDITION: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
    // lower bound where f(x) is true in [0, N)
    //    oooooooOOOOOOxxxxxx
    //    L           ^     R
    // O(logN)
    int lowerBoundBackward(const function<bool(T)>& f) const {
        int N = int(tree.size()) - 1;

        int blockSize = N;
        while (blockSize & (blockSize - 1))
            blockSize &= blockSize - 1;

        T delta = defaultValue;

        int lo = 0;
        for (; blockSize > 0; blockSize >>= 1) {
            int next = lo + blockSize;
            if (next <= N) {
                int val = mergeOp(delta, tree[next]);
                if (!f(val)) {
                    delta = val;
                    lo = next;
                }
            }
        }

        return lo;
    }

private:
    // O(|right - left| + logN)
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

// PRECONDITION: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// find next position where f(x) is true in [start, N)
//   f(x): xxxxxxxxxxxOOOOOOOO
//         S          ^
// O(logN)
template <typename T, typename MergeOp>
inline int findNext(const GeneralizedBIT<T, MergeOp>& gbit, int start, const function<bool(T)>& f) {
    int pos = start;
    while (pos <= gbit.N) {
        if ((pos & 1) == 0) {
            pos++;
            if (f(gbit.tree[pos]))
                return pos - 1;
        }
        if (pos <= gbit.N && f(gbit.treeR[pos])) {
            return pos;
            pos += pos & -pos;
        }

        while (pos <= gbit.N && !f(gbit.treeR[pos]))
            pos += pos & -pos;
    }

    return -1;
}

// PRECONDITION: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// find previous position where f(x) is true in [0, start]
//   f(x): OOOOOOOOxxxxxxxxxxx
//                ^          S
// O(logN)
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


// PRECONDITION: tree's range operation is monotonically increasing
// return min(x | query(left, i) >= value, left <= i <= right)
//    xxxxxxxOOOOOOoooooo
//    L      ^          R
// O((logN)^2)
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
// O((logN)^2)
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
// O((logN)^2)
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
// O((logN)^2)
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
