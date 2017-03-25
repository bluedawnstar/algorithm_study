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
#include <functional>
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

typedef long long ll;
typedef unsigned long ull;

// - O(kn^2) => O(kn)
// - dp[i][j] = min { dp[i - 1][k] + b[k] * a[j] }
//              k<j
//   => b[k] is slope, dp[i - 1][k] is xterm
template <typename T>
struct DPConvexHullTrick {
    struct Line {
        T slope, xterm;

        template <typename U>
        inline T get(U x) const {
            return slope * x + xterm;
        }
    };

    deque<Line> lines;

    // a[j] <= a[j + 1], b[k] >= b[k + 1]
    void insert(T slope, T xterm) {
        Line l{ slope, xterm };

        while (lines.size() > 1 && ccw(lines[lines.size() - 2], lines[lines.size() - 1], l))
            lines.pop_back();

        if (lines.size() == 1 && lines.front().xterm > l.xterm)
            lines.pop_back();
        lines.push_back(l);
    }

    // a[j] >= a[j + 1], b[k] <= b[k + 1]
    void insertReverse(T slope, T xterm) {
        Line l{ slope, xterm };

        while (lines.size() > 1 && cw(lines[lines.size() - 2], lines[lines.size() - 1], l))
            lines.pop_back();

        if (lines.size() != 1 || lines.front().xterm > l.xterm)
            lines.push_back(l);
    }

    template <typename U>
    T query(U x) {
        if (lines.empty())
            return 0;

        while (lines.size() > 1 && lines[0].get(x) > lines[1].get(x))
            lines.pop_front();

        return lines[0].get(x);
    }

private:
    static T area(const Line& a, const Line& b, const Line& c) {
        T ax = (b.slope - a.slope);
        T bx = (c.xterm - a.xterm);
        T ay = (c.slope - a.slope);
        T by = (b.xterm - a.xterm);
        return ax * bx - ay * by;
    }

    static bool ccw(const Line& a, const Line& b, const Line& c) {
        return area(a, b, c) > 0;
    }

    static bool cw(const Line& a, const Line& b, const Line& c) {
        return area(a, b, c) < 0;
    }
};


#define MAXK    5000
#define MAXN    5000

int gN, gK;
int gX[MAXN + 10];
int gW[MAXN + 10];

ll gSumW[MAXN + 2];
ll gSumXW[MAXN + 2];
ll gDP[MAXN + 2][MAXN + 2];

ll calcW(int s, int e) {
    return (s > e) ? 0 : gSumW[e + 1] - gSumW[s];
}

ll calcXW(int s, int e) {
    return (s > e) ? 0 : gSumXW[e + 1] - gSumXW[s];
}

int main(void) {
    //ios_base::sync_with_stdio(false);
    //cin.tie(nullptr);

    scanf("%d %d", &gN, &gK);
    for (int i = 0; i < gN; i++) {
        scanf("%d %d", &gX[gN - 1 - i], &gW[gN - 1 - i]);
    }

    gSumW[0] = 0;
    gSumXW[0] = 0;
    for (int i = 0; i < gN; i++) {
        gSumW[i + 1] = gSumW[i] + gW[i];
        gSumXW[i + 1] = gSumXW[i] + ll(gW[i]) * gX[i];
    }

    for (int n = 1; n < gN; n++)
        gDP[1][n] = calcXW(0, n) - calcW(0, n) * gX[n];

    for (int k = 2; k <= gK; k++) {
        DPConvexHullTrick<ll> cht;
        cht.insertReverse(calcW(0, k - 2), gDP[k - 1][k - 2] - calcXW(0, k - 2));
        for (int n = k - 1; n < gN; n++) {
            if (n < k)
                gDP[k][n] = 0;
            else
                gDP[k][n] = calcXW(0, n) - gX[n] * calcW(0, n) + cht.query(gX[n]);
            cht.insertReverse(calcW(0, n), gDP[k - 1][n] - calcXW(0, n));
        }
    }

    printf("%lld\n", gDP[gK][gN - 1]);

    return 0;
}
