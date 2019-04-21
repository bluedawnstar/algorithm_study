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
#define INF     ((1ll << 62) - 1)

int gN, gK;
ll gA[MAXN + 1];
int gT[MAXN + 1];

int gXN;
ll gX[MAXN + 1];

ll gSX[MAXN + 1];

ll gAns;

unordered_set<ull> gDP;

void dfs(int depth, int pos, ll X) {
    if (X == 0)
        return;

    ull key = ((depth + 1) * (ull)1e18) + ((pos + 1) * (ull)1e16) + X;
    auto it = gDP.find(key);
    if (it != gDP.end())
        return;

    //if (depth >= gK) {
    //    gAns = max(gAns, X);
    //    return;
    //}

    if (depth == gK - 1) {
        X &= gSX[gXN] - gSX[pos];
        gAns = max(gAns, X);
        return;
    }
    
    int R = gXN - (gK - depth - 1);

    ll s = 0;
    for (int i = pos; i < R; i++) {
        s += gX[i];
        dfs(depth + 1, i + 1, X & s);
    }

    gDP.insert(key);
}

ll solve() {
    gDP.clear();

    gXN = 0;
    gSX[0] = 0;
    for (int i = 0; i < gN; i++) {
        if (gT[i]) {
            gX[gXN++] = gA[i] * gT[i];
            gSX[gXN] = gSX[gXN - 1] + gX[gXN - 1];
        }
    }

    if (gXN < gK)
        return 0ll;

    gAns = 0;
    dfs(0, 0, INF);

    return gAns;
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> gN;
    for (int i = 0; i < gN; i++)
        cin >> gA[i];

    int Q;
    cin >> Q;

    while (Q-- > 0) {
        cin >> gK;

        for (int i = 0; i < gN; i++)
            cin >> gT[i];

        cout << solve() << endl;
    }

    return 0;
}
