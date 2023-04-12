#pragma once

/*
  1. operations
      1) set base
          B[i] = x                       ,  0 <= i < N
          A[i] = B[i]

      2) set delta
          A[i] = B[i] + x                ,  L <= i <= R

      3) update
          A[i] = mergeOp(A[i], B[i] + x) ,  L <= i <= R

          mergeOp = min | max

      4) query
          mergeOp(A[L], A[L + 1], ..., A[R])

  2. lazy operations
      1) value
          prev\next | lzSet   | lzUpdate
          ----------+---------+----------
           lzNone   | replace | merge (or replace)
           lzSet    | replace | merge
           lzUpdate | replace | merge

      2) lazy type
          prev\next | lzSet   | lzUpdate
          ----------+---------+----------
           lzNone   | lzSet   | lzUpdate
           lzSet    | lzSet   | lzSet (+ merge)
           lzUpdate | lzSet   | lzUpdate
*/
template <typename T, typename MergeOp = function<T(T, T)>>
struct RollbackablePersistentSegmentTreeLazyWithBase {
    enum LazyT {
        lzNone,
        lzSet,
        lzUpdate
    };

    struct Node {
        T     base;         // fixed base value
        T     value;        // value = base + delta
        int   L;
        int   R;

        T     lazy;
        LazyT lazyType;

        Node() {
        }

        Node(T base, T value, T lazy, LazyT lazyType)
            : base(base), value(value), L(-1), R(-1), lazy(lazy), lazyType(lazyType) {
        }

        Node(T base, T value, int L, int R, T lazy, LazyT lazyType)
            : base(base), value(value), L(L), R(R), lazy(lazy), lazyType(lazyType) {
        }
    };

    int             N;
    vector<Node>    nodes;
    int             initRoot;

    T               defaultValue;
    MergeOp         mergeOp;

    RollbackablePersistentSegmentTreeLazyWithBase(MergeOp mop, T dflt = T())
        : N(0), nodes(), initRoot(-1), defaultValue(dflt), mergeOp(mop) {
    }

    RollbackablePersistentSegmentTreeLazyWithBase(int n, MergeOp mop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop) {
        build(defaultValue, n);
    }

    RollbackablePersistentSegmentTreeLazyWithBase(T value, int n, MergeOp mop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop) {
        build(value, n);
    }

    RollbackablePersistentSegmentTreeLazyWithBase(const T A[], int n, MergeOp mop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop) {
        build(A, n);
    }

    RollbackablePersistentSegmentTreeLazyWithBase(const vector<T>& A, MergeOp mop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop) {
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
    int setBase(int root, int index, T val) {
        return recSetBase(root, 0, N - 1, index, val);
    }

    // return root node index, O(logN)
    int set(int root, int left, int right, T val) {
        return recSet(root, 0, N - 1, left, right, val);
    }

    // return root node index, O(logN)
    int update(int root, int left, int right, T val) {
        return recUpdate(root, 0, N - 1, left, right, val);
    }

    // O(logN)
    pair<T,int> query(int root, int left, int right) {
        return recQuery(root, 0, N - 1, left, right);
    }

    //---

    int checkPoint() {
        return int(nodes.size());
    }

    void rollback(int chk) {
        nodes.resize(chk);
    }

private:
    int recBuild(T value, int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight) {
            nodes.emplace_back(value, value, -1, -1, defaultValue, lzNone);
        } else {
            int mid = (nodeLeft + nodeRight) >> 1;
            int L = recBuild(value, nodeLeft, mid);
            int R = recBuild(value, mid + 1, nodeRight);
            nodes.emplace_back(mergeOp(nodes[L].base, nodes[R].base),
                               mergeOp(nodes[L].value, nodes[R].value),
                               L, R, defaultValue, lzNone);
        }
        return int(nodes.size()) - 1;
    }

