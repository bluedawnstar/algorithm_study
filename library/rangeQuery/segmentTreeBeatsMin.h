#pragma once

/*
https://codeforces.com/blog/entry/58564
https://codeforces.com/blog/entry/57319

    1. update
        A[i] = min(A[i], x)   ,  L <= i <= R

    2. query #1
        max(A[L], A[L + 1], ..., A[R])

    3. query #2
        sum(A[L], A[L + 1], ..., A[R])
*/
template <typename T>
struct SegmentTreeBeatsMin {
    static const T INF = numeric_limits<T>::max();

    struct Node {
        T   maxValue;
        T   maxValue2;

        T   cntMax;
        T   sumValue;

        T   minValue;

        Node() {
            init();
        }

        void init() {
            maxValue = -INF;
            maxValue2 = -INF;

            cntMax = 0;
            sumValue = 0;

            minValue = INF;
        }

        void init(T x) {
            maxValue = x;
            maxValue2 = -INF;

            cntMax = 1;
            sumValue = x;

            minValue = x;
        }


        void mergeOp(const Node& L, const Node& R) {
            if (L.maxValue == R.maxValue) {
                maxValue = L.maxValue;
                maxValue2 = max(L.maxValue2, R.maxValue2);
                cntMax = L.cntMax + R.cntMax;
            } else {
                maxValue = max(L.maxValue, R.maxValue);
                maxValue2 = max(min(L.maxValue, R.maxValue), max(L.maxValue2, R.maxValue2));
                cntMax = (maxValue == L.maxValue) ? L.cntMax : R.cntMax;
            }
            sumValue = L.sumValue + R.sumValue;

            minValue = min(L.minValue, R.minValue);
        }

        bool isBreakCondition(T value) const {
            return value >= maxValue;
        }

        bool isTagCondition(T value) const {
            return value > maxValue2;
        }
    };

    int       N;                // the size of array
    vector<Node> tree;          // 

    SegmentTreeBeatsMin() {
    }

    void init(int size) {
        N = size;
        tree = vector<Node>(size * 4);
    }

    void build(T value, int n) {
        init(n);
        buildSub(value, 0, n - 1, 1);
    }

    void build(const T arr[], int n) {
        init(n);
        buildSub(arr, 0, n - 1, 1);
    }

    void build(const vector<T>& v) {
        build(&v[0], int(v.size()));
    }


    // A[i] = min(A[i], X), inclusive, O(logN)
    void updateMin(int left, int right, T newValue) {
        updateSub(left, right, newValue, 1, 0, N - 1);
    }


    // inclusive, O(logN)
    T querySum(int left, int right) {
        return querySumSub(left, right, 1, 0, N - 1);
    }

    // inclusive, O(logN)
    pair<T,T> queryMinMax(int left, int right) {
        return queryMinMaxSub(left, right, 1, 0, N - 1);
    }

private:
    void applyMin(int node, T value) {
        tree[node].minValue = min(tree[node].minValue, value);

        tree[node].sumValue -= tree[node].cntMax * (tree[node].maxValue - value);
        tree[node].maxValue = value;
    }

    void pushDown(int node, int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight)
            return;

        // left node
        if (tree[node].maxValue < tree[node * 2].maxValue)
            applyMin(node * 2, tree[node].maxValue);

        // right node
        if (tree[node].maxValue < tree[node * 2 + 1].maxValue)
            applyMin(node * 2 + 1, tree[node].maxValue);
    }

    //---

    // inclusive
    int buildSub(const T arr[], int left, int right, int node) {
        if (left > right)
            return 0;

        if (left == right) {
            tree[node].init(arr[left]);
            return node;
        }

        int mid = left + (right - left) / 2;
        int nodeL = buildSub(arr, left, mid, node * 2);
        int nodeR = buildSub(arr, mid + 1, right, node * 2 + 1);

        tree[node].mergeOp(tree[nodeL], tree[nodeR]);

        return node;
    }

    // inclusive
    int buildSub(T value, int left, int right, int node) {
        if (left > right)
            return 0;

        if (left == right) {
            tree[node].init(value);
            return node;
        }

        int mid = left + (right - left) / 2;
        int nodeL = buildSub(value, left, mid, node * 2);
        int nodeR = buildSub(value, mid + 1, right, node * 2 + 1);

        tree[node].mergeOp(tree[nodeL], tree[nodeR]);

        return node;
    }

    int updateSub(int left, int right, T newValue, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left || tree[node].isBreakCondition(newValue))
            return node;

        pushDown(node, nodeLeft, nodeRight);
        if (left <= nodeLeft && nodeRight <= right && tree[node].isTagCondition(newValue)) {
            applyMin(node, newValue);
            return node;
        }

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        int nodeL = updateSub(left, right, newValue, node * 2, nodeLeft, mid);
        int nodeR = updateSub(left, right, newValue, node * 2 + 1, mid + 1, nodeRight);

        tree[node].mergeOp(tree[nodeL], tree[nodeR]);

        return node;
    }

    //---

    T querySumSub(int left, int right, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return 0;

        if (left <= nodeLeft && nodeRight <= right)
            return tree[node].sumValue;

        pushDown(node, nodeLeft, nodeRight);

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        auto resL = querySumSub(left, right, node * 2, nodeLeft, mid);
        auto resR = querySumSub(left, right, node * 2 + 1, mid + 1, nodeRight);

        return resL + resR;
    }

    pair<T,T> queryMinMaxSub(int left, int right, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return make_pair(INF, -INF);

        if (left <= nodeLeft && nodeRight <= right)
            return make_pair(tree[node].minValue, tree[node].maxValue);

        pushDown(node, nodeLeft, nodeRight);

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        auto resL = queryMinMaxSub(left, right, node * 2, nodeLeft, mid);
        auto resR = queryMinMaxSub(left, right, node * 2 + 1, mid + 1, nodeRight);

        return make_pair(min(resL.first, resR.first), max(resL.second, resR.second));
    }
};
