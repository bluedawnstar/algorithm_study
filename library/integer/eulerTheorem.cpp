#include <cmath>
#include <numeric>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "eulerTheorem.h"
#include "eulerTheoremEx.h"
#include "eulerTheorem-coprimePairs.h"
#include "eulerTheorem-gcdOfAllPairs.h"

#include "primeFactor.h"
#include "primeNumberBasic.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

const int NN = 1000000;
const int MOD = 1000000007;

static long long phiSum(int n) {
    long long res = 0;
    for (int i = 1; i <= n; i++)
        res += phi(i);
    return res;
}


static int gcd(int p, int q) {
    return q == 0 ? p : gcd(q, p % q);
}

static long long calcCoprimePairMultSum(int n) {
    long long res = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (gcd(i, j) == 1)
                res += 1ll * i * j;
        }
    }
    return res;
}

static long long calcCoprimePairMultSumMod(int n) {
    long long res = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (gcd(i, j) == 1)
                res = (res + 1ll * i * j) % MOD;
        }
    }
    return res;
}


static long long calcCoprimeMultSumOfAllPairs(int n) {
    long long res = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            int g = gcd(i, j);
            res += 1ll * i/g * j/g;
        }
    }
    return res;
}

static long long calcCoprimeMultSumOfAllPairsMod(int n) {
    long long res = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            int g = gcd(i, j);
            res = (res + 1ll * i/g * j/g) % MOD;
        }
    }
    return res;
}


static long long calcGcdSumOfAllPairs(int n) {
    long long res = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            res += gcd(i, j);
        }
    }
    return res;
}

static long long calcGcdSumOfAllPairsMod(int n) {
    long long res = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            res += gcd(i, j);
        }
    }
    return res % MOD;
}

void testEulerTheorem() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Euler Theorem -----------------------" << endl;
    {
        //int N = 1000000009;
        int N = 1009;
        for (int i = 1; i <= N; i++) {
            auto ans1 = phi(i);
            auto ans2 = phiFast(i);
            if (ans1 != ans2)
                cerr << "Mismatched!" << endl;
            assert(ans1 == ans2);
        }
    }
    {
        int n = 100000;
#ifdef _DEBUG
        n = 10000;
#endif
        auto ans = phiAll(n);
        for (int i = 0; i <= n; i++) {
            int gt = phi(i);
            if (ans[i] != gt)
                cout << "Mismatched at " << i << " : " << ans[i] << ", " << gt << endl;
            assert(ans[i] == gt);
        }
    }
    {
        int N = 10000;
#ifdef _DEBUG
        N = 1000;
#endif

        PhiSum solver(N);

        for (int i = 1; i <= N; i++) {
            auto ans1 = phiSum(i);
            auto ans2 = solver.sum(i);
            if (ans1 != ans2)
                cout << "Mismatched at " << i << " : " << ans1 << ", " << ans2 << endl;
            assert(ans1 == ans2);
        }
    }
    //-- multiplication-sum of all coprime pairs 
    {
        int N = 1000;
#ifdef _DEBUG
        N = 100;
#endif
        CoprimePairMultSum sum;
        sum.build(N);

        for (int i = 1; i <= N; i++) {
            auto ans = sum.sum(i);
            auto gt = calcCoprimePairMultSum(i);
            if (ans != gt) {
                cout << "Mismatched : " << ans << ", " << gt << endl;
            }
            assert(ans == gt);
        }
    }
    {
        int N = 1000;
#ifdef _DEBUG
        N = 100;
#endif
        CoprimePairMultSumMod<int> sum;
        sum.build(N);

        for (int i = 1; i <= N; i++) {
            auto ans = sum.sum(i);
            auto gt = calcCoprimePairMultSumMod(i);
            if (ans != gt) {
                cout << "Mismatched : " << ans << ", " << gt << endl;
            }
            assert(ans == gt);
        }
    }
    //-- coprime multiplication-sum of all pairs 
    {
        int N = 1000;
#ifdef _DEBUG
        N = 100;
#endif
        CoprimeMultSumOfAllPairs sum;
        sum.build(N);

        for (int i = 1; i <= N; i++) {
            auto ans = sum.sum(i);
            auto gt = calcCoprimeMultSumOfAllPairs(i);
            if (ans != gt) {
                cout << "Mismatched : " << ans << ", " << gt << endl;
            }
            assert(ans == gt);
        }
    }
    {
        int N = 1000;
#ifdef _DEBUG
        N = 100;
#endif
        CoprimeMultSumOfAllPairsMod<int> sum;
        sum.build(N);

        for (int i = 1; i <= N; i++) {
            auto ans = sum.sum(i);
            auto gt = calcCoprimeMultSumOfAllPairsMod(i);
            if (ans != gt) {
                cout << "Mismatched : " << ans << ", " << gt << endl;
            }
            assert(ans == gt);
        }
    }
    //-- GCD-sum of all pairs 
    {
        int N = 1000;
#ifdef _DEBUG
        N = 100;
#endif
        GcdSumOfAllPairs sum;
        sum.build(N);

        for (int i = 1; i <= N; i++) {
            auto ans = sum.sum(i);
            auto gt = calcGcdSumOfAllPairs(i);
            if (ans != gt) {
                cout << "Mismatched : " << ans << ", " << gt << endl;
            }
            assert(ans == gt);
        }
    }
    {
        int N = 1000;
#ifdef _DEBUG
        N = 100;
#endif
        GcdSumOfAllPairsMod<int> sum;
        sum.build(N);

        for (int i = 1; i <= N; i++) {
            auto ans = sum.sum(i);
            auto gt = calcGcdSumOfAllPairsMod(i);
            if (ans != gt) {
                cout << "Mismatched : " << ans << ", " << gt << endl;
            }
            assert(ans == gt);
        }
    }

    cout << "OK!" << endl;
}