    int recBuild(const T A[], int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight) {
            nodes.emplace_back(A[nodeLeft], A[nodeLeft], -1, -1, defaultValue, lzNone);
        } else {
            int mid = (nodeLeft + nodeRight) >> 1;
            int L = recBuild(A, nodeLeft, mid);
            int R = recBuild(A, mid + 1, nodeRight);
            nodes.emplace_back(mergeOp(nodes[L].base, nodes[R].base),
                               mergeOp(nodes[L].value, nodes[R].value),
                               L, R, defaultValue, lzNone);
        }
        return int(nodes.size()) - 1;
    }

    //---

    int add(T base, T value, int L, int R, T lazy, LazyT lazyType) {
        nodes.emplace_back(base, value, L, R, lazy, lazyType);
        return int(nodes.size()) - 1;
    }

    int pushDown(int node, int nodeLeft, int nodeRight, T val, LazyT lazyType) {
        if (val == defaultValue)
            return node;

        if (nodeLeft == nodeRight) {
            T newVal;
            if (lazyType == lzSet)
                newVal = nodes[node].base + val;
            else
                newVal = mergeOp(nodes[node].value, nodes[node].base + val);
            return add(nodes[node].base, newVal, -1, -1, defaultValue, lzNone);
        }

        if (lazyType == lzSet) {
            return add(nodes[node].base,
                       nodes[node].base + val,
                       nodes[node].L, nodes[node].R,
                       val,
                       lzSet);
        } else /*if (lazyType == lzUpdate)*/ {
            return add(nodes[node].base,
                       mergeOp(nodes[node].value, nodes[node].base + val),
                       nodes[node].L, nodes[node].R,
                       mergeOp(nodes[node].lazy, val),
                       (nodes[node].lazyType == lzNone) ? lzUpdate : nodes[node].lazyType);
        }
    }

    int recSetBase(int node, int nodeLeft, int nodeRight, int index, T x) {
        if (index < nodeLeft || nodeRight < index)
            return node;

        if (nodeLeft == nodeRight)
            return add(x, defaultValue, -1, -1, defaultValue, lzNone);

        int L = nodes[node].L;
        int R = nodes[node].R;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        if (nodes[node].lazyType != lzNone) {
            L = pushDown(L, nodeLeft, mid, nodes[node].lazy, nodes[node].lazyType);
            R = pushDown(R, mid + 1, nodeRight, nodes[node].lazy, nodes[node].lazyType);
        }

        L = recSetBase(L, nodeLeft, mid, index, x);
        R = recSetBase(R, mid + 1, nodeRight, index, x);
        return add(mergeOp(nodes[L].base, nodes[R].base),
                   mergeOp(nodes[L].value, nodes[R].value),
                   L, R, defaultValue, lzNone);
    }

    int recSet(int node, int nodeLeft, int nodeRight, int indexL, int indexR, T val) {
        if (indexR < nodeLeft || nodeRight < indexL)
            return node;

        if (nodeLeft == nodeRight)
            return add(nodes[node].base, nodes[node].base + val, -1, -1, defaultValue, lzNone);

        int L = nodes[node].L;
        int R = nodes[node].R;

        if (indexL <= nodeLeft && nodeRight <= indexR)
            return add(nodes[node].base, nodes[node].base + val, L, R, val, lzSet);

        int mid = (nodeLeft + nodeRight) >> 1;
        if (nodes[node].lazyType != lzNone) {
            L = pushDown(L, nodeLeft, mid, nodes[node].lazy, nodes[node].lazyType);
            R = pushDown(R, mid + 1, nodeRight, nodes[node].lazy, nodes[node].lazyType);
        }

        L = recSet(L, nodeLeft, mid, indexL, indexR, val);
        R = recSet(R, mid + 1, nodeRight, indexL, indexR, val);
        return add(nodes[node].base, nodes[node].base + val, L, R, val, lzSet);
    }

    int recUpdate(int node, int nodeLeft, int nodeRight, int indexL, int indexR, T val) {
        if (indexR < nodeLeft || nodeRight < indexL)
            return node;

        if (nodeLeft == nodeRight)
            return add(nodes[node].base,
                       mergeOp(nodes[node].value, nodes[node].base + val),
                       -1, -1, defaultValue, lzNone);

        int L = nodes[node].L;
        int R = nodes[node].R;

        if (indexL <= nodeLeft && nodeRight <= indexR)
            return add(nodes[node].base,
                       mergeOp(nodes[node].value, nodes[node].base + val),
                       L, R, mergeOp(nodes[node].lazy, val), lzUpdate);

        int mid = (nodeLeft + nodeRight) >> 1;
        if (nodes[node].lazyType != lzNone) {
            L = pushDown(L, nodeLeft, mid, nodes[node].lazy, nodes[node].lazyType);
            R = pushDown(R, mid + 1, nodeRight, nodes[node].lazy, nodes[node].lazyType);
        }

        L = recUpdate(L, nodeLeft, mid, indexL, indexR, val);
        R = recUpdate(R, mid + 1, nodeRight, indexL, indexR, val);
        return add(nodes[node].base,
                   mergeOp(nodes[L].value, nodes[R].value),
                   L, R, defaultValue, lzNone);
    }

    pair<T,int> recQuery(int node, int nodeLeft, int nodeRight, int indexL, int indexR) {
        if (indexR < nodeLeft || nodeRight < indexL)
            return make_pair(defaultValue, node);

        if (indexL <= nodeLeft && nodeRight <= indexR)
            return make_pair(nodes[node].value, node);

        int L = nodes[node].L;
        int R = nodes[node].R;

        int mid = (nodeLeft + nodeRight) >> 1;
        if (nodes[node].lazyType != lzNone) {
            L = pushDown(L, nodeLeft, mid, nodes[node].lazy, nodes[node].lazyType);
            R = pushDown(R, mid + 1, nodeRight, nodes[node].lazy, nodes[node].lazyType);
        }

        auto resL = recQuery(L, nodeLeft, mid, indexL, indexR);
        auto resR = recQuery(R, mid + 1, nodeRight, indexL, indexR);
        if (resL.second != nodes[node].L || resR.second != nodes[node].R) {
            node = add(nodes[node].base,
                       nodes[node].value,
                       resL.second, resR.second, defaultValue, lzNone);
        }
        return make_pair(mergeOp(resL.first, resR.first), node);
    }
};

