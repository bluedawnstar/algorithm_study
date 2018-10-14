#pragma once

template <typename T, typename MergeOp = function<T(T,T)>>
struct SimplePersistentSegmentTree {
    int N;
    vector<T> value;
    vector<int> L;
    vector<int> R;
    vector<int> roots;

    MergeOp mergeOp;
    T defaultValue;

    explicit SimplePersistentSegmentTree(MergeOp op, T dflt = T()) : mergeOp(op), defaultValue(dflt) {
    }

    SimplePersistentSegmentTree(int n, MergeOp op, T dflt = T()) : mergeOp(op), defaultValue(dflt) {
        build(n);
    }

    // return root node index
    int build(int n) {
        N = n;
        value.reserve(N * 4);
        L.reserve(N * 4);
        R.reserve(N * 4);

        int r = dfsBuild(0, N - 1);
        roots.push_back(r);
        return r;
    }

    //---

    int getInitRoot() const {
        return roots.front();
    }

    // return root node index, O(logN)
    int update(int root, int index, T val) {
        int r = dfsUpdate(root, 0, N - 1, index, val);
        roots.push_back(r);
        return r;
    }

    // return root node index, O(logN)
    int set(int root, int index, T val) {
        int r = dfsSet(root, 0, N - 1, index, val);
        roots.push_back(r);
        return r;
    }

    // O(logN)
    T query(int root, int left, int right) const {
        return dfsQuery(root, 0, N - 1, left, right);
    }

    // PRECONDITION: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
    // lower bound where f(x) is true in [0, N)
    //   f(x): xxxxxxxxxxxOOOOOOOO
    //         S          ^
    // O(logN)
    int lowerBound(int root, const function<bool(T)>& f) const {
        return dfsLowerBound(f, defaultValue, root, 0, N - 1);
    }

private:
    int dfsBuild(int left, int right) {
        int l = -1, r = -1;
        if (left < right) {
            int mid = (left + right) >> 1;
            l = dfsBuild(left, mid);
            r = dfsBuild(mid + 1, right);
        }
        value.push_back(defaultValue);
        L.push_back(l);
        R.push_back(r);
        return int(value.size()) - 1;
    }

    int dfsUpdate(int node, int left, int right, int index, T val) {
        T newValue;
        int l = -1, r = -1;
        if (left == right) {
            newValue = mergeOp(value[node], val);   // merge
        } else {
            int mid = (left + right) >> 1;
            if (index <= mid) {
                l = dfsUpdate(L[node], left, mid, index, val);
                r = R[node];
            } else {
                l = L[node];
                r = dfsUpdate(R[node], mid + 1, right, index, val);
            }
            newValue = mergeOp(value[l], value[r]);
        }
        value.push_back(newValue);
        L.push_back(l);
        R.push_back(r);
        return int(value.size()) - 1;
    }

    int dfsSet(int node, int left, int right, int index, T val) {
        T newValue;
        int l = -1, r = -1;
        if (left == right) {
            newValue = val;                         // set
        } else {
            int mid = (left + right) >> 1;
            if (index <= mid) {
                l = dfsSet(L[node], left, mid, index, val);
                r = R[node];
            } else {
                l = L[node];
                r = dfsSet(R[node], mid + 1, right, index, val);
            }
            newValue = mergeOp(value[l], value[r]);
        }
        value.push_back(newValue);
        L.push_back(l);
        R.push_back(r);
        return int(value.size()) - 1;
    }

    T dfsQuery(int node, int left, int right, int indexL, int indexR) const {
        if (indexR < left || right < indexL)
            return defaultValue;

        if (indexL <= left && right <= indexR)
            return value[node];

        int mid = (left + right) >> 1;
        return mergeOp(dfsQuery(L[node], left, mid, indexL, indexR),
                       dfsQuery(R[node], mid + 1, right, indexL, indexR));
    }

    int dfsLowerBound(const function<bool(T)>& f, T delta, int node, int left, int right) const {
        if (left > right)
            return left;

        if (left == right)
            return left + (f(mergeOp(delta, value[node])) ? 0 : 1);

        int mid = (left + right) >> 1;
        auto val = mergeOp(delta, value[L[node]]);
        if (f(val))
            return dfsLowerBound(f, delta, L[node], left, mid);
        else
            return dfsLowerBound(f, val, R[node], mid + 1, right);
    }
};

template <typename T, typename MergeOp>
inline SimplePersistentSegmentTree<T, MergeOp> makeSimplePersistentSegmentTree(MergeOp op, T dfltValue = T()) {
    return SimplePersistentSegmentTree<T, MergeOp>(op, dfltValue);
}

template <typename T, typename MergeOp>
inline SimplePersistentSegmentTree<T, MergeOp> makeSimplePersistentSegmentTree(int n, MergeOp op, T dfltValue = T()) {
    return SimplePersistentSegmentTree<T, MergeOp>(n, op, dfltValue);
}
