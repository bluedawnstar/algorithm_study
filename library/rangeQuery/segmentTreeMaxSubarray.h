#pragma once

#include <vector>
#include <functional>

//--------- Segment Tree for Maximum Subarray ---------------------------------

template <typename T, const T INF = 0x3f3f3f3f>
struct SegmentTreeMaxSubarray {
    struct Node {
        T   prefixSum;
        T   suffixSum;
        T   totalSum;
        T   bestSum;

        void init(T x) {
            prefixSum = x;
            suffixSum = x;
            totalSum = x;
            bestSum = x;
        }
    };

    static Node mergeOp(const Node& l, const Node& r) {
        return Node{
            max(l.prefixSum, max(l.totalSum, l.totalSum + r.prefixSum)),
            max(r.suffixSum, max(r.totalSum, r.totalSum + l.suffixSum)),
            l.totalSum + r.totalSum,
            max(l.bestSum, max(r.bestSum, l.suffixSum + r.prefixSum))
        };
    }

    static void mergeOpR(const Node& l, Node& r) {
        r.bestSum = max(l.bestSum, max(r.bestSum, l.suffixSum + r.prefixSum));
        r.prefixSum = max(l.prefixSum, max(l.totalSum, l.totalSum + r.prefixSum));
        r.suffixSum = max(r.suffixSum, max(r.totalSum, r.totalSum + l.suffixSum));
        r.totalSum = l.totalSum + r.totalSum;
    }

    static void mergeOpL(Node& l, const Node& r) {
        l.bestSum = max(l.bestSum, max(r.bestSum, l.suffixSum + r.prefixSum));
        l.prefixSum = max(l.prefixSum, max(l.totalSum, l.totalSum + r.prefixSum));
        l.suffixSum = max(r.suffixSum, max(r.totalSum, r.totalSum + l.suffixSum));
        l.totalSum = l.totalSum + r.totalSum;
    }

    static void mergeOp(Node& out, const Node& l, const Node& r) {
        out.prefixSum = max(l.prefixSum, max(l.totalSum, l.totalSum + r.prefixSum));
        out.suffixSum = max(r.suffixSum, max(r.totalSum, r.totalSum + l.suffixSum));
        out.totalSum = l.totalSum + r.totalSum;
        out.bestSum = max(l.bestSum, max(r.bestSum, l.suffixSum + r.prefixSum));
    }

    int          N;         // the size of array
    vector<Node> tree;      //

    SegmentTreeMaxSubarray() : N(0) {
    }

    explicit SegmentTreeMaxSubarray(int size) {
        init(size);
    }

    SegmentTreeMaxSubarray(T value, int size) {
        build(value, size);
    }

    SegmentTreeMaxSubarray(const T arr[], int size) {
        build(arr, size);
    }

    SegmentTreeMaxSubarray(const vector<T>& v) {
        build(v);
    }


    void init(int size) {
        N = size + (size & 1);
        tree = vector<Node>(N * 2);
    }

    void build(T value, int size) {
        init(size);

        for (int i = 0; i < size; i++)
            tree[N + i].init(value);

        for (int i = N - 1; i > 0; i--)
            mergeOp(tree[i], tree[i << 1], tree[(i << 1) | 1]);
    }

    void build(const T arr[], int size) {
        init(size);

        for (int i = 0; i < size; i++)
            tree[N + i].init(arr[i]);

        for (int i = N - 1; i > 0; i--)
            mergeOp(tree[i], tree[i << 1], tree[(i << 1) | 1]);
    }

    void build(const vector<T>& v) {
        build(&v[0], int(v.size()));
    }


    T query(int index) const {
        return tree[index + N].bestSum;
    }

    // inclusive
    T query(int left, int right) const {
        Node resL = { -INF, -INF, -INF, -INF };
        Node resR = { -INF, -INF, -INF, -INF };

        for (int L = left + N, R = right + N + 1; L < R; L >>= 1, R >>= 1) {
            if (L & 1)
                mergeOpL(resL, tree[L++]);
            if (R & 1)
                mergeOpR(tree[--R], resR);
        }

        return mergeOp(resL, resR).bestSum;
    }

    void update(int index, T newValue) {
        tree[index + N].init(newValue);

        for (int i = (index + N) >> 1; i > 0; i >>= 1)
            mergeOp(tree[i], tree[i << 1], tree[(i << 1) | 1]);
    }

    // inclusive
    void updateRange(int left, int right, T newValue) {
        for (int L = left + N, R = right + N + 1; L < R; L++)
            tree[L].init(newValue);

        for (int L = (left + N) >> 1, R = (right + N) >> 1; L > 0; L >>= 1, R >>= 1) {
            for (int i = L; i <= R; i++)
                mergeOp(tree[i], tree[i << 1], tree[(i << 1) | 1]);
        }
    }
};
