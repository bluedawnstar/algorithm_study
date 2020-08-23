#pragma once

/*
https://codeforces.com/blog/entry/58564
https://codeforces.com/blog/entry/57319

    1. update
        A[i] = max(A[i], x)   ,  L <= i <= R

    2. query #1
        min(A[L], A[L + 1], ..., A[R])

    3. query #2
        sum(A[L], A[L + 1], ..., A[R])
*/
template <typename T>
struct SegmentTreeBeatsMax {
    static const T INF = numeric_limits<T>::max();

    struct Node {
        T   minValue;
        T   minValue2;

        T   cntMin;
        T   sumValue;

        T   maxValue;

        Node() {
            init();
        }

        void init() {
            minValue = INF;
            minValue2 = INF;

            cntMin = 0;
            sumValue = 0;

            maxValue = -INF;
        }

        void init(T x) {
            minValue = x;
            minValue2 = INF;

            cntMin = 1;
            sumValue = x;

            maxValue = x;
        }


        void mergeOp(const Node& L, const Node& R) {
            if (L.minValue == R.minValue) {
                minValue = L.minValue;
                minValue2 = min(L.minValue2, R.minValue2);
                cntMin = L.cntMin + R.cntMin;
            } else {
                minValue = min(L.minValue, R.minValue);
                minValue2 = min(max(L.minValue, R.minValue), min(L.minValue2, R.minValue2));
                cntMin = (minValue == L.minValue) ? L.cntMin : R.cntMin;
            }
            sumValue = L.sumValue + R.sumValue;

            maxValue = max(L.maxValue, R.maxValue);
        }

        bool isBreakCondition(T value) const {
            return value <= minValue;
        }

        bool isTagCondition(T value) const {
            return value < minValue2;
        }
    };

    int       N;                // the size of array
    vector<Node> tree;          // 

    SegmentTreeBeatsMax() {
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


    // A[i] = max(A[i], X), inclusive, O(logN)
    void updateMax(int left, int right, T newValue) {
        updateSub(left, right, newValue, 1, 0, N - 1);
    }


    // inclusive, O(logN)
    T querySum(int left, int right) {
        return querySumSub(left, right, 1, 0, N - 1);
    }

    // inclusive, O(logN)
    pair<T, T> queryMinMax(int left, int right) {
        return queryMinMaxSub(left, right, 1, 0, N - 1);
    }

private:
    void pushDown(int node, int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight)
            return;

        // left node
        if (tree[node].minValue > tree[node * 2].minValue) {
            tree[node * 2].maxValue = max(tree[node * 2].maxValue, tree[node].minValue);

            tree[node * 2].sumValue += tree[node * 2].cntMin * (tree[node].minValue - tree[node * 2].minValue);
            tree[node * 2].minValue = tree[node].minValue;
        }

        // right node
        if (tree[node].minValue > tree[node * 2 + 1].minValue) {
            tree[node * 2 + 1].maxValue = max(tree[node * 2 + 1].maxValue, tree[node].minValue);

            tree[node * 2 + 1].sumValue += tree[node * 2 + 1].cntMin * (tree[node].minValue - tree[node * 2 + 1].minValue);
            tree[node * 2 + 1].minValue = tree[node].minValue;
        }
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
            tree[node].maxValue = max(tree[node].maxValue, newValue);

            tree[node].sumValue += tree[node].cntMin * (newValue - tree[node].minValue);
            tree[node].minValue = newValue;
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

    pair<T, T> queryMinMaxSub(int left, int right, int node, int nodeLeft, int nodeRight) {
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
