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

#define MAXN    100000

int gN;
vector<int> gE[MAXN + 1];
int gP[MAXN + 1];
ll gV[MAXN + 1];
ll gM[MAXN + 1];

int gLeafN;
int gLeafS[MAXN + 1];

ll gG[MAXN + 1];

// iterative
template <typename T>
T gcd(T p, T q) {
    if (p < q)
        swap(p, q);

    T t;
    while (q != 0) {
        t = q;
        q = p % q;
        p = t;
    }

    return p;
}

inline unsigned long long gcdFast(unsigned long long p, unsigned long long q) {
    if (p == 0)
        return q;
    if (q == 0)
        return p;

#ifdef __GNUC__
    int shift = int(__builtin_ctzll(p | q));

    p >>= __builtin_ctzll(p);
    do {
        q >>= __builtin_ctzll(q);
        if (p > q) {
            unsigned long long t = q;
            q = p;
            p = t;
        }
        q -= p;
    } while (q != 0);

    return p << shift;
#elif defined(_M_X64) 
    int shift = int(_tzcnt_u64(p | q));

    p >>= _tzcnt_u64(p);
    do {
        q >>= _tzcnt_u64(q);
        if (p > q) {
            unsigned long long t = q;
            q = p;
            p = t;
        }
        q -= p;
    } while (q != 0);

    return p << shift;
#else
    return gcd(p, q);
#endif
}


void dfs(int u, int p, ll g) {
    gP[u] = p;

    if (g > 1)
        g = (ll)gcdFast(g, gV[u]);

    int childN = 0;
    for (int v : gE[u]) {
        if (v == p)
            continue;
        dfs(v, u, g);
        childN++;
    }

    if (childN == 0) {
        gLeafS[gLeafN++] = u;
        if (g == 1 || gM[u] == 1)
            gG[u] = 1;
        else if (gG[u] > 1)
            gG[u] = (ll)gcdFast(g, gM[u]);
    }
}

void solve() {
    for (int i = 0; i < gN; i++)
        gG[i] = gM[i];

    gLeafN = 0;
    dfs(0, -1, gV[0]);

    sort(gLeafS, gLeafS + gLeafN);

    for (int i = 0; i < gLeafN; i++) {
        if (i > 0)
            printf(" ");

        int u = gLeafS[i];
        ll m = gM[u];
        printf("%lld", ((m - 1) / gG[u]) * gG[u]);
    }
    printf("\n");
}

#if 1
int main(void) {
    int T;
    scanf("%d", &T);

    while (T-- > 0) {
        cin >> gN;
        for (int i = 0; i < gN; i++)
            gE[i].clear();
        for (int i = 1; i < gN; i++) {
            int u, v;
            scanf("%d %d", &u, &v);
            u--;
            v--;
            gE[u].push_back(v);
            gE[v].push_back(u);
        }
        for (int i = 0; i < gN; i++)
            scanf("%lld", &gV[i]);
        for (int i = 0; i < gN; i++)
            scanf("%lld", &gM[i]);

        solve();
    }

    return 0;
}
#else
int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T-- > 0) {
        cin >> gN;
        for (int i = 0; i < gN; i++)
            gE[i].clear();
        for (int i = 1; i < gN; i++) {
            int u, v;
            cin >> u >> v;
            u--;
            v--;
            gE[u].push_back(v);
            gE[v].push_back(u);
        }
        for (int i = 0; i < gN; i++)
            cin >> gV[i];
        for (int i = 0; i < gN; i++)
            cin >> gM[i];

        solve();
    }

    return 0;
}
#endif
