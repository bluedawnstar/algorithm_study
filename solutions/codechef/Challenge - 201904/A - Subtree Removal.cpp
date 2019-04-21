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
int gX;
int gA[MAXN + 1];
vector<int> gE[MAXN + 1];

ll gS[MAXN + 1];

void dfs(int u, int p) {
    gS[u] = gA[u];
    for (auto v : gE[u]) {
        if (v == p)
            continue;
        dfs(v, u);
        gS[u] += gS[v];
    }
}

ll dfsSolve(int u, int p) {
    ll res = 0;
    for (int v : gE[u]) {
        if (v == p)
            continue;
        res += dfsSolve(v, u);
    }

    res = min(res, gS[u] + gX);
    if (res > 0)
        res = 0;
    return res;
}

ll solve() {
    dfs(0, -1);
    return gS[0] - dfsSolve(0, -1);
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T-- > 0) {
        cin >> gN >> gX;

        for (int i = 0; i < gN; i++) {
            cin >> gA[i];
            gE[i].clear();
        }

        for (int i = 1; i < gN; i++) {
            int u, v;
            cin >> u >> v;
            u--, v--;

            gE[u].push_back(v);
            gE[v].push_back(u);
        }

        cout << solve() << endl;
    }

    return 0;
}
