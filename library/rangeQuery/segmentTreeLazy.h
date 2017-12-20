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

    SegmentTreeLazy(int size, T dflt = T())
        : N(size), tree(size * 4, dflt), treeLazy(size * 4, dflt), lazyExist(size * 4),
          defaultValue(dflt), mergeOp(), blockOp() {
    }

    SegmentTreeLazy(int size, MergeOp mop, BlockOp bop, T dflt = T())
        : N(size), tree(size * 4, dflt), treeLazy(size * 4, dflt), lazyExist(size * 4),
          defaultValue(dflt), mergeOp(mop), blockOp(bop) {
    }

    // inclusive
    T build(const vector<T>& v, int left, int right) {
        return buildSub(v, left, right, 1);
    }
    // inclusive
    T build(const T arr[], int left, int right) {
        return buildSub(arr, left, right, 1);
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
    T updateRange(int left, int right, T newValue) {
        return updateRangeSub(left, right, newValue, 1, 0, N - 1);
    }

private:
    // inclusive
    template <typename U>
    T buildSub(const U& arr, int left, int right, int node) {
        if (left > right)
            return defaultValue;

        if (left == right)
            return tree[node] = arr[left];

        int mid = left + (right - left) / 2;
        T leftSum = buildSub<U>(arr, left, mid, node * 2);
        T rightSum = buildSub<U>(arr, mid + 1, right, node * 2 + 1);

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

    T updateRangeSub(int left, int right, T newValue, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
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

        if (left <= nodeLeft && nodeRight <= right) {
            lazyExist[node] = true;
            treeLazy[node] = newValue;
            return tree[node] = blockOp(newValue, nodeRight - nodeLeft + 1);
        }

        return tree[node] = mergeOp(updateRangeSub(left, right, newValue, node * 2, nodeLeft, mid),
                                    updateRangeSub(left, right, newValue, node * 2 + 1, mid + 1, nodeRight));
    }

    T pushDown(T newValue, int node, int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight)
            return tree[node] = newValue;

        lazyExist[node] = true;
        treeLazy[node] = newValue;
        return tree[node] = blockOp(newValue, nodeRight - nodeLeft + 1);
    }
};

template <typename T, typename MergeOp, typename BlockOp>
SegmentTreeLazy<T, MergeOp, BlockOp> makeSegmentTreeLazy(int size, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return SegmentTreeLazy<T, MergeOp, BlockOp>(size, mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
SegmentTreeLazy<T, MergeOp, BlockOp> makeSegmentTreeLazy(const vector<T>& v, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    auto segTree = SegmentTreeLazy<T, MergeOp, BlockOp>((int)v.size(), mop, bop, dfltValue);
    segTree.build(v, 0, (int)v.size() - 1);
    return segTree;
}

template <typename T, typename MergeOp, typename BlockOp>
SegmentTreeLazy<T, MergeOp, BlockOp> makeSegmentTreeLazy(const T arr[], int size, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    auto segTree = SegmentTreeLazy<T, MergeOp, BlockOp>(size, mop, bop, dfltValue);
    segTree.build(arr, 0, size - 1);
    return segTree;
}
