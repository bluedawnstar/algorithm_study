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
template <typename T,
          typename MergeOp = function<T(T, T)>>
struct SegmentTreeLazyWithBase {
    enum LazyT {
        lzNone,
        lzSet,
        lzUpdate
    };

    struct Node {
        T       base;           // fixed base value
        T       value;          // value = base + delta
        T       lazy;
        LazyT   lazyType;

        Node() {
        }

        Node(T base, T value, T lazy, LazyT lazyType)
            : base(base), value(value), lazy(lazy), lazyType(lazyType) {
        }
    };

    int          N;             // the size of array
    vector<Node> tree;

    T       defaultValue;
    MergeOp mergeOp;

    explicit SegmentTreeLazyWithBase(MergeOp mop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop) {
    }

    SegmentTreeLazyWithBase(int size, MergeOp mop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop) {
        init(size);
    }

    SegmentTreeLazyWithBase(T x, int n, MergeOp mop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop) {
        build(x, n);
    }

    SegmentTreeLazyWithBase(const T A[], int n, MergeOp mop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop) {
        build(A, n);
    }

    SegmentTreeLazyWithBase(const vector<T>& A, MergeOp mop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop) {
        build(A);
    }


    void init(int size) {
        N = size;
        tree.assign(size * 4, Node{ defaultValue, defaultValue, defaultValue, lzNone });
    }

    void build(T x, int n) {
        init(n);
        buildSub(x, 0, n - 1, 1);
    }

    void build(const T A[], int n) {
        init(n);
        buildSub(A, 0, n - 1, 1);
    }

    void build(const vector<T>& A) {
        build(&A[0], int(A.size()));
    }

    //--- update

    // inclusive
    void setBase(int index, T value) {
        setBaseSub(index, value, 1, 0, N - 1);
    }

    // inclusive
    void set(int left, int right, T value) {
        setSub(left, right, value, 1, 0, N - 1);
    }

    // inclusive
    void update(int left, int right, T value) {
        updateSub(left, right, value, 1, 0, N - 1);
    }

    //--- query

    // inclusive
    T query(int left, int right) {
        return querySub(left, right, 1, 0, N - 1);
    }

private:
    void mergeNode(int node, int nodeL, int nodeR) {
        tree[node].base = mergeOp(tree[nodeL].base, tree[nodeR].base);
        tree[node].value = mergeOp(tree[nodeL].value, tree[nodeR].value);
    }

    // inclusive
    void buildSub(const T A[], int left, int right, int node) {
        if (left == right) {
            tree[node].base = A[left];
            tree[node].value = A[left];
            return;
        }

        int L = node * 2;
        int R = L + 1;
        int mid = left + (right - left) / 2;
        buildSub(A, left, mid, L);
        buildSub(A, mid + 1, right, R);
        mergeNode(node, L, R);
    }

    // inclusive
    void buildSub(T x, int left, int right, int node) {
        if (left == right) {
            tree[node].base = x;
            tree[node].value = x;
            return;
        }

        int L = node * 2;
        int R = L + 1;
        int mid = left + (right - left) / 2;
        buildSub(x, left, mid, L);
        buildSub(x, mid + 1, right, R);
        mergeNode(node, L, R);
    }

    void pushDown(T x, LazyT lazyType, int node, int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight) {
            if (lazyType == lzSet)
                tree[node].value = tree[node].base + x;
            else
                tree[node].value = mergeOp(tree[node].value, tree[node].base + x);
            return;
        }

        if (lazyType == lzSet) {
            tree[node].value = tree[node].base + x;
            tree[node].lazy = x;
            tree[node].lazyType = lzSet;
        } else /*if (lazyType == lzUpdate)*/ {
            tree[node].value = mergeOp(tree[node].value, tree[node].base + x);
            tree[node].lazy = mergeOp(tree[node].lazy, x);
            if (tree[node].lazyType == lzNone)
                tree[node].lazyType = lzUpdate;
        }
    }

    void pushDownFrom(int node, int nodeLeft, int nodeRight) {
        if (tree[node].lazyType == lzNone)
            return;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        pushDown(tree[node].lazy, tree[node].lazyType, node * 2, nodeLeft, mid);
        pushDown(tree[node].lazy, tree[node].lazyType, node * 2 + 1, mid + 1, nodeRight);
        tree[node].lazy = defaultValue;
        tree[node].lazyType = lzNone;
    }

    void setBaseSub(int index, T x, int node, int nodeLeft, int nodeRight) {
        if (index < nodeLeft || nodeRight < index)
            return;

        if (nodeLeft == nodeRight) {
            tree[node].base = x;
            tree[node].value = defaultValue;
            return;
        }

        pushDownFrom(node, nodeLeft, nodeRight);

        int L = node * 2;
        int R = L + 1;
        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        setBaseSub(index, x, L, nodeLeft, mid);
        setBaseSub(index, x, R, mid + 1, nodeRight);
        mergeNode(node, L, R);
    }

    void setSub(int left, int right, T x, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return;

        if (nodeLeft == nodeRight) {
            tree[node].value = tree[node].base + x;
            return;
        }

        if (left <= nodeLeft && nodeRight <= right) {
            tree[node].value = tree[node].base + x;
            tree[node].lazy = x;
            tree[node].lazyType = lzSet;
            return;
        }

        pushDownFrom(node, nodeLeft, nodeRight);

        int L = node * 2;
        int R = L + 1;
        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        setSub(left, right, x, L, nodeLeft, mid);
        setSub(left, right, x, R, mid + 1, nodeRight);
        mergeNode(node, L, R);
    }

    void updateSub(int left, int right, T x, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return;

        if (nodeLeft == nodeRight) {
            tree[node].value = mergeOp(tree[node].value, tree[node].base + x);
            return;
        }

        if (left <= nodeLeft && nodeRight <= right) {
            tree[node].value = mergeOp(tree[node].value, tree[node].base + x);
            tree[node].lazy = mergeOp(tree[node].lazy, x);
            tree[node].lazyType = lzUpdate;
            return;
        }

        pushDownFrom(node, nodeLeft, nodeRight);

        int L = node * 2;
        int R = L + 1;
        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        updateSub(left, right, x, L, nodeLeft, mid);
        updateSub(left, right, x, R, mid + 1, nodeRight);
        mergeNode(node, L, R);
    }

    T querySub(int left, int right, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return defaultValue;

        if (left <= nodeLeft && nodeRight <= right)
            return tree[node].value;

        pushDownFrom(node, nodeLeft, nodeRight);

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return mergeOp(querySub(left, right, node * 2, nodeLeft, mid),
                       querySub(left, right, node * 2 + 1, mid + 1, nodeRight));
    }
};
