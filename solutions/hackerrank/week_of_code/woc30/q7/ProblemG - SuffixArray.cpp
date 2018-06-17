#include <memory.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cctype>
#include <cstring>
#include <climits>
#include <cmath>
#include <cassert>
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


//https://www.hackerrank.com/contests/w30/challenges/substring-queries

#define MAXN        50000
#define MAXS        100000

int gN, gQ;
vector<pii> gS;
string gSS;

int gSuffixToStrID[MAXS + MAXN + 1];

char gT[MAXN + 10];

vector<int> gSA;
vector<int> gLCP;

vector<int> gAns;
vector<pii> gQIn;
unordered_map<int, unordered_map<int, int>> gQIndex; // (L, R) -> Q index
vector<unordered_map<int,int>> gQuery;     // R --> (L, Q index)

template <typename T>
vector<int> makeSuffixArrayFast(T s, int n) {
    vector<int> SA(n);
    if (n <= 1)
        return SA;

    int m = 27;                                     // alphabet size + 1
    vector<int> cnt(max(n, m)), currG(n), nextG(n);

    for (int i = 0; i < n; i++) {
        currG[i] = s[i] - 'a';
        ++cnt[currG[i]];
    }
    for (int i = 1; i < m; i++)
        cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; i--)
        SA[--cnt[currG[i]]] = i;

    int len = 1;
    do {
        int g = 0;
        for (int i = n - len; i < n; i++)
            nextG[g++] = i;
        for (int i = 0; i < n; i++) {
            if (SA[i] >= len)
                nextG[g++] = SA[i] - len;
        }

        fill(cnt.begin(), cnt.begin() + m, 0);
        for (int i = 0; i < n; i++)
            ++cnt[currG[nextG[i]]];
        for (int i = 1; i < m; i++)
            cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; i--)
            SA[--cnt[currG[nextG[i]]]] = nextG[i];

        int curSA = SA[0];
        nextG[curSA] = g = 0;
        for (int i = 1; i < n; i++) {
            int prvSA = curSA;
            curSA = SA[i];

            nextG[curSA] = (prvSA + len < n && curSA + len < n
                && currG[prvSA] == currG[curSA]
                && currG[prvSA + len] == currG[curSA + len])
                ? g : ++g;
        }
        swap(currG, nextG);

        len <<= 1;
        m = g + 1;
    } while (m < n);

    return SA;
}

template <typename T>
vector<int> makeLcpArray(const vector<int>& suffixArray, T s, int n) {
    vector<int> rank(n), height(n);
    for (int i = 0; i < n; i++)
        rank[suffixArray[i]] = i;

    for (int i = 0, h = 0; i < n; i++) {
        if (rank[i] > 0) {
            int j = suffixArray[rank[i] - 1];
            while (i + h < n && j + h < n && s[i + h] == s[j + h])
                h++;
            height[rank[i]] = h;
            if (h > 0)
                h--;
        }
    }

    return height;
}

struct SparseTableMin {
    int N;
    vector<vector<int>> value;
    vector<int> H;

    template <typename U>
    SparseTableMin(const U& a, int n) {
        this->N = n;

        H.resize(n + 1);
        H[1] = 0;
        for (int i = 2; i < (int)H.size(); i++)
            H[i] = H[i >> 1] + 1;

        value.resize(H.back() + 1, vector<int>(n));
        for (int i = 0; i < n; i++)
            value[0][i] = a[i];

        for (int i = 1; i < (int)value.size(); i++) {
            vector<int>& prev = value[i - 1];
            vector<int>& curr = value[i];
            for (int v = 0; v < n; v++) {
                if (v + (1 << (i - 1)) < n)
                    curr[v] = min(prev[v], prev[v + (1 << (i - 1))]);
                else
                    curr[v] = prev[v];
            }
        }
    }

    int query(int left, int right) {
        right++;
        if (right <= left)
            return INT_MAX;

        int k = H[right - left];
        vector<int>& mink = value[k];
        return min(mink[left], mink[right - (1 << k)]);
    }
};

struct LcpArraySparseTable {
    SparseTableMin lcpTable;

