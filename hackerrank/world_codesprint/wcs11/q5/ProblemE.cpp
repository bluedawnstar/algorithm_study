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

#define MAXN    100000

int gN;
vector<int> gA;

bitset<(1 << 27)> gUsed;

clock_t gStartTime;

bool checkTime() {
#ifdef _DEBUG
    return false;
#else
    return ll(clock() - gStartTime) * 1000 / CLOCKS_PER_SEC >= 990;
#endif
}

bool check(vector<int>& A, int mask) {
    for (int i = 0; i < (int)A.size(); i++) {
        if ((A[i] & mask) == 0)
            return false;
    }
    return true;
}

int bitCnt(int x) {
    int res = 0;
    while (x) {
        x &= x - 1;
        res++;
    }
    return res;
}

int main(void) {
    gStartTime = clock();

    PROFILE_START(0);

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> gN;

    gA.resize(gN);

    int andV = -1;
    int allBit = 0;
    for (int i = 0; i < gN; i++) {
        cin >> gA[i];

        allBit |= gA[i];
        andV &= gA[i];
    }
    if (andV) {
        cout << (andV & -andV) << endl;
        return 0;
    }

    int mask = 1;
    while (mask <= allBit)
        mask <<= 1;
    mask--;

    PROFILE_STOP(0);
    PROFILE_START(1);

    vector<vector<int>> P(2);

    for (int i = 0; i < gN; i++) {
        int v = ~gA[i] & mask; // allBit;
        if (v == 0 || gUsed.test(v))
            continue;
        P[0].push_back(v);
        gUsed[v] = true;
    }

    int from = 0;
    int to = 1;
    while (!P[from].empty()) {
        P[to].clear();

        for (auto u : P[from]) {
            for (int t = u; t; t &= t - 1) {
                int v = u & ~(t & -t);
                if (v == 0 || gUsed.test(v))
                    continue;
                P[to].push_back(v);
                gUsed[v] = true;
            }
        }
        swap(from, to);
    }

    PROFILE_STOP(1);

    PROFILE_START(2);
    int ans = allBit;
    int bitN = bitCnt(ans);
    for (int subset = allBit; subset; subset = (subset - 1) & allBit) {
        if (!gUsed.test(subset)) {
            int bn = bitCnt(subset);
            if (bn < bitN || bn == bitN && subset < ans) {
                ans = subset;
                bitN = bn;
            }
        }
    }
    PROFILE_STOP(2);

    cout << ans << endl;

    return 0;
}
