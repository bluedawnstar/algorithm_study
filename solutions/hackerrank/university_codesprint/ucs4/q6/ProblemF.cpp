#include <memory.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cctype>
#include <cstring>
#include <climits>
#include <cmath>
#include <vector>
#include <string>
#include <memory>
#include <numeric>
#include <limits>
#include <functional>
#include <tuple>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iostream>

using namespace std;

#define PROFILE_START(i)    clock_t start##i = clock()
#define PROFILE_STOP(i)     fprintf(stderr, "elapsed time (" #i ") = %f\n", double(clock() - start##i) / CLOCKS_PER_SEC)

typedef long long           ll;
typedef unsigned long long  ull;

#define MOD 1000000007
#define MAXN 200000

ll SumX[MAXN + 3];
ll SumXX[MAXN + 3];

// The first 'node' number is 1, not 0
// Others('left', 'right', 'nodeLeft', 'nodeRight', 'index') are started from 0
// Upper envelope with line segment
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
        sumXTbl.resize(N + 2);
        sumXXTbl.resize(N + 2);
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
        return query(x, 1, 0, N - 1);
    }

    // query sum in [left, right]
    //   - first : SUM{ max{ b[k] + m[k] * x }     }, i <= k <= j, i <= x <= j
    //   - second: SUM{ max{ b[k] + m[k] * x } * x }, i <= k <= j, i <= x <= j
    // inclusive, O(NlogN)
    pair<long long, long long> querySum(int left, int right) {
        return querySum(left, right, 1, 0, N - 1);
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
        addSub(l, left, right, node << 1, nodeLeft, mid);
        addSub(l, left, right, (node << 1) | 1, mid + 1, nodeRight);

        if (nodeLeft < nodeRight) {
            treeL[node] = treeL[node << 1];
            treeR[node] = treeR[(node << 1) | 1];

            onMerge(node);
        }
    }

    long long query(int x, int node, int nodeLeft, int nodeRight) {
        if (nodeLeft > nodeRight || x < nodeLeft || x > nodeRight)
            return -INF;

        if (nodeLeft == nodeRight)
            return treeLazy[node].get(x);

        pushDown(node, nodeLeft, nodeRight);
        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        if (x <= mid)
            return query(x, node << 1, nodeLeft, mid);
        else
            return query(x, (node << 1) | 1, mid + 1, nodeRight);
    }

    pair<long long, long long> querySum(int left, int right, int node, int nodeLeft, int nodeRight) {
        if (nodeLeft > nodeRight || right < nodeLeft || left > nodeRight)
            return make_pair(0ll, 0ll);

        auto res = make_pair(0ll, 0ll);

        pushDown(node, nodeLeft, nodeRight);
        if (left <= nodeLeft && nodeRight <= right) {
            res.first = (res.first + sum[node]);
            res.second = (res.second + sumX[node]);
            return res;
        }

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        auto l = querySum(left, right, node << 1, nodeLeft, mid);
        auto r = querySum(left, right, (node << 1) | 1, mid + 1, nodeRight);

        res.first = (l.first + r.first);
        res.second = (l.second + r.second);

        return res;
    }

private:
    //TODO: customize these functions
/*
    void onInit() {
        for (int i = 1; i <= N; i++) {
            sumXTbl[i] = sumXTbl[i - 1] + i;
            sumXXTbl[i] = sumXXTbl[i - 1] + 1ll * i * i;
        }
    }

    void onUpdate(const Line& l, int left, int right, int node) {
        long long b = l.b;
        long long sX = (SumX[right] - SumX[left - 1]);
        long long sXX = (SumXX[right] - SumXX[left - 1]);
        sum[node] = (b * (right - left + 1) + l.m * sX);
        sumX[node] = (b * sX + l.m * sXX);
    }

    void onMerge(int node) {
        sum[node] = (sum[node << 1] + sum[(node << 1) | 1]);
        sumX[node] = (sumX[node << 1] + sumX[(node << 1) | 1]);
    }
*/

    void onInit() {
        for (int i = 1; i <= N; i++) {
            sumXTbl[i] = (sumXTbl[i - 1] + i) % MOD;
            sumXXTbl[i] = (sumXXTbl[i - 1] + 1ll * i * i % MOD) % MOD;
        }
    }

    void onUpdate(const Line& l, int left, int right, int node) {
        long long b = l.b % MOD;
        long long sX = (sumXTbl[right] + MOD - sumXTbl[left - 1]) % MOD;
        long long sXX = (sumXXTbl[right] + MOD - sumXXTbl[left - 1]) % MOD;
        sum[node] = (b * (right - left + 1) % MOD + MOD + l.m * sX % MOD) % MOD;
        sumX[node] = (b * sX % MOD + MOD + l.m * sXX % MOD) % MOD;
    }

    void onMerge(int node) {
        sum[node] = (sum[node << 1] + sum[(node << 1) | 1]) % MOD;
        sumX[node] = (sumX[node << 1] + sumX[(node << 1) | 1]) % MOD;
    }
};

