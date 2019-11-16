#pragma once

// It's not a DP solution.

// https://www.codechef.com/problems/PBOXES
// the fast solver of max profit problem with multiple transactions at the same time
struct MaxProfitSegmentTreeWithMultipleTransactions2 {
    static const int INF = 0x3f3f3f3f;

    int maxX;               // 1 <= x <= maxX
    vector<deque<int>> Y;   // [x] = { y0, y1, ... },  1 <= x <= maxX,  1 <= y_i

    //-- segment trees

    vector<int> forwardPairCount;

    vector<pair<int, int>> forwardMaxPair;
    vector<pair<int, int>> backwardMaxPair;
    vector<pair<int, int>> backwardMaxPair2;

    vector<int> xOfMinY;
    vector<int> xOfMaxY;

    vector<int> leftX;      // left node of X
    vector<int> rightX;     // right node of X

    //--- initialize & build

    void init(int maxX) {
        if (maxX < 2)
            maxX = 2;

        int N = maxX + 1;

        this->maxX = maxX;
        Y.assign(N, deque<int>());

        //---

        forwardPairCount.assign(N * 4, 0);

        forwardMaxPair.assign(N * 4, pair<int, int>());
        backwardMaxPair.assign(N * 4, pair<int, int>());
        backwardMaxPair2.assign(N * 4, pair<int, int>());

        xOfMinY.assign(N * 4, 0);
        xOfMaxY.assign(N * 4, 0);
        leftX.assign(N * 4, 0);
        rightX.assign(N * 4, 0);
    }

    void add(int x, int y) {
        Y[x].push_back(y);
    }

    void build() {
        for (int i = 0; i <= maxX; i++)
            sort(Y[i].begin(), Y[i].end());
        build(1, 1, maxX);
    }

    //--- getter

    int getMinY(int x) const {
        return Y[x].empty() ? INF : Y[x].front();
    }

    int getMaxY(int x) const {
        return Y[x].empty() ? -INF : Y[x].back();
    }

    int getDeltaY(int x) const {
        return (Y[x].empty() ? -INF : Y[x].back()) - (Y[x].empty() ? INF : Y[x].front());
    }

    int getDeltaY(int xL, int xR) const {
        return (Y[xR].empty() ? -INF : Y[xR].back()) - (Y[xL].empty() ? INF : Y[xL].front());
    }

    //--- update

    void updateForwardPairCount(int left, int right, int delta) {
        updateForwardPairCount(1, 1, maxX, left, right, delta);
    }

    void updateValue(int x) {
        updateValue(1, 1, maxX, x);
    }

    //--- query

    // return a pair to have maximum Y delta and the value (left X, right X, Y delta)
    tuple<int,int,int> queryMaxPair() const {
        pair<int, int> p = forwardMaxPair[1];
        if (forwardPairCount[1] > 0)
            p = maxPair(forwardMaxPair[1], backwardMaxPair[1]);
        else
            p = maxPair(forwardMaxPair[1], backwardMaxPair2[1]);
        return make_tuple(p.first, p.second, getDeltaY(p.first, p.second));
    }

    tuple<int,int,int> popMaxPair() {
        auto res = queryMaxPair();
        if (get<2>(res) > 0) {
            int left = get<0>(res);
            int right = get<1>(res);

            Y[right].pop_back();
            updateValue(right);

            Y[left].pop_front();
            updateValue(left);

            if (left < right)
                updateForwardPairCount(left, right, 1);
            else if (left > right)
                updateForwardPairCount(right, left, -1);
        }
        return res;
    }

    // values must be sorted, O(N*logN)
    static vector<long long> solve(const pair<int,int> values[], int n, int maxK) {
        int maxX = max_element(values, values + n)->first;

        MaxProfitSegmentTreeWithMultipleTransactions2 tree;
        tree.init(maxX);

        for (int i = 0; i < n; i++)
            tree.add(values[i].first, values[i].second);
        tree.build();

        vector<long long> ans(maxK);
        for(int i = 0; i < maxK; i++) {
            auto maxValue = tree.popMaxPair();
            if (get<2>(maxValue) <= 0)
                break;
            ans[i] = (i > 0 ? ans[i - 1] : 0) + get<2>(maxValue);
        }

        for(int i = 1; i < maxK; i++)
            ans[i] = max(ans[i], ans[i - 1]);

        return ans;
    }

    static vector<long long> solve(const vector<pair<int, int>>& values, int maxK) {
        return solve(values.data(), int(values.size()), maxK);
    }

private:
    pair<int, int> maxPair(const pair<int, int>& a, const pair<int, int>& b) const {
        return (getDeltaY(a.first, a.second) > getDeltaY(b.first, b.second)) ? a : b;
    }

