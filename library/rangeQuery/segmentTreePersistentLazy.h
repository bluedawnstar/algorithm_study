#pragma once

// The first 'node' number is 1, not 0
// Others('left', 'right', 'nodeLeft', 'nodeRight', 'index') are started from 0
template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct PersistentSegmentTreeLazy {
    struct Node {
        T   value;
        int L;
        int R;

        T   lazy;

        Node() {
        }

        Node(T value, T lazy) : value(value), L(-1), R(-1), lazy(lazy) {
        }

        Node(T value, int L, int R, T lazy) : value(value), L(L), R(R), lazy(lazy) {
        }
    };

    int             N;
    vector<Node>    nodes;
    int             initRoot;

    T               defaultValue;
    MergeOp         mergeOp;
    BlockOp         blockOp;

    PersistentSegmentTreeLazy(MergeOp mop, BlockOp bop, T dflt = T())
            : N(0), nodes(), initRoot(-1), defaultValue(dflt), mergeOp(mop), blockOp(bop) {
    }

    PersistentSegmentTreeLazy(int n, MergeOp mop, BlockOp bop, T dflt = T())
            : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(defaultValue, n);
    }

    PersistentSegmentTreeLazy(T value, int n, MergeOp mop, BlockOp bop, T dflt = T())
            : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(value, n);
    }

    PersistentSegmentTreeLazy(const T A[], int n, MergeOp mop, BlockOp bop, T dflt = T())
            : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(A, n);
    }

    PersistentSegmentTreeLazy(const vector<T>& A, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(A);
    }

    // return root node index
    int build(T value, int n) {
        nodes.clear();

        N = n;
        nodes.reserve(N * 4);
        return initRoot = recBuild(value, 0, N - 1);
    }

    int build(const T A[], int n) {
        nodes.clear();

        N = n;
        nodes.reserve(N * 4);
        return initRoot = recBuild(A, 0, N - 1);
    }

    int build(int n) {
        return build(defaultValue, n);
    }

    int build(const vector<T>& A) {
        return build(A.data(), int(A.size()));
    }

    //---

    int getInitRoot() const {
        return initRoot;
    }

    // return root node index, O(logN)
    int set(int root, int index, T val) {
        return recSet(root, 0, N - 1, index, index, val);
    }

    // return root node index, O(logN)
    int update(int root, int index, T val) {
        return recUpdate(root, 0, N - 1, index, index, val);
    }

    // return root node index, O(logN)
    int update(int root, int left, int right, T val) {
        return recUpdate(root, 0, N - 1, left, right, val);
    }

    // O(logN)
    T query(int root, int left, int right) {
        return recQuery(root, 0, N - 1, left, right);
    }

    // PRECONDITION: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
    // lower bound where f(x) is true in [0, N)
    //   f(x): xxxxxxxxxxxOOOOOOOO
    //         S          ^
    // O(logN)
    int lowerBound(int root, const function<bool(T)>& f) {
        return recLowerBound(root, f, defaultValue, 0, N - 1);
    }

