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

#define MAXN    50000

int gN, gM;
vector<int> gE[MAXN];

bitset<MAXN> gL[MAXN];
int gP[MAXN];

bitset<MAXN> gVis;

// cycle parent
void dfs(int u, bitset<MAXN>& visStack, vector<int>& stack) {
    gL[u].set(u);

    gVis[u] = true;
    visStack[u] = true;
    stack.push_back(u);
    for (int i = 0; i < (int)gE[u].size(); i++) {
        int v = gE[u][i];
        if (!gVis[v]) {
            dfs(v, visStack, stack);
        } else if (visStack[v]) {
            // set cycle groups' parent
            for (int i = (int)stack.size() - 1; i >= 0; i--) {
                if (stack[i] == v)
                    break;
                gP[stack[i]] = v;
            }
        }
        gL[u] |= gL[v];
    }
    stack.pop_back();
    visStack[u] = false;
}

int main(void) {
    //ios_base::sync_with_stdio(false);
    //cin.tie(nullptr);

    memset(gP, -1, sizeof(gP));

    int u, v;

    scanf("%d %d", &gN, &gM);
    for (int i = 0; i < gM; i++) {
        scanf("%d %d", &u, &v);
        u--; v--;
        gE[u].push_back(v);
    }

    vector<pair<int, int>> query;

    int Q;
    scanf("%d", &Q);
    while (Q-- > 0) {
        int type, x, y;
        scanf("%d %d %d", &type, &x, &y);

        if (type == 1) {
            x--;
            if (y == 0)
                gE[x].push_back(gN++);
            else
                gE[gN++].push_back(x);
        } else {
            x--; y--;
            query.emplace_back(x, y);
        }
    }

    for (int i = 0; i < gN; i++) {
        if (!gVis[i]) {
            bitset<MAXN> s;
            vector<int> st;
            dfs(i, s, st);
        }
    }

    for (int i = 0; i < gN; i++) {
        if (gP[i] < 0)
            continue;

        // find cycle groups' root
        int t = gP[i];
        while (gP[t] >= 0)
            t = gP[t];

        gL[i] |= gL[t];
    }

    for (auto q : query) {
        printf("%s\n", gL[q.first].test(q.second) ? "Yes" : "No");
    }

    return 0;
}
