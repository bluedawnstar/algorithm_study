#pragma once

// The first 'node' number is 1, not 0
// Others('left', 'right', 'nodeLeft', 'nodeRight', 'index') are started from 0
// Upper envelope with line segment
// 
// https://www.hackerrank.com/contests/university-codesprint-4/challenges/magic-value
struct SegmentTreeLineSegment2DMaxSum {
    static const long long INF = 0x3f3f3f3f3f3f3f3fll;

    struct Line {
        long long m, b;     // f(x) = m * x + b

        Line() : m(0), b(-INF) {
        }

        Line(long long m, long long b) : m(m), b(b) {
        }

        long long get(int x) const {
            return m * x + b;
        }

        int compare(const Line& rhs, int x) const {
            if (get(x) >= rhs.get(x))
                return 1;
            else if (m <= rhs.m)
                return 2;
            else
                return 0;
        }
    };

    int N;
    vector<Line> treeL;
    vector<Line> treeR;
    vector<Line> treeLazy;
    vector<bool> lazyExist;

    // sum
    vector<long long> sum;      // SUM{ max{ b[k] + m[k] * x }     }, i <= k <= j, i <= x <= j
    vector<long long> sumX;     // SUM{ max{ b[k] + m[k] * x } * x }, i <= k <= j, i <= x <= j

    vector<long long> sumXTbl;  // SUM[0<=x<=n]{ x }
    vector<long long> sumXXTbl; // SUM[0<=x<=n]{ x * x }

    SegmentTreeLineSegment2DMaxSum(int size)
        : N(size), treeL(size * 4), treeR(size * 4), treeLazy(size * 4), lazyExist(size * 4) {

        sum.resize(N * 4);
        sumX.resize(N * 4);
        sumXTbl.resize(N + 1);
        sumXXTbl.resize(N + 1);
        onInit();
    }

    // add a line segment
    // inclusive, O(NlogN)
    void add(int left, int right, long long m, long long b) {
        Line l(m, b);
        addSub(l, left, right, 1, 0, N - 1);
    }

    // query max f(x)
    // O(NlogN)
    long long query(int x) {
        return querySub(x, 1, 0, N - 1);
    }

    // query sum in [left, right]
    //   - first : SUM{ max{ b[k] + m[k] * x }     }, i <= k <= j, i <= x <= j
    //   - second: SUM{ max{ b[k] + m[k] * x } * x }, i <= k <= j, i <= x <= j
    // inclusive, O(NlogN)
    pair<long long, long long> querySum(int left, int right) {
        return querySumSub(left, right, 1, 0, N - 1);
    }

private:
    void pushDown(int node, int nodeLeft, int nodeRight) {
        if (lazyExist[node]) {
            if (nodeLeft < nodeRight) {
                treeLazy[node << 1] = treeLazy[(node << 1) | 1] = treeLazy[node];
                lazyExist[node << 1] = lazyExist[(node << 1) | 1] = true;
            }

            treeL[node] = treeLazy[node];
            treeR[node] = treeLazy[node];

            onUpdate(treeLazy[node], nodeLeft, nodeRight, node);

            lazyExist[node] = false;
        }
    }

    void addSub(Line& l, int left, int right, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return;

        pushDown(node, nodeLeft, nodeRight);
        if (left <= nodeLeft && nodeRight <= right) {
            int cl = l.compare(treeL[node], nodeLeft);
            int cr = l.compare(treeR[node], nodeRight);
            if (cl == cr) {
                if (cl == 1) {
                    treeLazy[node] = l;
                    lazyExist[node] = true;
                    pushDown(node, nodeLeft, nodeRight);
                }
                return;
            }
        }

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        addSub(l, left, right, (node << 1), nodeLeft, mid);
        addSub(l, left, right, (node << 1) | 1, mid + 1, nodeRight);

        if (nodeLeft < nodeRight) {
            treeL[node] = treeL[node << 1];
            treeR[node] = treeR[(node << 1) | 1];

            onMerge(node);
        }
    }

    long long querySub(int x, int node, int nodeLeft, int nodeRight) {
        if (nodeLeft > nodeRight || x < nodeLeft || x > nodeRight)
            return -INF;

        pushDown(node, nodeLeft, nodeRight);
        if (nodeLeft == nodeRight)
            return treeLazy[node].get(x);

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        if (x <= mid)
            return querySub(x, (node << 1), nodeLeft, mid);
        else
            return querySub(x, (node << 1) | 1, mid + 1, nodeRight);
    }

    pair<long long, long long> querySumSub(int left, int right, int node, int nodeLeft, int nodeRight) {
        if (nodeLeft > nodeRight || right < nodeLeft || left > nodeRight)
            return make_pair(0ll, 0ll);

        auto res = make_pair(0ll, 0ll);

        pushDown(node, nodeLeft, nodeRight);
        if (left <= nodeLeft && nodeRight <= right) {
            res.first = sum[node];
            res.second = sumX[node];
            return res;
        }

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        auto l = querySumSub(left, right, (node << 1), nodeLeft, mid);
        auto r = querySumSub(left, right, (node << 1) | 1, mid + 1, nodeRight);

        res.first = (l.first + r.first);
        res.second = (l.second + r.second);

        return res;
    }

private:
    //TODO: customize these functions
#ifndef MOD
    void onInit() {
        for (int i = 1; i <= N; i++) {
            sumXTbl[i] = sumXTbl[i - 1] + i;
            sumXXTbl[i] = sumXXTbl[i - 1] + 1ll * i * i;
        }
    }

    void onUpdate(const Line& l, int left, int right, int node) {
        long long b = l.b;
        long long sX = sumXTbl[right] - (left > 0 ? sumXTbl[left - 1] : 0ll);
        long long sXX = sumXXTbl[right] - (left > 0 ? sumXXTbl[left - 1] : 0ll);
        sum[node] = (b * (right - left + 1) + l.m * sX);
        sumX[node] = (b * sX + l.m * sXX);
    }

    void onMerge(int node) {
        sum[node] = (sum[node << 1] + sum[(node << 1) | 1]);
        sumX[node] = (sumX[node << 1] + sumX[(node << 1) | 1]);
    }
#else
    void onInit() {
        for (int i = 1; i <= N; i++) {
            sumXTbl[i] = (sumXTbl[i - 1] + i) % MOD;
            sumXXTbl[i] = (sumXXTbl[i - 1] + 1ll * i * i % MOD) % MOD;
        }
    }

    void onUpdate(const Line& l, int left, int right, int node) {
        long long b = l.b % MOD;
        long long sX = (sumXTbl[right] + MOD - (left > 0 ? sumXTbl[left - 1] : 0)) % MOD;
        long long sXX = (sumXXTbl[right] + MOD - (left > 0 ? sumXXTbl[left - 1] : 0)) % MOD;
        sum[node] = (b * (right - left + 1) % MOD + MOD + l.m * sX % MOD) % MOD;
        sumX[node] = (b * sX % MOD + MOD + l.m * sXX % MOD) % MOD;
    }

    void onMerge(int node) {
        sum[node] = (sum[node << 1] + sum[(node << 1) | 1]) % MOD;
        sumX[node] = (sumX[node << 1] + sumX[(node << 1) | 1]) % MOD;
    }
#endif
};