    // PRECONDITION: lcpArray[i] = LCP(SA[i], SA[i - 1])
    template <typename T>
    LcpArraySparseTable(const T& lcpArray, int n)
        : lcpTable(lcpArray, n) {
        // no action
    }

    // inclusive (left SA index, right SA index)
    // CAUTION: if left == right, lcp() will return INT_MAX
    int lcp(int left, int right) {
        int res = lcpTable.query(left + 1, right);
        return res;
    }
};

// 
// 1) how to reduce redundant questions
//      gQuery[R][L] = the index of answer to (L, R) or (R, L)
// 2) how to reduce the number of comparing between two suffixes 
//      update forward and backward instead of making all pairs
// 3) how to use suffix array, LCP array, sparse table, etc
//      
int main(void) {
    //ios_base::sync_with_stdio(false);
    //cin.tie(nullptr);

    scanf("%d %d", &gN, &gQ);

    gS.reserve(gN);
    for (int i = 0; i < gN; i++) {
        scanf("%s", gT);

        string s(gT);
        gS.emplace_back(gSS.length(), gSS.length() + s.length());

        int len = (int)gSS.length();
        gSS += s;
        gSS += char('z' + 1);

        for (int j = 0; j < (int)s.length(); j++)
            gSuffixToStrID[len + j] = i;
        gSuffixToStrID[len + s.length()] = -1;
    }

    gAns = vector<int>(gQ);
    gQIn = vector<pii>(gQ);
    gQuery = vector<unordered_map<int,int>>(gN);   // R --> (L, Q index)

    for (int i = 0; i < gQ; i++) {
        int L, R;
        scanf("%d %d", &L, &R);

        if (L > R)
            swap(L, R);

        gQIn[i] = mp(L, R);
        if (gQIndex[L].find(R) == gQIndex[L].end())
            gQIndex[L][R] = i;

        if (L == R)
            gAns[gQIndex[L][R]] = (int)gS[L].second - (int)gS[L].first;
        else
            gQuery[R][L] = gQIndex[L][R];
    }

    gSA = makeSuffixArrayFast(gSS, (int)gSS.length());
    gLCP = makeLcpArray(gSA, gSS, (int)gSS.length());
    LcpArraySparseTable sparseTable(gLCP, (int)gLCP.size());

    vector<int> lastSAIndex(gN, -1);
    int prevStrID = -1;
    for (int i = 0; i < (int)gSA.size(); i++) {
        int rStrID = gSuffixToStrID[gSA[i]];
        if (rStrID < 0)
            continue;

        if (prevStrID != rStrID) {
            for (auto it : gQuery[rStrID]) {
                int lStrID = it.first;
                if (lastSAIndex[lStrID] < 0)
                    continue;

                int lcp = sparseTable.lcp(lastSAIndex[lStrID], i);

                int maxLen = min(gS[lStrID].second - gSA[lastSAIndex[lStrID]],
                                 gS[rStrID].second - gSA[i]);
                if (lcp > maxLen)
                    lcp = maxLen;

                gAns[it.second] = max(gAns[it.second], lcp);
            }
        }
        lastSAIndex[rStrID] = i;
        prevStrID = rStrID;
    }

    fill(lastSAIndex.begin(), lastSAIndex.end(), -1);
    prevStrID = -1;
    for (int i = (int)gSA.size() - 1; i >= 0; i--) {
        int rStrID = gSuffixToStrID[gSA[i]];
        if (rStrID < 0)
            continue;

        if (prevStrID != rStrID) {
            for (auto it : gQuery[rStrID]) {
                int lStrID = it.first;
                if (lastSAIndex[lStrID] < 0)
                    continue;

                int lcp = sparseTable.lcp(i, lastSAIndex[lStrID]);

                int maxLen = min(gS[lStrID].second - gSA[lastSAIndex[lStrID]],
                                 gS[rStrID].second - gSA[i]);
                if (lcp > maxLen)
                    lcp = maxLen;

                gAns[it.second] = max(gAns[it.second], lcp);
            }
        }
        lastSAIndex[rStrID] = i;
        prevStrID = rStrID;
    }

    for (int i = 0; i < gQ; i++)
        printf("%d\n", gAns[gQIndex[gQIn[i].first][gQIn[i].second]]);

    return 0;
}