// The first 'node' number is 1, not 0
// Others('left', 'right', 'nodeLeft', 'nodeRight', 'index') are started from 0
// Lower envelope with line segment
struct SegmentTreeLineSegment2DMinSum {
    static const long long INF = 0x3f3f3f3f3f3f3f3fll;

    struct Line {
        long long m, b;     // f(x) = m * x + b

        Line() : m(0), b(INF) {
        }

        Line(long long m, long long b) : m(m), b(b) {
        }

        long long get(int x) const {
            return m * x + b;
        }

        int compare(const Line& rhs, int x) const {
            if (get(x) <= rhs.get(x))
                return 1;
            else if (m >= rhs.m)
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
    vector<long long> sum;      // SUM{ min{ b[k] + m[k] * x }     }, i <= k <= j, i <= x <= j
    vector<long long> sumX;     // SUM{ min{ b[k] + m[k] * x } * x }, i <= k <= j, i <= x <= j

    vector<long long> sumXTbl;  // SUM[0<=x<=n]{ x }
    vector<long long> sumXXTbl; // SUM[0<=x<=n]{ x * x }

    SegmentTreeLineSegment2DMinSum(int size)
        : N(size), treeL(size * 4), treeR(size * 4), treeLazy(size * 4), lazyExist(size * 4) {

        sum.resize(N * 4);
        sumX.resize(N * 4);
        sumXTbl.resize(N + 2);
        sumXXTbl.resize(N + 2);
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
        return query(x, 1, 0, N - 1);
    }

    // query sum in [left, right]
    //   - first : SUM{ min{ b[k] + m[k] * x }     }, i <= k <= j, i <= x <= j
    //   - second: SUM{ min{ b[k] + m[k] * x } * x }, i <= k <= j, i <= x <= j
    // inclusive, O(NlogN)
    pair<long long, long long> querySum(int left, int right) {
        return querySum(left, right, 1, 0, N - 1);
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
        addSub(l, left, right, node << 1, nodeLeft, mid);
        addSub(l, left, right, (node << 1) | 1, mid + 1, nodeRight);

        if (nodeLeft < nodeRight) {
            treeL[node] = treeL[node << 1];
            treeR[node] = treeR[(node << 1) | 1];

            onMerge(node);
        }
    }

    long long query(int x, int node, int nodeLeft, int nodeRight) {
        if (nodeLeft > nodeRight || x < nodeLeft || x > nodeRight)
            return INF;

        if (nodeLeft == nodeRight)
            return treeLazy[node].get(x);

        pushDown(node, nodeLeft, nodeRight);
        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        if (x <= mid)
            return query(x, node << 1, nodeLeft, mid);
        else
            return query(x, (node << 1) | 1, mid + 1, nodeRight);
    }

    pair<long long, long long> querySum(int left, int right, int node, int nodeLeft, int nodeRight) {
        if (nodeLeft > nodeRight || right < nodeLeft || left > nodeRight)
            return make_pair(0ll, 0ll);

        auto res = make_pair(0ll, 0ll);

        pushDown(node, nodeLeft, nodeRight);
        if (left <= nodeLeft && nodeRight <= right) {
            res.first = (res.first + sum[node]);
            res.second = (res.second + sumX[node]);
            return res;
        }

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        auto l = querySum(left, right, node << 1, nodeLeft, mid);
        auto r = querySum(left, right, (node << 1) | 1, mid + 1, nodeRight);

        res.first = (l.first + r.first);
        res.second = (l.second + r.second);

        return res;
    }

private:
    //TODO: customize these functions
    /*
    void onInit() {
    for (int i = 1; i <= N; i++) {
    sumXTbl[i] = sumXTbl[i - 1] + i;
    sumXXTbl[i] = sumXXTbl[i - 1] + 1ll * i * i;
    }
    }

    void onUpdate(const Line& l, int left, int right, int node) {
    long long b = l.b;
    long long sX = (SumX[right] - SumX[left - 1]);
    long long sXX = (SumXX[right] - SumXX[left - 1]);
    sum[node] = (b * (right - left + 1) + l.m * sX);
    sumX[node] = (b * sX + l.m * sXX);
    }

    void onMerge(int node) {
    sum[node] = (sum[node << 1] + sum[(node << 1) | 1]);
    sumX[node] = (sumX[node << 1] + sumX[(node << 1) | 1]);
    }
    */

    void onInit() {
        for (int i = 1; i <= N; i++) {
            sumXTbl[i] = (sumXTbl[i - 1] + i) % MOD;
            sumXXTbl[i] = (sumXXTbl[i - 1] + 1ll * i * i % MOD) % MOD;
        }
    }

    void onUpdate(const Line& l, int left, int right, int node) {
        long long b = l.b % MOD;
        long long sX = (sumXTbl[right] + MOD - sumXTbl[left - 1]) % MOD;
        long long sXX = (sumXXTbl[right] + MOD - sumXXTbl[left - 1]) % MOD;
        sum[node] = (b * (right - left + 1) % MOD + MOD + l.m * sX % MOD) % MOD;
        sumX[node] = (b * sX % MOD + MOD + l.m * sXX % MOD) % MOD;
    }

    void onMerge(int node) {
        sum[node] = (sum[node << 1] + sum[(node << 1) | 1]) % MOD;
        sumX[node] = (sumX[node << 1] + sumX[(node << 1) | 1]) % MOD;
    }
};


int N;
int A[MAXN + 1];

template <typename T>
inline T gcd(T p, T q) {
    if (p < q)
        swap(p, q);

    if (q == 0)
        return 0;
    else if (q == 1)
        return 1;

    T t;
    while (q != 0) {
        t = q;
        q = p % q;
        p = t;
    }

    return p;
}

ll solveMin(int L, int R) {
    if (L > R)
        return 0ll;
    else if (L == R)
        return A[L];

    int mid = L + (R - L) / 2;

    vector<tuple<int, int, int>> gcdR;  // (gcd, start, count)
    vector<tuple<int, int, int>> gcdL;  // (gcd, start, count)

                                        // x[mid + 1..R]
    {
        int g = A[mid + 1];
        gcdR.emplace_back(g, 0, 1);
        for (int i = mid + 2, j = 1; i <= R; i++, j++) {
            g = gcd(g, A[i]);
            if (get<0>(gcdR.back()) == g)
                ++get<2>(gcdR.back());
            else
                gcdR.emplace_back(g, j, 1);
        }
    }

    // m[L..mid]
    {
        int g = A[mid];
        gcdL.emplace_back(g, 0, 1);
        for (int i = mid - 1, j = 1; i >= L; i--, j++) {
            g = gcd(g, A[i]);
            if (get<0>(gcdL.back()) == g)
                ++get<2>(gcdL.back());
            else
                gcdL.emplace_back(g, j, 1);
        }
    }

    ll ans = 0;
    for (int i = 0; i < (int)gcdL.size(); i++) {
        int x = get<0>(gcdL[i]);
        int y = get<1>(gcdL[i]);
        int n = get<2>(gcdL[i]);
        int nn = (int)SumX[n];
        for (int j = 0; j < (int)gcdR.size(); j++) {
            int g = gcd(x, get<0>(gcdR[j]));
            int m = get<2>(gcdR[j]);
            int D = y + get<1>(gcdR[j]);
            int mm = (int)SumX[m];
            ans = (ans + ((mm + 1ll * m * D) * n + 1ll * nn * m) % MOD * g % MOD) % MOD;
        }
    }

    ans = (ans + solveMin(L, mid) + solveMin(mid + 1, R)) % MOD;

    return ans;
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);


