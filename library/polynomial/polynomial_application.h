#pragma once

/*
  1. Question
        https://www.codechef.com/problems/WEIRDMUL

                 n    n                         r
        P(x) = PROD PROD W(i,j)^2  ,  W(l,r) = SUM A(i) * X^(i-l)
                i=1  j=i                       i=l

  2. Solution
    1)

                n    n                                           i
        P(x) = PROD PROD { (F(j) - F(i-1)) / X^i }^2  ,  F(i) = SUM A(j) * X^j , F(0) = 0
               i=1  j=i                                         j=1

                  n    n
        P(x) = { PROD PROD (F(j) - F(i))^2 } / X^{2*(1*N + 2*(N-1) + 3*(N-2) ... + N*i)}
                 i=0  j=i+1

                  n    n
             = { PROD PROD (-1) * (F(j) - F(i)) * (F(i) - F(j)) } / X^{2*(1*N + 2*(N-1) + 3*(N-2) ... + N*i)}
                 i=0  j=i+1

                  n    n
             = { PROD PROD (F(j) - F(i)) * (F(i) - F(j)) } * (-1)^{N*(N+1)/2} / X^{2*(1*N + 2*(N-1) + 3*(N-2) ... + N*i)}
                 i=0  j=i+1

                  n     n
             = { PROD  PROD  (F(i) - F(j)) } * (-1)^{N*(N+1)/2} / X^{2*(1*N + 2*(N-1) + 3*(N-2) ... + N*i)}
                 i=0  j=0(j!=i)

                  n
             = { PROD A(F(i)) } * (-1)^{N*(N+1)/2} / X^{2*(1*N + 2*(N-1) + 3*(N-2) ... + N*i)}
                 i=0

                                n
                    <== A(x) = PROD (x - F(j))
                              j=0(j!=i)

    2)

                n
        G(x) = PROD (x - F(i))
               i=0

                 n   n
    <=> G'(x) = SUM PROD (x - F(i))
                i=0 j=0(j!=i)

     => G'(F(i)) = A(F(i))


    3)

                  n
        P(x) = { PROD A(F(i)) } * (-1)^{N*(N+1)/2} / X^{2*(1*N + 2*(N-1) + 3*(N-2) ... + N*i)}
                 i=0

                  n
             = { PROD G'(F(i)) } * (-1)^{N*(N+1)/2} / X^{2*(1*N + 2*(N-1) + 3*(N-2) ... + N*i)}
                 i=0

  3. Algorithm to solve

    1) calculate

        F(0) = 0,
        F(i) = F(i - 1) + A[i] * X^i    (1 <= i)

    2) calculate G(x)

        G(x) = (x - F(0))(x - F(1))...(x - F(n))

    3) calculate G'(x)

        G'(x) = (x - F(0))(x - F(1))...(x - F(n))

    4) evaluate multipoints

        { G'(F(0)), G'(F(1)), G'(F(2)), ..., G'(F(n)) }
      = { A(F(0)), A(F(1)), A(F(2)), ..., A(F(n)) }

    5) calculate P(X)

                  n
        P(X) = { PROD G'(F(i)) } * (-1)^{N*(N+1)/2} / X^{2*(1*N + 2*(N-1) + 3*(N-2) ... + N*i)}
                 i=0

*/
template <int mod = 1000000007>
struct WeirdProductWithPolynomial {
    static int solve(const int A[], int N, int X) {
        vector<ModInt<int,mod>> F(N + 1);
        {
            ModInt<int, mod> powX = X;
            for (int i = 1; i <= N; i++) {
                F[i] = F[i - 1] + powX * A[i - 1];
                powX *= X;
            }
        }

        // G(x) = (x - F(0))(x - F(1))...(x - F(n))
        vector<poly<ModInt<int, mod>>> tree(4 * N);
        build(tree, 1, begin(F), end(F));

        // G'(x) = ...
        auto derivG = tree[1].derivate();

        // { G'(F(0)), G'(F(1)), G'(F(2)), ..., G'(F(n)) }
        auto vals = derivG.eval(tree, 1, begin(F), end(F));

        // res = G'(F(0)) * G'(F(1)) * G'(F(2)) * ... * G'(F(n))
        ModInt<int, mod> res = 1;
        for (auto v : vals)
            res *= v;

        if ((1ll * N * (N + 1) / 2) & 1)
            res = -res;

        // denominator
        long long exp = 0;
        for (int i = 1; i <= N; i++)
            exp += 2ll * i * (N - i + 1);
        int denom = modInv(modPow(X, exp));

        res *= denom;

        return int(res.value);
    }

    static int solve(const vector<int>& A, int X) {
        return solve(A.data(), int(A.size()), X);
    }

private:
    static int modPow(int x, int n) {
        if (n == 0)
            return 1;

        long long t = x % mod;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % mod;
            t = t * t % mod;
        }
        return int(res);
    }

    static int modPow(int x, long long n) {
        return modPow(x, int(n % (mod - 1)));
    }

    static int modInv(int a) {
        return modPow(a, mod - 2);
    }
};
