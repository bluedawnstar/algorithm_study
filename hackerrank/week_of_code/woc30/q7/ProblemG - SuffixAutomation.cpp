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

typedef long long           ll;
typedef unsigned long long  ull;

typedef vector<int>     vi;
typedef pair<int, int>  pii;
typedef pair<ll, ll>    pll;
#define fi              first
#define se              second
#define pb              push_back
#define eb              emplace_back
#define em              emplace
#define mp              make_pair


#define MAXN    50000
#define MAXS    100000

int gN, gQ;
vector<string> gS;
vector<int> gSuffixToStrID;

char gT[MAXN + 10];

vector<int> gAns;
vector<pii> gQIn;
unordered_map<int, unordered_map<int, int>> gQIndex; // (L, R) -> Q index
vector<unordered_map<int, int>> gQuery;     // R --> (L, Q index)

struct SuffixAutomation {
    static const size_t MaxCharN = 26;
    static int ch2i(int c) { return c - 'a'; }

    struct StateT {
        int len;                    // the length of the longest string in a class
        int suffixLink;             // 
        int edges[MaxCharN];        // labeled edges
    };
    int N;
    vector<StateT> state;           // t0 is zeroth state, other states get numbers 1,2,3,...
    int last;

    SuffixAutomation(int maxN) : state(maxN * 2, StateT{ 0, 0,{ 0, } }) {
        init();
    }

    void init() {
        last = 0;
        state[0].len = 0;
        state[0].suffixLink = -1;
        N = 1;
    }

    void extend(char c) {
        int i = ch2i(c);

        int cur = N++;
        state[cur].len = state[last].len + 1;

        // add edge from last nodes into new node
        int p;
        for (p = last; p >= 0 && !state[p].edges[i]; p = state[p].suffixLink) {
            state[p].edges[i] = cur;
        }

        if (p == -1)
            state[cur].suffixLink = 0;                            // set new node's suffix link to t0
        else {
            int q = state[p].edges[i];
            if (state[p].len + 1 == state[q].len)
                state[cur].suffixLink = q;                        // we do not have to split q, just set the correct suffix link
            else {
                // we have to split, add q'
                int clone = N++;
                state[clone].len = state[p].len + 1;
                memcpy(state[clone].edges, state[q].edges, sizeof(state[q].edges)); // copy edges of q
                state[clone].suffixLink = state[q].suffixLink;          // copy suffix link of q

                for (; p >= 0 && state[p].edges[i] == q; p = state[p].suffixLink)
                    state[p].edges[i] = clone;                          // fix last edges of all nodes to have the same class to p

                state[q].suffixLink = state[cur].suffixLink = clone;    // set q and cur's suffix link to clone
            }
        }
        last = cur;
    }

    template <typename T>
    void extend(T s, int n) {
        for (int i = 0; i < n; i++)
            extend(s[i]);
    }
};


int lcs(SuffixAutomation& sa, string& t) {
    int v = 0, l = 0, best = 0, bestpos = 0;
    for (int i = 0; i < (int)t.length(); ++i) {
        while (v && !sa.state[v].edges[t[i] - 'a']) {
            v = sa.state[v].suffixLink;
            l = sa.state[v].len;
        }
        if (sa.state[v].edges[t[i] - 'a']) {
            v = sa.state[v].edges[t[i] - 'a'];
            ++l;
        }
        if (l > best)
            best = l, bestpos = i;
    }
    return best;
}

int main(void) {
    //ios_base::sync_with_stdio(false);
    //cin.tie(nullptr);

    scanf("%d %d", &gN, &gQ);

    gS.reserve(gN);
    for (int i = 0; i < gN; i++) {
        scanf("%s", gT);
        gS.emplace_back(gT);
    }

    vector<SuffixAutomation> SA;
    for (int i = 0; i < gN; i++) {
        SuffixAutomation t(gS[i].length());
        t.extend(gS[i], (int)gS[i].length());
        SA.emplace_back(std::move(t));
    }

    gAns = vector<int>(gQ);
    gQIn = vector<pii>(gQ);
    gQuery = vector<unordered_map<int, int>>(gN);   // R --> (L, Q index)

    for (int i = 0; i < gQ; i++) {
        int L, R;
        scanf("%d %d", &L, &R);

        if (L > R)
            swap(L, R);

        gQIn[i] = mp(L, R);
        if (gQIndex[L].find(R) == gQIndex[L].end())
            gQIndex[L][R] = i;

        if (L == R)
            gAns[gQIndex[L][R]] = (int)gS[L].length();
        else
            gQuery[R][L] = gQIndex[L][R];
    }

    for (int i = gN - 1; i > 0; i--) {
        if (gQuery[i].empty())
            continue;

        for (auto it : gQuery[i]) {
            if (gS[i].length() > gS[it.first].length())
                gAns[it.second] = lcs(SA[i], gS[it.first]);
            else
                gAns[it.second] = lcs(SA[it.first], gS[i]);
        }
    }

    for (int i = 0; i < gQ; i++)
        printf("%d\n", gAns[gQIndex[gQIn[i].first][gQIn[i].second]]);

    return 0;
}
