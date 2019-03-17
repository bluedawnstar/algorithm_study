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

#define MOD     1000000007
#define MAXN    100000

int gN;
int gA[MAXN + 1];

/*
  case 1-a) ..., +2, +2
       1-b) ..., +1, +2
       1-c) ..., +2, -1, +2
  case 2-a) ..., +2, +1
       2-b) ..., +1, +1
  case 3) ..., +2, +1, -2 * n
  case 4) ..., +2, -1, -2 * n

  1. forward processing
    case 1) dp2[i] = dp2[i - 2] + dp1[i - 2] + dpBack1[i - 2]
    case 2) dp1[i] = dp2[i - 1] + dp1[i - 1]
    case 4) dpBack1[i - 1] = (case 1-a) - (case 1-c)
                           = dp2[i] - dpBack1[i - 2]
*/
ll solveDP() {
    ll ans = 0;

    vector<ll> dp1(gN);
    vector<ll> dp2(gN);
    vector<ll> dpBack1(gN);

    dp1[0] = 1ll;
    ans += dp1[0];

    if (gN == 1)
        return ans;

    // step #1 : forward
    dp1[1] = 1ll;
    ans += dp1[1];
    for (int i = 2; i < gN; i++) {
        ll step2 = (gA[i - 2] == 2) ? ((dp2[i - 2] + dp1[i - 2]) % MOD) : 0ll;  // (1-a) + (1-b)
        ll step1 = (dp2[i - 1] + dp1[i - 1]) % MOD;                             // (2-a) + (2-b)
        ll stepBack1 = (gA[i - 2] == 2) ? dpBack1[i - 2] : 0ll;                 // (1-c)

        dp2[i] = (step2 + stepBack1) % MOD;
        dp1[i] = step1;
        ans = (ans + dp1[i] + dp2[i]) % MOD;

        // case 3: (3) - (1-c)
        if (i >= 3 && gA[i] == 2 && gA[i - 3] == 2) {
            ll b = (dp2[i - 1] + MOD - dpBack1[i - 3]) % MOD;       // 1-c: ..., +2, -1, +2, +1
            ans = (ans + b) % MOD;
        }
        // case 4: (1-a) - (1-c)
        if (gA[i - 2] == 2) {
            dpBack1[i - 1] = (dp2[i] + MOD - dpBack1[i - 2]) % MOD; // 1-c: ..., +2, -1, +2
            ans = (ans + dpBack1[i - 1]) % MOD;
        }
    }

    // step #2 : backward
    ll cntA1 = 0, cntA2 = 0, cntB1 = 0, cntB2 = 0;
    for (int i = gN - 1; i >= 5; i--) {
        // case 3
        {
            ll& cntA = (i & 1) ? cntA1 : cntA2;
            if (gA[i] == 2 && gA[i - 3] == 2) {     // ...,     +2, +1, -2
                int j = i - 5;
                if (gA[j] == 2 && gA[j + 3] == 2) { // ..., +2, +2, +1, -2, -2
                    ll b = (dp2[j + 2] + MOD - dpBack1[j]) % MOD;
                    ans = (ans + b * ++cntA % MOD) % MOD;
                } else {
                    cntA = 0;
                }
            } else {
                cntA = 0;
            }
        }
        // case 4
        {
            ll& cntB = (i & 1) ? cntB1 : cntB2;
            if (gA[i - 2] == 2) {                   // ...,     +2, ..., -2
                int j = i - 4;
                if (gA[j] == 2 && gA[j + 3] == 2) { // ..., +2, +2, ..., -2, -2
                    ll b = (dp2[j + 2] + MOD - dpBack1[j]) % MOD;
                    ans = (ans + b * ++cntB % MOD) % MOD;
                } else {
                    cntB = 0;
                }
            } else {
                cntB = 0;
            }
        }
    }
    {
        ll& cntB = cntB2;
        if (gN > 4 && gA[2] == 2) {
            if (gA[0] == 2 && gA[3] == 2) {
                ll b = (dp2[2] + MOD - dpBack1[0]) % MOD;
                ans = (ans + b * ++cntB % MOD) % MOD;
            }
        }
    }

    return ans;
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T-- > 0) {
        cin >> gN;
        for (int i = 0; i < gN; i++)
            cin >> gA[i];
        cout << solveDP() << endl;
    }

    return 0;
}
