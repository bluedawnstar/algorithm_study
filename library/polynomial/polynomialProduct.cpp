#include <complex>
#include <numeric>
#include <vector>
#include <algorithm>

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

using namespace std;

#include "polynomialProduct.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

//<Related problems>
// https://www.hackerearth.com/problem/algorithm/stange-product-sums-e3bf6340/

namespace polynomial_product {

#include "polynomial.h"
#include "modInt.h"

using namespace algebra;

const int MOD = 998'244'353;
typedef ModInt<int, MOD> baseT;
typedef poly<baseT> polyT;

struct PolynomialProductSolver {
    vector<int> factorial;
    vector<int> invFactorial;

    void prepare(int K) {
        factorial.resize(K + 2);
        invFactorial.resize(K + 2);

        factorial[0] = 1;
        for (int i = 1; i <= K + 1; i++)
            factorial[i] = int(1ll * factorial[i - 1] * i % MOD);

        invFactorial[K + 1] = modInv(factorial[K + 1]);
        for (int i = K; i >= 0; i--) {
            invFactorial[i] = int(1ll * invFactorial[i + 1] * (i + 1) % MOD);
        }
    }

    vector<int> solve(int A, int D, int K, int N) {
        /*
            * b_j = a + (j - 1) * d

                        N
                G(x) = SUM e^(b_j * x)
                        j=1

                    = e^(a*x) * { e^0 + e^(d*x) + e^(2*d*x) + ... + e^((N-1)*d*x) }

                                e^(N*d*x) - 1
                    = e^(a*x) * ---------------
                                    e^(d*x) - 1

                                    <=== e^x = x^0/0! + x^1/1! + x^2/2! + x^3/3! + ...

                                (N*d*x)^1/1! + (N*d*x)^2/2! + (N*d*x)^3/3! + ...
                    = e^(a*x) * --------------------------------------------------
                                (d*x)^1/1! + (d*x)^2/2! + (d*x)^3/3! + ...

                                N*d*x * { (N*d*x)^0/1! + (N*d*x)^1/2! + (N*d*x)^2/3! + ... }
                    = e^(a*x) * --------------------------------------------------------------
                                d*x * { (d*x)^0/1! + (d*x)^1/2! + (d*x)^2/3! + ... }

                                N * { (N*d*x)^0/1! + (N*d*x)^1/2! + (N*d*x)^2/3! + ... }
                    = e^(a*x) * ----------------------------------------------------------
                                    { (d*x)^0/1! + (d*x)^1/2! + (d*x)^2/3! + ... }
        */
        vector<int> expNDx(K + 2), expDx(K + 2);
        int powD = 1, powND = 1;
        for (int i = 1; i <= K + 1; i++) {
            expDx[i - 1] = int(1ll * powD * invFactorial[i] % MOD);
            expNDx[i - 1] = int(1ll * powND * invFactorial[i] % MOD);
            powD = int(1ll * powD * D % MOD);
            powND = int(1ll * powND * N % MOD * D % MOD);
        }

        auto G = polyT(expNDx) * polyT(expDx).inverse(K + 1);
        G.a.resize(K + 1);

        vector<int> expAx(K + 1);
        int powA = 1;
        for (int i = 0; i <= K; i++) {
            expAx[i] = int(1ll * powA * invFactorial[i] % MOD * N % MOD);
            powA = int(1ll * powA * A % MOD);
        }
        G *= polyT(expAx);

        /*
                            inf   x^k    N
                Q(x) = e^{ SUM { --- * SUM (b_j)^k } }
                            k=1    k    j=1

                    = e^{ [x^1]G(x) * 0! + [x^2]G(x) * 1! + [x^3]G(x) * 2! + ... }

                            inf
                    = e^{ SUM { [x^k]G(x) * (k-1)! } * x^k }
                            k=1
        */
        G.a.resize(K + 1);
        for (int i = 1; i <= K; i++)
            G.a[i] = G.a[i] * baseT(factorial[i - 1]);
        G.a[0] = 0;

        auto Q = polyT(G).exp(K + 1);

        vector<int> res(K + 1);
        for (int i = 0; i <= K; i++)
            res[i] = Q[i];

        return res;
    }

private:
    static int modPow(int x, int n) {
        if (n == 0)
            return 1;

        long long t = x % MOD;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % MOD;
            t = t * t % MOD;
        }
        return int(res);
    }

    static int modInv(int a) {
        return modPow(a, MOD - 2);
    }
};

} // namespace polynomial_product


void testPolynomialProduct() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Polynomial Product -----------------------" << endl;
    {
        const int A = 1, D = 3, K = 3, N = 3;

        polynomial_product::PolynomialProductSolver solver;
        solver.prepare(K);

        auto ans = solver.solve(A, D, K, N);
        auto gt = vector<int>{ 1, 12, 105, 820 };
        if (ans != gt) {
            cout << "Mismatched : ans = " << ans << ", gt = " << gt << endl;
        }
        assert(ans == gt);
    }
    {
        const int A = 2, D = 1, K = 2, N = 4;

        polynomial_product::PolynomialProductSolver solver;
        solver.prepare(K);

        auto ans = solver.solve(A, D, K, N);
        auto gt = vector<int>{ 1, 14, 125 };
        if (ans != gt) {
            cout << "Mismatched : ans = " << ans << ", gt = " << gt << endl;
        }
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