template <typename T, typename MergeOp>
inline RollbackablePersistentSegmentTreeLazyWithBase<T, MergeOp>
makeRollbackablePersistentSegmentTreeLazyWithBase(MergeOp mop, T dfltValue = T()) {
    return RollbackablePersistentSegmentTreeLazyWithBase<T, MergeOp, BlockOp>(mop, dfltValue);
}

template <typename T, typename MergeOp>
inline RollbackablePersistentSegmentTreeLazyWithBase<T, MergeOp>
makeRollbackablePersistentSegmentTreeLazyWithBase(int n, MergeOp mop, T dfltValue = T()) {
    return RollbackablePersistentSegmentTreeLazyWithBase<T, MergeOp>(n, mop, dfltValue);
}

template <typename T, typename MergeOp>
inline RollbackablePersistentSegmentTreeLazyWithBase<T, MergeOp>
makeRollbackablePersistentSegmentTreeLazyWithBase(T value, int n, MergeOp mop, T dfltValue = T()) {
    return RollbackablePersistentSegmentTreeLazyWithBase<T, MergeOp>(value, n, mop, dfltValue);
}

template <typename T, typename MergeOp>
inline RollbackablePersistentSegmentTreeLazyWithBase<T, MergeOp>
makeRollbackablePersistentSegmentTreeLazyWithBase(const T A[], int n, MergeOp mop, T dfltValue = T()) {
    return RollbackablePersistentSegmentTreeLazyWithBase<T, MergeOp>(A, n, mop, dfltValue);
}

template <typename T, typename MergeOp>
inline RollbackablePersistentSegmentTreeLazyWithBase<T, MergeOp>
makeRollbackablePersistentSegmentTreeLazyWithBase(const vector<T>& A, MergeOp mop, T dfltValue = T()) {
    return RollbackablePersistentSegmentTreeLazyWithBase<T, MergeOp>(A, mop, dfltValue);
}