    void merge(int node) {
        int L = node * 2;
        int R = node * 2 + 1;

        forwardMaxPair[node]  = maxPair(maxPair(forwardMaxPair[L] , forwardMaxPair[R] ), { xOfMinY[L], xOfMaxY[R] });
        backwardMaxPair[node] = maxPair(maxPair(backwardMaxPair[L], backwardMaxPair[R]), { xOfMinY[R], xOfMaxY[L] });
        backwardMaxPair2[node] = maxPair({ (forwardPairCount[R] > 0) ? xOfMinY[R] : leftX[R], (forwardPairCount[L] > 0) ? xOfMaxY[L] : rightX[L] },
            maxPair((forwardPairCount[R] > 0) ? backwardMaxPair[R] : backwardMaxPair2[R],
            (forwardPairCount[L] > 0) ? backwardMaxPair[L] : backwardMaxPair2[L]));

        xOfMinY[node] = (getMinY(xOfMinY[L]) < getMinY(xOfMinY[R])) ? xOfMinY[L] : xOfMinY[R];
        xOfMaxY[node] = (getMaxY(xOfMaxY[L]) > getMaxY(xOfMaxY[R])) ? xOfMaxY[L] : xOfMaxY[R];

        if (forwardPairCount[L] > 0)
            leftX[node] = (getMinY(xOfMinY[L]) < getMinY(leftX[R])) ? xOfMinY[L] : leftX[R];
        else
            leftX[node] = leftX[L];

        if (forwardPairCount[R] > 0)
            rightX[node] = (getMaxY(xOfMaxY[R]) > getMaxY(rightX[L])) ? xOfMaxY[R] : rightX[L];
        else
            rightX[node] = rightX[R];
    }

    void build(int node, int nodeLeft, int nodeRight) {
        forwardPairCount[node] = 0;

        if (nodeLeft + 1 == nodeRight) { // nodeLeft and nodeRight are leaves
            forwardMaxPair[node] = maxPair(maxPair({ nodeLeft, nodeLeft }, { nodeRight, nodeRight }), { nodeLeft, nodeRight });
            backwardMaxPair[node] = { nodeRight, nodeLeft };
            backwardMaxPair2[node] = { 0, 0 };

            xOfMinY[node] = (getMinY(nodeLeft) < getMinY(nodeRight)) ? nodeLeft : nodeRight;
            xOfMaxY[node] = (getMaxY(nodeLeft) > getMaxY(nodeRight)) ? nodeLeft : nodeRight;
            leftX[node] = nodeLeft;
            rightX[node] = nodeRight;
        } else {
            int mid = nodeLeft + ((nodeRight - nodeLeft) >> 1);
            build(node * 2, nodeLeft, mid);
            build(node * 2 + 1, mid, nodeRight);
            merge(node);
        }
    }

    void updateForwardPairCount(int node, int nodeLeft, int nodeRight, int left, int right, int delta) {
        if (right <= nodeLeft || nodeRight <= left)
            return;

        if (left <= nodeLeft && nodeRight <= right)
            forwardPairCount[node] += delta;
        else {
            int mid = nodeLeft + ((nodeRight - nodeLeft) >> 1);

            forwardPairCount[node * 2    ] += forwardPairCount[node];
            forwardPairCount[node * 2 + 1] += forwardPairCount[node];

            updateForwardPairCount(node * 2, nodeLeft, mid, left, right, delta);
            updateForwardPairCount(node * 2 + 1, mid, nodeRight, left, right, delta);

            int d = max(0, min(forwardPairCount[node * 2], forwardPairCount[node * 2 + 1]));
            forwardPairCount[node * 2    ] -= d;
            forwardPairCount[node * 2 + 1] -= d;
            forwardPairCount[node] = d;
        }

        if (nodeLeft < left || right < nodeRight)
            merge(node);
    }

    void updateValue(int node, int nodeLeft, int nodeRight, int x) {
        if (x < nodeLeft || nodeRight < x)
            return;

        if (nodeLeft + 1 == nodeRight) { // nodeLeft and nodeRight are leaves
            forwardMaxPair[node] = maxPair(maxPair({ nodeLeft, nodeLeft }, { nodeRight, nodeRight }), { nodeLeft, nodeRight });
            backwardMaxPair[node] = { nodeRight, nodeLeft };
            backwardMaxPair2[node] = { 0, 0 };

            xOfMinY[node] = (getMinY(nodeLeft) < getMinY(nodeRight)) ? nodeLeft : nodeRight;
            xOfMaxY[node] = (getMaxY(nodeLeft) > getMaxY(nodeRight)) ? nodeLeft : nodeRight;
            leftX[node] = nodeLeft;
            rightX[node] = nodeRight;
        } else {
            int mid = nodeLeft + ((nodeRight - nodeLeft) >> 1);
            updateValue(node * 2, nodeLeft, mid, x);
            updateValue(node * 2 + 1, mid, nodeRight, x);
            merge(node);
        }
    }
};