    cin >> N;
    for (int i = 0; i < N; i++)
        cin >> A[i];

    for (int i = 1; i <= N + 1; i++) {
        SumX[i] = (SumX[i - 1] + i) % MOD;
        SumXX[i] = (SumXX[i - 1] + 1ll * i * i % MOD) % MOD;
    }

#if 0
    for (int i = 0; i < N; i++) {
        ll maxV = 0;
        for (int j = i; j < N; j++) {
            maxV = max(maxV, 1ll * A[j] * (j - i + 1));             // max{ A(k) * (k - i + 1) }, i <= k <= j
            ans = (ans + maxV % MOD * (j - i + 1) % MOD) % MOD;     // max{ A(k) * (k - i + 1) } * (j - i + 1), i <= k <= j
        }
    }
#else
    SegmentTreeLineSegment2DMaxSum tree(N + 1);

    ll ans = 0;
    for (int i = 1; i <= N; i++) {
        tree.add(1, i, (ll)-A[i - 1], 1ll * A[i - 1] * (i + 1));

        auto r = tree.querySum(1, i);
        r.first %= MOD;
        r.second %= MOD;
        ans = (ans + r.first * (i + 1) % MOD + MOD - r.second) % MOD;
    }
#endif

    cout << (ans + MOD - solveMin(0, N - 1)) % MOD << endl;

    return 0;
}