private:
    int recBuild(T value, int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight) {
            nodes.emplace_back(value, -1, -1, defaultValue);
        } else {
            int mid = (nodeLeft + nodeRight) >> 1;
            int L = recBuild(value, nodeLeft, mid);
            int R = recBuild(value, mid + 1, nodeRight);
            nodes.emplace_back(mergeOp(nodes[L].value, nodes[R].value), L, R, defaultValue);
        }
        return int(nodes.size()) - 1;
    }

    int recBuild(const T A[], int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight) {
            nodes.emplace_back(A[nodeLeft], -1, -1, defaultValue);
        } else {
            int mid = (nodeLeft + nodeRight) >> 1;
            int L = recBuild(A, nodeLeft, mid);
            int R = recBuild(A, mid + 1, nodeRight);
            nodes.emplace_back(mergeOp(nodes[L].value, nodes[R].value), L, R, defaultValue);
        }
        return int(nodes.size()) - 1;
    }

    //---

    int add(T value, int L, int R, T lazy) {
        nodes.emplace_back(value, L, R, lazy);
        return int(nodes.size()) - 1;
    }

    int pushDown(int node, int nodeLeft, int nodeRight, T val) {
        if (val == defaultValue)
            return node;

        T value = nodes[node].value;

        if (nodeLeft == nodeRight)
            return add(mergeOp(value, val), -1, -1, defaultValue);

        return add(mergeOp(value, blockOp(val, nodeRight - nodeLeft + 1)),
                   nodes[node].L,
                   nodes[node].R,
                   mergeOp(nodes[node].lazy, val));
    }

    int recSet(int node, int nodeLeft, int nodeRight, int indexL, int indexR, T val) {
        if (indexR < nodeLeft || nodeRight < indexL)
            return node;

        if (nodeLeft == nodeRight)
            return add(val, -1, -1, defaultValue);

        T value = nodes[node].value;
        int L = nodes[node].L;
        int R = nodes[node].R;
        T lazy = nodes[node].lazy;

        if (indexL <= nodeLeft && nodeRight <= indexR) {
            value = mergeOp(value, blockOp(val, nodeRight - nodeLeft + 1));
            lazy = mergeOp(lazy, val);
        } else {
            int mid = (nodeLeft + nodeRight) >> 1;
            if (lazy != defaultValue) {
                L = pushDown(L, nodeLeft, mid, lazy);
                R = pushDown(R, mid + 1, nodeRight, lazy);
                lazy = defaultValue;
            }
            L = recSet(L, nodeLeft, mid, indexL, indexR, val);
            R = recSet(R, mid + 1, nodeRight, indexL, indexR, val);
            value = mergeOp(nodes[L].value, nodes[R].value);
        }
        return add(value, L, R, lazy);
    }

    int recUpdate(int node, int nodeLeft, int nodeRight, int indexL, int indexR, T val) {
        if (indexR < nodeLeft || nodeRight < indexL)
            return node;

        T value = nodes[node].value;

        if (nodeLeft == nodeRight)
            return add(mergeOp(value, val), -1, -1, defaultValue);

        int L = nodes[node].L;
        int R = nodes[node].R;
        T lazy = nodes[node].lazy;

        if (indexL <= nodeLeft && nodeRight <= indexR) {
            value = mergeOp(value, blockOp(val, nodeRight - nodeLeft + 1));
            lazy = mergeOp(lazy, val);
        } else {
            int mid = (nodeLeft + nodeRight) >> 1;
            if (lazy != defaultValue) {
                L = pushDown(L, nodeLeft, mid, lazy);
                R = pushDown(R, mid + 1, nodeRight, lazy);
                lazy = defaultValue;
            }
            L = recUpdate(L, nodeLeft, mid, indexL, indexR, val);
            R = recUpdate(R, mid + 1, nodeRight, indexL, indexR, val);
            value = mergeOp(nodes[L].value, nodes[R].value);
        }
        return add(value, L, R, lazy);
    }

    T recQuery(int node, int nodeLeft, int nodeRight, int indexL, int indexR) {
        if (indexR < nodeLeft || nodeRight < indexL)
            return defaultValue;

        if (indexL <= nodeLeft && nodeRight <= indexR)
            return nodes[node].value;

        int mid = (nodeLeft + nodeRight) >> 1;
        int L = nodes[node].L;
        int R = nodes[node].R;
        T lazy = nodes[node].lazy;
        if (lazy != defaultValue) {
            L = pushDown(L, nodeLeft, mid, lazy);
            R = pushDown(R, mid + 1, nodeRight, lazy);

            nodes[node].value = mergeOp(nodes[L].value, nodes[R].value);
            nodes[node].L = L;
            nodes[node].R = R;
            nodes[node].lazy = defaultValue;
        }

        return mergeOp(recQuery(L, nodeLeft, mid, indexL, indexR),
                       recQuery(R, mid + 1, nodeRight, indexL, indexR));
    }

    int recLowerBound(int node, const function<bool(T)>& f, T delta, int nodeLeft, int nodeRight) {
        if (nodeLeft > nodeRight)
            return nodeLeft;

        if (nodeLeft == nodeRight)
            return nodeLeft + (f(mergeOp(delta, nodes[node].value)) ? 0 : 1);

        int mid = (nodeLeft + nodeRight) >> 1;
        int L = nodes[node].L;
        int R = nodes[node].R;
        T lazy = nodes[node].lazy;
        if (lazy != defaultValue) {
            L = pushDown(L, nodeLeft, mid, lazy);
            R = pushDown(R, mid + 1, nodeRight, lazy);

            nodes[node].value = mergeOp(nodes[L].value, nodes[R].value);
            nodes[node].L = L;
            nodes[node].R = R;
            nodes[node].lazy = defaultValue;
        }

        auto val = mergeOp(delta, nodes[L].value);
        if (f(val))
            return recLowerBound(L, f, delta, nodeLeft, mid);
        else
            return recLowerBound(R, f, val, mid + 1, nodeRight);
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline PersistentSegmentTreeLazy<T, MergeOp, BlockOp>
makePersistentSegmentTreeLazy(MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return PersistentSegmentTreeLazy<T, MergeOp, BlockOp>(mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline PersistentSegmentTreeLazy<T, MergeOp, BlockOp>
makePersistentSegmentTreeLazy(int n, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return PersistentSegmentTreeLazy<T, MergeOp, BlockOp>(n, mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline PersistentSegmentTreeLazy<T, MergeOp, BlockOp>
makePersistentSegmentTreeLazy(T value, int n, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return PersistentSegmentTreeLazy<T, MergeOp, BlockOp>(value, n, mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline PersistentSegmentTreeLazy<T, MergeOp, BlockOp>
makePersistentSegmentTreeLazy(const T A[], int n, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return PersistentSegmentTreeLazy<T, MergeOp, BlockOp>(A, n, mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline PersistentSegmentTreeLazy<T, MergeOp, BlockOp>
makePersistentSegmentTreeLazy(const vector<T>& A, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return PersistentSegmentTreeLazy<T, MergeOp, BlockOp>(A, mop, bop, dfltValue);
}
