#pragma once

/*
  1. problem
      https://www.codechef.com/problems/CHEFSUMS
      https://discuss.codechef.com/t/chefsums-editorial/75259

      SUM G(T) * W(T) * Sx(T) * Pm(T)
       T

      G(T) = gcd(a[i_1], a[i_2], ..., a[i_K])
      Sx(T) = a[i_1]^X + a[i_2]^X + ... + a[i_K]^X
      Pm(T) = (a[i_1] * a[i_2] * ... * a[i_K])^M
      W(T) = smallest prime divisor of P1(T)

      T = (i_1, i_2, ... i_K)
      i_j = one of { 0, 1, 2, ..., N - 1 }
      a[i_j] = one of { A[0], A[1], ..., A[N - 1] }

  2. solution
    1) Sx(Ti) * Pm(Ti)
       =  (a[0]^x + a[1]^x + ... + a[K-1]^x)*(a[0]^m * a[1]^m * ... * a[K-1]^m)
       =  a[0]^(m+x) * a[1]^m     * ... * a[K-1]^m
        + a[0]^m     * a[1]^(m+x) * ... * a[K-1]^m
        + ...
        + a[0]^m     * a[1]^m     * ... * a[K-1]^(m+x)

       =  A[i_0]^(freq[i_0]*(m+x)) * A[i_1]^(freq[i_1]*m)     * ... * A[i_z]^(freq[i_z]*m)
        + A[i_0]^(freq[i_0]*m)     * A[i_1]^(freq[i_1]*(m+x)) * ... * A[i_z]^(freq[i_z]*m)
        + ...
        + A[i_0]^(freq[i_0]*m)     * A[i_1]^(freq[i_1]*m)     * ... * A[i_z]^(freq[i_z]*(m+x))

    2) SUM G(T) * W(T) * Sx(T) * Pm(T)

         MAXX
       = SUM g * W(T) * Sx(T) * Pm(T)
         g=1

         MAXX
       = SUM g * SUM W(Ti) * Sx(Ti) * Pm(Ti)
         g=1   gcd(Ti)=g

         MAXX
       = SUM g * SUM p * SUM Sx(Ti) * Pm(Ti)      , W(a,b) is a min value of min prime numbers of a and b
         g=1      p     W(g,Ti)=p
                        gcd(Ti)=g

         MAXX           MAXX/g
       = SUM g * SUM p * SUM { cases including (g*j)^(M+X) } }
         g=1      p      j=1
                       W(g,j)=p

         MAXX                 MAXX/g
       = SUM g * SUM p * { K * SUM F(g,j) }
         g=1      p            j=1
                             W(g,j)=p

         F(g,j) = { SUM Vm[l] }^(K-1) * { SUM Vx[l] } - { SUM Vm[l] }^(K-1) * { SUM Vx[l] }
                    l=j                   l=j           l=next(j)             l=next(j)
                  W(g,l)=p              W(g,l)=p        W(g,l)=p              W(g,l)=p

                 MAXX/g
         Vm[i] =  SUM (g * j)^M
               j=1,W(g,j)=i

                 MAXX/g
         Vx[i] =  SUM (g * j)^(M + X)
               j=1,W(g,j)=i
*/
template <int mod = 1'000'000'007>
struct ChefAndSumSolver2 {
    // O(N*(logN)^2)
    static int solve(int maxX, const int A[], int N, long long K, long long M, long long X) {
        vector<int> primes;
        vector<int> minPrimeFactors;
        primeFactorization(maxX, primes, minPrimeFactors);

        vector<int> phi = phiAll(maxX);

        vector<int> freq(maxX + 1);
        for (int i = 0; i < N; i++)
            freq[A[i]]++;

        vector<int> powM(maxX + 1);
        vector<int> powXM(maxX + 1);
        for (int i = 1; i <= maxX; i++) {
            powM[i] = int(1ll * freq[i] * modPowLL(i, M) % mod);
            powXM[i] = int(1ll * freq[i] * modPowLL(i, X + M) % mod);
        }

        vector<int> vM(maxX + 1);
        vector<int> vXM(maxX + 1);
        vector<bool> vis(maxX + 1);

        vector<int> val;

        int res = 0;
        for (int i = 1; i <= maxX; i++) {
            val.clear();

            for (int j = i; j <= maxX; j += i) {
                if (freq[j]) {
                    int cur = minPrimeFactors[j];
                    if ((vM[cur] += powM[j]) >= mod)
                        vM[cur] -= mod;
                    if ((vXM[cur] += powXM[j]) >= mod)
                        vXM[cur] -= mod;
                    if (!vis[cur]) {
                        vis[cur] = true;
                        val.push_back(cur);
                    }
                }
            }
            sort(val.begin(), val.end(), [](int a, int b) { return a > b; });

            int sum = 0;
            int curXM = 0, curM = 0, pre = 0;
            for (auto p : val) {
                if ((curM += vM[p]) >= mod)
                    curM -= mod;
                if ((curXM += vXM[p]) >= mod)
                    curXM -= mod;
                int cur = int(K % mod * modPowLL(curM, K - 1) % mod * curXM % mod);
                sum = int((sum + 1ll * p * (cur - pre + mod)) % mod);

                pre = cur;
                vM[p] = vXM[p] = 0;
                vis[p] = false;
            }
            res = int((res + 1ll * sum * phi[i]) % mod);
        }

        return res;
    }

    static int solve(int maxX, const vector<int>& A, long long K, long long M, long long X) {
        return solve(maxX, A.data(), int(A.size()), K, M, X);
    }

private:
    static void primeFactorization(int n, vector<int>& primes, vector<int>& minPrimeFactors) {
        primes.clear();
        minPrimeFactors = vector<int>(n + 1);

        if (n >= 2) {
            primes.push_back(2);
            minPrimeFactors[2] = 2;
            for (int j = 2 * 2; j <= n; j += 2)
                minPrimeFactors[j] = 2;
        }

        for (int i = 3; i <= n; i += 2) {
            if (!minPrimeFactors[i]) {
                primes.push_back(i);
                minPrimeFactors[i] = i;

                if (1ll * i * i <= n) {
                    for (int j = i * i; j <= n; j += i) {
                        if (!minPrimeFactors[j])
                            minPrimeFactors[j] = i;
                    }
                }
            }
        }
    }

    static vector<int> phiAll(int n) {
        vector<int> res(n + 1);
        iota(res.begin(), res.end(), 0);

        for (int p = 2; p <= n; p++) {
            if (res[p] == p) {
                res[p] = p - 1;
                for (int i = 2 * p; i <= n; i += p)
                    res[i] = (res[i] / p) * (p - 1);
            }
        }

        return res;
    }

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

    // mod is a prime number
    static int modPowLL(int x, long long n) {
        if (x == 0 && n > 0)
            return 0;
        return modPow(x, int(n % (mod - 1)));
    }
};
