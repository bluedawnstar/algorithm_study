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
#define MAXV    (1 << 26)

int gN;
vector<int> gA;

unsigned int gDP[MAXV + 10];
int gB[1 << 16];

void setB(int x, bool val) {
    if (val)
        gDP[x / 32] |= 1u << (x & 31);
    else
        gDP[x / 32] &= ~(1u << (x & 31));
}

int testB(int x) {
    return gDP[x / 32] & (1u << (x & 31));
}

bool check(vector<int>& A, int mask) {
    for (int i = 0; i < (int)A.size(); i++) {
        if ((A[i] & mask) == 0)
            return false;
    }
    return true;
}

int bitCnt(int x) {
    return gB[x >> 16] + gB[x & ((1 << 16) - 1)];
}

void makeBitCountTable() {
    for (int i = 1; i < (1 << 16); i++)
        gB[i] = gB[i & (i - 1)] + 1;
}

int main(void) {
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

    makeBitCountTable();

    int mask = 1;
    while (mask <= allBit)
        mask <<= 1;
    mask--;

    for (int i = 0; i < gN; i++) {
        int v = ~gA[i] & mask;
        setB(v, true);
    }

    for (int i = mask >> 5; i >= 0; i--) {
        for (int j = 31; j > 0; j--) {
            if (gDP[i] & (1u << j)) {
                for (int t = j; t; t &= t - 1) {
                    int v = j & ~(t & -t);
                    gDP[i] |= 1u << v;
                }
            }
        }
        for (int t = i; t; t &= t - 1) {
            int v = i & ~(t & -t);
            gDP[v] |= gDP[i];
        }
    }

    int ans = allBit;
    int bitN = bitCnt(ans);
    for (int subset = allBit; subset; subset = (subset - 1) & allBit) {
        if (!testB(subset)) {
            int bn = bitCnt(subset);
            if (bn < bitN || bn == bitN && subset < ans) {
                ans = subset;
                bitN = bn;
            }
        }
    }

    cout << ans << endl;

    return 0;
}
