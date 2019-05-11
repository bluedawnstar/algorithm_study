#pragma once

template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct SimplePersistentSegmentTreeLazy {
    struct Node {
        T   value;
        int L;
        int R;

        T   lazy;
    };

    int             N;
    vector<Node>    nodes;
    int             initRoot;

    T               defaultValue;
    MergeOp         mergeOp;
    BlockOp         blockOp;

    SimplePersistentSegmentTreeLazy(MergeOp mop, BlockOp bop, T dflt = T())
            : N(0), nodes(), initRoot(-1), defaultValue(dflt), mergeOp(mop), blockOp(bop) {
    }

    SimplePersistentSegmentTreeLazy(int n, MergeOp mop, BlockOp bop, T dflt = T())
            : N(0), nodes(), initRoot(-1), defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(n);
    }

    // return root node index
    int build(int n) {
        nodes.clear();

        N = n;
        nodes.reserve(N * 4);
        initRoot = dfsBuild(0, N - 1);
        return initRoot;
    }

    //---

    int getInitRoot() const {
        return initRoot;
    }

    // return root node index, O(logN)
    int update(int root, int index, T val) {
        return dfsUpdate(root, 0, N - 1, index, index, val);
    }

    // return root node index, O(logN)
    int update(int root, int left, int right, T val) {
        return dfsUpdate(root, 0, N - 1, left, right, val);
    }

    // O(logN)
    T query(int root, int left, int right) {
        return dfsQuery(root, 0, N - 1, left, right);
    }

private:
    int add(T value, int L, int R, T lazy) {
        nodes.push_back({ value, L, R, lazy });
        return int(nodes.size()) - 1;
    }

    int pushDown(int node, int nodeLeft, int nodeRight, T val) {
        if (val == defaultValue)
            return node;

        T newValue = nodes[node].value;
        T lazy = nodes[node].lazy;
        int l = nodes[node].L;
        int r = nodes[node].R;

        if (nodeLeft == nodeRight) {
            newValue = mergeOp(newValue, val);
        } else {
            lazy = mergeOp(lazy, val);
            newValue = mergeOp(newValue, blockOp(val, nodeRight - nodeLeft + 1));
        }
        return add(newValue, l, r, lazy);
    }

    int dfsBuild(int nodeLeft, int nodeRight) {
        int l = -1, r = -1;
        if (nodeLeft < nodeRight) {
            int mid = (nodeLeft + nodeRight) >> 1;
            l = dfsBuild(nodeLeft, mid);
            r = dfsBuild(mid + 1, nodeRight);
        }
        nodes.push_back({ defaultValue, l, r, defaultValue });
        return int(nodes.size()) - 1;
    }

    int dfsUpdate(int node, int nodeLeft, int nodeRight, int indexL, int indexR, T val) {
        if (indexR < nodeLeft || nodeRight < indexL)
            return node;

        T newValue = nodes[node].value;
        int l = nodes[node].L;
        int r = nodes[node].R;
        T lazy = nodes[node].lazy;

        if (nodeLeft == nodeRight) {
            newValue = mergeOp(nodes[node].value, val);
        } else {
            if (indexL <= nodeLeft && nodeRight <= indexR) {
                lazy = mergeOp(lazy, val);
                newValue = mergeOp(nodes[node].value, blockOp(val, nodeRight - nodeLeft + 1));
            } else {
                int mid = (nodeLeft + nodeRight) >> 1;
                if (lazy != defaultValue) {
                    l = pushDown(l, nodeLeft, mid, lazy);
                    r = pushDown(r, mid + 1, nodeRight, lazy);
                    lazy = defaultValue;
                }
                l = dfsUpdate(l, nodeLeft, mid, indexL, indexR, val);
                r = dfsUpdate(r, mid + 1, nodeRight, indexL, indexR, val);
                newValue = mergeOp(nodes[l].value, nodes[r].value);
            }
        }
        return add(newValue, l, r, lazy);
    }

    T dfsQuery(int node, int nodeLeft, int nodeRight, int indexL, int indexR) {
        if (indexR < nodeLeft || nodeRight < indexL)
            return defaultValue;

        if (indexL <= nodeLeft && nodeRight <= indexR)
            return nodes[node].value;

        int mid = (nodeLeft + nodeRight) >> 1;
        int l = nodes[node].L;
        int r = nodes[node].R;
        T lazy = nodes[node].lazy;
        if (lazy != defaultValue) {
            l = pushDown(l, nodeLeft, mid, lazy);
            r = pushDown(r, mid + 1, nodeRight, lazy);

            nodes[node].value = mergeOp(nodes[l].value, nodes[r].value);
            nodes[node].L = l;
            nodes[node].R = r;
            nodes[node].lazy = defaultValue;
        }

        return mergeOp(dfsQuery(l, nodeLeft, mid, indexL, indexR),
                       dfsQuery(r, mid + 1, nodeRight, indexL, indexR));
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline SimplePersistentSegmentTreeLazy<T, MergeOp, BlockOp>
makeSimplePersistentSegmentTreeLazy(MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return SimplePersistentSegmentTreeLazy<T, MergeOp, BlockOp>(mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline SimplePersistentSegmentTreeLazy<T, MergeOp, BlockOp>
makeSimplePersistentSegmentTreeLazy(int n, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return SimplePersistentSegmentTreeLazy<T, MergeOp, BlockOp>(n, mop, bop, dfltValue);
}
