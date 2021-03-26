#pragma once

/*
  The number of labeled graphs without isolated vertices
  = the number of edge covers of the complete graph K_n
  - https://oeis.org/A006129

       n
      SUM C(n,i) * F(i) = 2^C(n,2)
      i=0

  <=>
                        n-1
      F(n) = 2^C(n,2) - SUM C(n,i) * F(i)
                        i=0

              n
           = SUM { (-1)^(n - i) * C(n,i) * 2^C(i,2) }
             i=0


    i    | F(i)
  -------+----------------------
    0    | 1
    1    | 0
    2    | 1
    3    | 4
    4    | 41
    5    | 768
    6    | 27449
    7    | 1887284
    8    | 252522481
    9    | 66376424160
    10   | 34509011894545
    11   | 35645504882731588
    12   | 73356937912127722841
    13   | 301275024444053951967648
    14   | 2471655539737552842139838345
    15   | 40527712706903544101000417059892
    16   | 1328579255614092968399503598175745633
       . . .
*/
template <int mod = 1'000'000'007>
struct LabeledGraphCounter {
    int N;
    vector<int> factorial;      // factorial
    vector<int> factInverse;    // inverse factorial
    vector<int> inverse;

    LabeledGraphCounter() {
    }

    explicit LabeledGraphCounter(int maxN) {
        init(maxN);
    }

    // O(N)
    void init(int maxN) {
        N = max(1, maxN);

        factorial.resize(maxN + 1);
        factInverse.resize(maxN + 1);
        inverse.resize(maxN + 1);

        inverse[0] = 0;
        inverse[1] = 1;
        for (int i = 2; i <= maxN; i++)
            inverse[i] = int((mod - 1ll * (mod / i) * inverse[mod % i] % mod) % mod);

        factorial[0] = 1;
        factInverse[0] = 1;

        factorial[1] = 1;
        factInverse[1] = 1;

        for (int i = 2; i <= maxN; i++) {
            factorial[i] = int(1ll * factorial[i - 1] * i % mod);
            factInverse[i] = int(1ll * factInverse[i - 1] * inverse[i] % mod);
        }
    }

    int count(int n) {
        long long res = 0;

        /*
                    n
            F(n) = SUM { (-1)^(n - i) * C(n,i) * 2^C(i,2) }
                   i=0
        */
        for (int i = 0; i <= n; i++) {
            long long cc = 1ll * comb(n, i) * modPow(2, int(1ll * i * (i - 1) / 2 % (mod - 1))) % mod;
            if ((n - i) & 1) {
                res -= cc;
                if (res < 0)
                    res += mod;
            } else {
                res += cc;
                if (res >= mod)
                    res -= mod;
            }
        }

        return int(res);
    }

private:
    // nCr, O(1)
    int comb(int n, int r) {
        if (n < r)
            return 0;

        if (n == 0 || r == 0 || n == r)
            return 1;

        if (n - r < r)
            r = n - r;

        return int(1ll * factorial[n] * factInverse[n - r] % mod * factInverse[r] % mod);
    }

    // O(logn), mod must be a prime number
    static int modPow(long long x, int n) {
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

    // O(logn), mod must be a prime number
    static int modInv(long long a) {
        return modPow(a, mod - 2);
    }
};
