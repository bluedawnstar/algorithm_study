#pragma once

struct SegmentTreeBitFlip {
    int         N;
    vector<int> one;
    vector<bool> inv;

    explicit SegmentTreeBitFlip(int n) {
        init(n);
    }

    template <typename T>
    SegmentTreeBitFlip(const vector<T>& v, T mask) {
        init((int)v.size());
        build(v, mask);
    }

    void init(int n) {
        N = n;
        one.assign(N * 4, 0);
        inv.assign(N * 4, false);
    }

    template <typename T>
    void build(const vector<T>& v, T mask) {
        vector<int> x(N);
        for (int i = 0; i < N; i++)
            x[i] = (v[i] & mask) != 0;

        buildSub(x, 1, 0, N - 1);
    }


    // inclusive
    int countOne(int left, int right) {
        return countOneSub(left, right, 1, 0, N - 1);
    }

    // inclusive
    void flip(int left, int right) {
        flipSub(left, right, 1, 0, N - 1);
    }

private:
    int buildSub(const vector<int>& x, int node, int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight)
            return one[node] = x[nodeLeft];

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return one[node] = buildSub(x, node * 2, nodeLeft, mid) +
                           buildSub(x, node * 2 + 1, mid + 1, nodeRight);
    }

    int countOneSub(int left, int right, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return 0;

        if (left <= nodeLeft && nodeRight <= right)
            return inv[node] ? (nodeRight - nodeLeft + 1) - one[node] : one[node];

        pushDown(node, nodeLeft, nodeRight);

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return countOneSub(left, right, node * 2, nodeLeft, mid) +
               countOneSub(left, right, node * 2 + 1, mid + 1, nodeRight);
    }

    int flipSub(int left, int right, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return inv[node] ? (nodeRight - nodeLeft + 1) - one[node] : one[node];

        if (left <= nodeLeft && nodeRight <= right) {
            inv[node] = !inv[node];
            return inv[node] ? (nodeRight - nodeLeft + 1) - one[node] : one[node];
        }

        pushDown(node, nodeLeft, nodeRight);

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return one[node] = flipSub(left, right, node * 2, nodeLeft, mid) +
                           flipSub(left, right, node * 2 + 1, mid + 1, nodeRight);
    }

    void pushDown(int node, int nodeLeft, int nodeRight) {
        if (!inv[node])
            return;

        one[node] = (nodeRight - nodeLeft + 1) - one[node];
        inv[node] = false;

        if (nodeLeft < nodeRight) {
            inv[node * 2] = !inv[node * 2];
            inv[node * 2 + 1] = !inv[node * 2 + 1];
        }
    }
};
