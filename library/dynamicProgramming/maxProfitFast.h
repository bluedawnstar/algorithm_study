#pragma once

// It's not a DP solution.

// https://www.codechef.com/problems/PBOXES
// the fast solver of max profit problem with multiple transactions at the same time
struct MaxProfitSegmentTreeWithMultipleTransactions {
    static const int INF = 0x3f3f3f3f;

    int N;
    vector<pair<int,int>> values;               // values[i] = (Xi, Yi)

    //---

    vector<int> lazy;
    vector<int> forwardCount;                   // the number of forward pairs

    vector<pair<int, int>> minValue, maxValue;  // (value, index)
    vector<pair<int,pair<int,int>>> ans;        // (value, (index1, index2))

    vector<pair<int, int>> minL, minR;          // (value, index)
    vector<pair<int, int>> maxL, maxR;          // (value, index)

    MaxProfitSegmentTreeWithMultipleTransactions() : N(0) {
    }

    MaxProfitSegmentTreeWithMultipleTransactions(const pair<int, int> values[], int n) {
        build(values, n);
    }

    MaxProfitSegmentTreeWithMultipleTransactions(const vector<pair<int, int>>& values) {
        build(values);
    }

    //---

    // values[i] = (Xi, Yi)
    // values must be sorted, O(N*logN)
    void build(const pair<int,int> values[], int n) {
        N = n;
        this->values.assign(values, values + n);

        lazy = vector<int>(n * 4);
        forwardCount = vector<int>(n * 4);

        minValue = vector<pair<int, int>>(n * 4);
        maxValue = vector<pair<int, int>>(n * 4);
        ans = vector<pair<int,pair<int,int>>>(n * 4);

        minL = vector<pair<int, int>>(n * 4);
        minR = vector<pair<int, int>>(n * 4);
        maxL = vector<pair<int, int>>(n * 4);
        maxR = vector<pair<int, int>>(n * 4);

        build(values, 1, 0, n - 1);
    }

    void build(const vector<pair<int, int>>& values) {
        build(values.data(), int(values.size()));
    }

    //--- query

    // result = (profit, (left, right)), O(logN)
    pair<int, pair<int, int>> findNext() {
        pair<int, pair<int, int>> res(0, make_pair(-1, -1));

        auto t = ans[1];
        if (t.first > 0)
            res.first = t.first;

        if (t.first > 0) {
            change(1, 0, N - 1, t.second.first);
            change(1, 0, N - 1, t.second.second);
            if (values[t.second.first].second < values[t.second.second].second)
                add(1, 0, N - 1, t.second.first + 1, t.second.second, 1);
            else
                add(1, 0, N - 1, t.second.first + 1, t.second.second, -1);
        }
        return res;
    }

    // result[i] = max profit with at most (i + 1) transactions, O(K*logN)
    vector<long long> solve(int maxK) {
        vector<long long> res;

        long long ans = 0;
        for (int i = 0; i < maxK; i++) {
            ans += findNext().first;
            res.push_back(ans);
        }

        return res;
    }

private:
    void merge(int node) {
        int L = node * 2;
        int R = node * 2 + 1;

        minValue[node] = min(minValue[L], minValue[R]);
        maxValue[node] = max(maxValue[L], maxValue[R]);

        ans[node] = max(ans[L], ans[R]);
        if (maxValue[R].first - minValue[L].first > ans[node].first)
            ans[node] = make_pair(maxValue[R].first - minValue[L].first, make_pair(minValue[L].second, maxValue[R].second));
        if (maxR[L].first - minL[R].first > ans[node].first)
            ans[node] = make_pair(maxR[L].first - minL[R].first, make_pair(maxR[L].second, minL[R].second));

        if (forwardCount[L] > 0) {
            minL[node] = min(minValue[L], minL[R]);
            maxL[node] = max(maxValue[L], maxL[R]);
        } else {
            minL[node] = minL[L];
            maxL[node] = maxL[L];
        }

        if (forwardCount[R] > 0) {
            minR[node] = min(minValue[R], minR[L]);
            maxR[node] = max(maxValue[R], maxR[L]);
        } else {
            minR[node] = minR[R];
            maxR[node] = maxR[R];
        }
        forwardCount[node] = min(forwardCount[L], forwardCount[R]);
    }

    void build(const pair<int,int> values[], int node, int nodeLeft,int nodeRight) {
        if (nodeLeft == nodeRight) {
            minValue[node] = maxValue[node] = make_pair(values[nodeLeft].second, nodeLeft);
            minL[node] = minR[node] = make_pair(INF, nodeLeft);
            maxL[node] = maxR[node] = make_pair(-INF, nodeLeft);
            return;
        }

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        build(values, node * 2, nodeLeft, mid);
        build(values, node * 2 + 1, mid + 1, nodeRight);
        merge(node);
    }

    //--- 

    void getAns(int node, int nodeLeft, int nodeRight, int c) {
        lazy[node] += c;
        forwardCount[node] += c;

        if (nodeLeft == nodeRight || forwardCount[node] > 0) {
            if (forwardCount[node] > 0) {
                minL[node] = minR[node] = minValue[node];
                maxL[node] = maxR[node] = maxValue[node];
            } else {
                minL[node] = minR[node] = make_pair(INF, nodeLeft);
                maxL[node] = maxR[node] = make_pair(-INF, nodeLeft);
            }
            if (nodeLeft != nodeRight && maxValue[node].first - minValue[node].first > ans[node].first) {
                ans[node] = make_pair(maxValue[node].first - minValue[node].first,
                    make_pair(min(maxValue[node].second, minValue[node].second), max(maxValue[node].second, minValue[node].second)));
            }
            return;
        }
        if (lazy[node]) {
            int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
            getAns(node * 2, nodeLeft, mid, lazy[node]);
            getAns(node * 2 + 1, mid + 1, nodeRight, lazy[node]);
            lazy[node] = 0;
        }
        merge(node);
    }

    void pushDown(int node, int nodeLeft, int nodeRight) {
        if (lazy[node]) {
            int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
            getAns(node * 2, nodeLeft, mid, lazy[node]);
            getAns(node * 2 + 1, mid + 1, nodeRight, lazy[node]);
            lazy[node] = 0;
        }
    }

    void change(int node, int nodeLeft, int nodeRight, int x) {
        if (nodeLeft == nodeRight) {
            minValue[node] = minL[node] = minR[node] = make_pair(INF, x);
            maxValue[node] = maxL[node] = maxR[node] = make_pair(-INF, x);
            return;
        }

        pushDown(node, nodeLeft, nodeRight);
        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        if (x <= mid)
            change(node * 2, nodeLeft, mid, x);
        else
            change(node * 2 + 1, mid + 1, nodeRight, x);
        merge(node);
    }

    void add(int node, int nodeLeft, int nodeRight, int left, int right, int c) {
        if (nodeLeft == left && nodeRight == right) {
            getAns(node, nodeLeft, nodeRight, c);
            return;
        }

        pushDown(node, nodeLeft, nodeRight);

        int mid = nodeLeft + (nodeRight - nodeLeft)/2;
        if (right <= mid)
            add(node * 2, nodeLeft, mid, left, right, c);
        else if (left > mid)
            add(node * 2 + 1, mid + 1, nodeRight, left, right, c);
        else {
            add(node * 2, nodeLeft, mid, left, mid, c);
            add(node * 2 + 1, mid + 1, nodeRight, mid + 1, right, c);
        }
        merge(node);
    }
};
