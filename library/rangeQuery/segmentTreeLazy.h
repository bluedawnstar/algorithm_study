#pragma once

// The first 'node' number is 1, not 0
// Others('left', 'right', 'nodeLeft', 'nodeRight', 'index') are started from 0
template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct SegmentTreeLazy {
    int       N;                // the size of array
    vector<T> tree;             // 
    vector<T> treeLazy;         // 
    vector<bool> lazyExist;     // 

    T         defaultValue;
    MergeOp   mergeOp;
    BlockOp   blockOp;

    SegmentTreeLazy(MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
    }

    SegmentTreeLazy(int size, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        init(size);
    }

    SegmentTreeLazy(T value, int n, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(value, n);
    }

    SegmentTreeLazy(const T arr[], int n, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(arr, n);
    }

    SegmentTreeLazy(const vector<T> &v, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(v);
    }


    void init(int size) {
        N = size;
        tree.assign(size * 4, defaultValue);
        treeLazy.assign(size * 4, defaultValue);
        lazyExist.assign(size * 4, false);
    }

    T build(T value, int n) {
        init(n);
        return buildSub(value, 0, n - 1, 1);
    }

    T build(const T arr[], int n) {
        init(n);
        return buildSub(arr, 0, n - 1, 1);
    }

    T build(const vector<T>& v) {
        return build(&v[0], int(v.size()));
    }


    // inclusive
    T query(int left, int right) {
        return querySub(left, right, 1, 0, N - 1);
    }

    // inclusive
    T update(int index, T newValue) {
        return updateSub(index, newValue, 1, 0, N - 1);
    }

    // inclusive
    T update(int left, int right, T newValue) {
        return updateSub(left, right, newValue, 1, 0, N - 1);
    }


    // PRECONDITION: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
    // lower bound where f(x) is true in [0, N)
    //   f(x): xxxxxxxxxxxOOOOOOOO
    //         S          ^
    pair<T, int> lowerBound(const function<bool(T)>& f) {
        return lowerBoundSub(f, T(0), 1, 0, N - 1);
    }

private:
    // inclusive
    T buildSub(const T arr[], int left, int right, int node) {
        if (left > right)
            return defaultValue;

        if (left == right)
            return tree[node] = arr[left];

        int mid = left + (right - left) / 2;
        T leftSum = buildSub(arr, left, mid, node * 2);
        T rightSum = buildSub(arr, mid + 1, right, node * 2 + 1);

        return tree[node] = mergeOp(leftSum, rightSum);
    }

    // inclusive
    T buildSub(T value, int left, int right, int node) {
        if (left > right)
            return defaultValue;

        if (left == right)
            return tree[node] = value;

        int mid = left + (right - left) / 2;
        T leftSum = buildSub(value, left, mid, node * 2);
        T rightSum = buildSub(value, mid + 1, right, node * 2 + 1);

        return tree[node] = mergeOp(leftSum, rightSum);
    }

    T querySub(int left, int right, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return defaultValue;

        if (left <= nodeLeft && nodeRight <= right)
            return tree[node];

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        if (lazyExist[node]) {
            lazyExist[node] = false;
            pushDown(treeLazy[node], node * 2, nodeLeft, mid);
            pushDown(treeLazy[node], node * 2 + 1, mid + 1, nodeRight);
            //treeLazy[node] = defaultValue;
        }
        return mergeOp(querySub(left, right, node * 2, nodeLeft, mid),
                       querySub(left, right, node * 2 + 1, mid + 1, nodeRight));
    }

    T updateSub(int index, T newValue, int node, int nodeLeft, int nodeRight) {
        if (index < nodeLeft || nodeRight < index)
            return tree[node];

        if (nodeLeft == nodeRight)
            return tree[node] = newValue;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        if (lazyExist[node]) {
            lazyExist[node] = false;
            pushDown(treeLazy[node], node * 2, nodeLeft, mid);
            pushDown(treeLazy[node], node * 2 + 1, mid + 1, nodeRight);
            //treeLazy[node] = defaultValue;
        }
        return tree[node] = mergeOp(updateSub(index, newValue, node * 2, nodeLeft, mid),
                                    updateSub(index, newValue, node * 2 + 1, mid + 1, nodeRight));
    }

    T updateSub(int left, int right, T newValue, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return tree[node];

        if (nodeLeft == nodeRight)
            return tree[node] = newValue;

#if 0
        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        if (lazyExist[node]) {
            lazyExist[node] = false;
            pushDown(treeLazy[node], node * 2, nodeLeft, mid);
            pushDown(treeLazy[node], node * 2 + 1, mid + 1, nodeRight);
            //treeLazy[node] = defaultValue;
        }

        if (left <= nodeLeft && nodeRight <= right) {
            lazyExist[node] = true;
            treeLazy[node] = newValue;
            return tree[node] = blockOp(newValue, nodeRight - nodeLeft + 1);
        }
#else
        if (left <= nodeLeft && nodeRight <= right) {
            lazyExist[node] = true;
            treeLazy[node] = newValue;
            return tree[node] = blockOp(newValue, nodeRight - nodeLeft + 1);
        }

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        if (lazyExist[node]) {
            lazyExist[node] = false;
            pushDown(treeLazy[node], node * 2, nodeLeft, mid);
            pushDown(treeLazy[node], node * 2 + 1, mid + 1, nodeRight);
            //treeLazy[node] = defaultValue;
        }
#endif

        return tree[node] = mergeOp(updateSub(left, right, newValue, node * 2, nodeLeft, mid),
                                    updateSub(left, right, newValue, node * 2 + 1, mid + 1, nodeRight));
    }

    T pushDown(T newValue, int node, int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight)
            return tree[node] = newValue;

        lazyExist[node] = true;
        treeLazy[node] = newValue;
        return tree[node] = blockOp(newValue, nodeRight - nodeLeft + 1);
    }


    pair<T, int> lowerBoundSub(const function<bool(T)>& f, T delta, int node, int nodeLeft, int nodeRight) {
        if (nodeLeft >= nodeRight)
            return make_pair(tree[node], nodeLeft);

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        if (lazyExist[node]) {
            lazyExist[node] = false;
            pushDown(treeLazy[node], node * 2, nodeLeft, mid);
            pushDown(treeLazy[node], node * 2 + 1, mid + 1, nodeRight);
            //treeLazy[node] = defaultValue;
        }

        auto val = mergeOp(delta, tree[node * 2]);
        if (f(val))
            return lowerBoundSub(f, delta, node * 2, nodeLeft, mid);
        else
            return lowerBoundSub(f, val, node * 2 + 1, mid + 1, nodeRight);
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline SegmentTreeLazy<T, MergeOp, BlockOp> makeSegmentTreeLazy(int size, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return SegmentTreeLazy<T, MergeOp, BlockOp>(size, mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline SegmentTreeLazy<T, MergeOp, BlockOp> makeSegmentTreeLazy(const vector<T>& v, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    auto segTree = SegmentTreeLazy<T, MergeOp, BlockOp>(v, mop, bop, dfltValue);
    return segTree;
}

template <typename T, typename MergeOp, typename BlockOp>
inline SegmentTreeLazy<T, MergeOp, BlockOp> makeSegmentTreeLazy(const T arr[], int size, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    auto segTree = SegmentTreeLazy<T, MergeOp, BlockOp>(arr, size, mop, bop, dfltValue);
    return segTree;
}
