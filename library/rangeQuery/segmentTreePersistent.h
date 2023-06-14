#pragma once

template <typename T, typename MergeOp = function<T(T,T)>>
struct PersistentSegmentTree {
    struct Node {
        T   value;
        int L;
        int R;

        Node() {
        }

        explicit Node(T value) : value(value), L(-1), R(-1) {
        }

        Node(T value, int L, int R) : value(value), L(L), R(R) {
        }
    };

    int             N;
    vector<Node>    nodes;
    int             initRoot;

    MergeOp         mergeOp;
    T               defaultValue;

    explicit PersistentSegmentTree(MergeOp op, T dflt = T()) : N(0), initRoot(-1), mergeOp(op), defaultValue(dflt) {
    }

    PersistentSegmentTree(int n, MergeOp op, T dflt = T()) : mergeOp(op), defaultValue(dflt) {
        build(defaultValue, n);
    }

    PersistentSegmentTree(T value, int n, MergeOp op, T dflt = T()) : mergeOp(op), defaultValue(dflt) {
        build(value, n);
    }

    PersistentSegmentTree(const T A[], int n, MergeOp op, T dflt = T()) : mergeOp(op), defaultValue(dflt) {
        build(A, n);
    }

    PersistentSegmentTree(const vector<T>& A, MergeOp op, T dflt = T()) : mergeOp(op), defaultValue(dflt) {
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
    int update(int root, int index, T val) {
        return recUpdate(root, 0, N - 1, index, val);
    }

    // return root node index, O(logN)
    int set(int root, int index, T val) {
        return recSet(root, 0, N - 1, index, val);
    }

    // O(logN)
    T query(int root, int left, int right) const {
        return recQuery(root, 0, N - 1, left, right);
    }

    // PRECONDITION: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
    // lower bound where f(x) is true in [0, N)
    //   f(x): xxxxxxxxxxxOOOOOOOO
    //         S          ^
    // O(logN)
    int lowerBound(int root, const function<bool(T)>& f) const {
        return recLowerBound(f, defaultValue, root, 0, N - 1);
    }

private:
    int recBuild(T value, int left, int right) {
        if (left == right) {
            nodes.emplace_back(value, -1, -1);
        } else {
            int mid = (left + right) >> 1;
            int L = recBuild(value, left, mid);
            int R = recBuild(value, mid + 1, right);
            nodes.emplace_back(mergeOp(nodes[L].value, nodes[R].value), L, R);
        }
        return int(nodes.size()) - 1;
    }

    int recBuild(const T A[], int left, int right) {
        if (left == right) {
            nodes.emplace_back(A[left], -1, -1);
        } else {
            int mid = (left + right) >> 1;
            int L = recBuild(A, left, mid);
            int R = recBuild(A, mid + 1, right);
            nodes.emplace_back(mergeOp(nodes[L].value, nodes[R].value), L, R);
        }
        return int(nodes.size()) - 1;
    }

    int recUpdate(int node, int left, int right, int index, T val) {
        T newValue;
        int L = -1, R = -1;
        if (left == right) {
            newValue = mergeOp(nodes[node].value, val);
        } else {
            int mid = (left + right) >> 1;
            if (index <= mid) {
                L = recUpdate(nodes[node].L, left, mid, index, val);
                R = nodes[node].R;
            } else {
                L = nodes[node].L;
                R = recUpdate(nodes[node].R, mid + 1, right, index, val);
            }
            newValue = mergeOp(nodes[L].value, nodes[R].value);
        }
        nodes.emplace_back(newValue, L, R);
        return int(nodes.size()) - 1;
    }

    int recSet(int node, int left, int right, int index, T val) {
        T newValue;
        int L = -1, R = -1;
        if (left == right) {
            newValue = val;
        } else {
            int mid = (left + right) >> 1;
            if (index <= mid) {
                L = recSet(nodes[node].L, left, mid, index, val);
                R = nodes[node].R;
            } else {
                L = nodes[node].L;
                R = recSet(nodes[node].R, mid + 1, right, index, val);
            }
            newValue = mergeOp(nodes[L].value, nodes[R].value);
        }
        nodes.emplace_back(newValue, L, R);
        return int(nodes.size()) - 1;
    }

    T recQuery(int node, int left, int right, int indexL, int indexR) const {
        if (indexR < left || right < indexL)
            return defaultValue;

        if (indexL <= left && right <= indexR)
            return nodes[node].value;

        int mid = (left + right) >> 1;
        return mergeOp(recQuery(nodes[node].L, left, mid, indexL, indexR),
                       recQuery(nodes[node].R, mid + 1, right, indexL, indexR));
    }

    int recLowerBound(const function<bool(T)>& f, T delta, int node, int left, int right) const {
        if (left > right)
            return left;

        if (left == right)
            return left + (f(mergeOp(delta, nodes[node].value)) ? 0 : 1);

        int mid = (left + right) >> 1;
        auto val = mergeOp(delta, nodes[nodes[node].L].value);
        if (f(val))
            return recLowerBound(f, delta, nodes[node].L, left, mid);
        else
            return recLowerBound(f, val, nodes[node].R, mid + 1, right);
    }
};

template <typename T, typename MergeOp>
inline PersistentSegmentTree<T, MergeOp> makePersistentSegmentTree(MergeOp op, T dfltValue = T()) {
    return PersistentSegmentTree<T, MergeOp>(op, dfltValue);
}

template <typename T, typename MergeOp>
inline PersistentSegmentTree<T, MergeOp> makePersistentSegmentTree(int n, MergeOp op, T dfltValue = T()) {
    return PersistentSegmentTree<T, MergeOp>(n, op, dfltValue);
}

template <typename T, typename MergeOp>
inline PersistentSegmentTree<T, MergeOp> makePersistentSegmentTree(const vector<T>& v, MergeOp op, T dfltValue = T()) {
    return PersistentSegmentTree<T, MergeOp>(v, op, dfltValue);
}

template <typename T, typename MergeOp>
inline PersistentSegmentTree<T, MergeOp> makePersistentSegmentTree(const T A[], int n, MergeOp op, T dfltValue = T()) {
    return PersistentSegmentTree<T, MergeOp>(A, n, op, dfltValue);
}

//-----------------------------------------------------------------------------

// find kth value from Persistent Segment Tree
// 
// PRECONDITION: tree.value[x] is the number of value 'x',  0 <= x < N
// Parameters:
//      segTreeRoots[i] = (the index of a root node, delta), delta is +1 or -1
//      1 <= kth <= N
// 
// O(klogN)
template <typename T = int, typename MergeOp = function<T(T, T)>>
T findKthValue(PersistentSegmentTree<T, MergeOp>& segTree, T maxValue, vector<pair<int, int>>& segTreeRoots, T kth) {
    T lo = 0, hi = maxValue;
    while (lo < hi) {
        T qt = 0;
        for (auto& r : segTreeRoots)
            qt += r.second * segTree.nodes[segTree.nodes[r.first].L].value;

        T mid = (lo + hi) >> 1;
        if (kth <= qt) {
            for (auto& r : segTreeRoots)
                r.first = segTree.nodes[r.first].L;
            hi = mid;
        } else {
            kth -= qt;
            for (auto& r : segTreeRoots)
                r.first = segTree.nodes[r.first].R;
            lo = mid + 1;
        }
    }

    T qt = 0;
    for (auto& r : segTreeRoots)
        qt += r.second * segTree.nodes[r.first].value;

    return kth <= qt ? lo : T(-1);
}
