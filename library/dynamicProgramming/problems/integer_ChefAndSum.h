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
struct ChefAndSumSolver {
    // O(N*(logN)^2)
    static int solve(int maxX, const int A[], int N, long long K, long long M, long long X) {
        vector<int> primes;
        vector<int> minPrimeFactors;
        primeFactorization(maxX, primes, minPrimeFactors);

        vector<int> freq(maxX + 1);
        for (int i = 0; i < N; i++)
            freq[A[i]]++;

        vector<int> primeCount(maxX + 1);
        vector<int> powM(maxX + 1);
        vector<int> powXM(maxX + 1);
        for (int i = 0, j = 0; i <= maxX; i++) {
            if (j < int(primes.size()) && primes[j] == i)
                j++;
            primeCount[i] = j;
            powM[i] = int(1ll * freq[i] * modPowLL(i, M) % mod);
            powXM[i] = int(1ll * freq[i] * modPowLL(i, X + M) % mod);
        }

        int res = 0;

        vector<int> vM(primes.size());
        vector<int> vXM(primes.size());

        vector<int> dp(maxX + 1);

        for (int g = maxX; g > 1; g--) {
            int siz = primeCount[maxX / g];
            int minPF = minPrimeFactors[g];

            int sum = 0;
            int curM = 0, curXM = 0;
            int prvP = 0, curP = 0;

            if (primeCount[minPF] > siz) {
                if (freq[g] > 0) {
                    if ((curM += powM[g]) >= mod)
                        curM -= mod;
                    if ((curXM += powXM[g]) >= mod)
                        curXM -= mod;
                    prvP = curP;
                    curP = int(1ll * modPowLL(curM, K - 1) * curXM % mod);
                    sum = int((sum + 1ll * minPF * (curP - prvP + mod)) % mod);
                }
            } else {
                if (freq[g] > 0) {
                    int idx = primeCount[minPF] - 1;
                    if ((vM[idx] += powM[g]) >= mod)
                        vM[idx] -= mod;
                    if ((vXM[idx] += powXM[g]) >= mod)
                        vXM[idx] -= mod;
                }
            }

            /*
                                 maxX/g
                Vm[minPF(g, w)] = SUM (g * w)^M        , w >= 2
                                  w=2,minPF(g,w)

                                 maxX/g
                Vx[minPF(g, w)] = SUM (g * w)^(M + X)  , w >= 2
                                  w=2,minPF(g,w)
            */
            for (int r = g << 1, w = 2; r <= maxX; r += g, w++) {
                if (freq[r] == 0)
                    continue;
                int idx = primeCount[min(minPrimeFactors[w], minPF)] - 1;
                if ((vM[idx] += powM[r]) >= mod)
                    vM[idx] -= mod;
                if ((vXM[idx] += powXM[r]) >= mod)
                    vXM[idx] -= mod;
            }

            /*
                curP = { SUM Vm[i] }^(K - 1) * SUM Vx[i]
                          i                     i

                sum += p * SUM { Sm(Ti) * Pm(Ti) } = p * SUM (curP - prvP)
                         minPF(Ti)=p                  minPF(Ti)=p
            */
            for (int i = min(primeCount[minPF], siz) - 1; i >= 0; i--) {
                if ((curM += vM[i]) >= mod)
                    curM -= mod;
                if ((curXM += vXM[i]) >= mod)
                    curXM -= mod;

                prvP = curP;
                vM[i] = vXM[i] = 0;

                curP = int(1ll * modPowLL(curM, K - 1) * curXM % mod);
                sum = int((sum + 1ll * primes[i] * (curP - prvP + mod)) % mod);
            }

            /*
                subtract duplicates

                      maxX/g
                sum -= SUM dp[g * j]
                       j=2
            */
            for (int d = g << 1; d <= maxX; d += g) {
                if ((sum -= dp[d]) < 0)
                    sum += mod;
            }
            dp[g] = sum;

            /*
                res += g * SUM W(g,Ti) * Sx(Ti) * Pm(Ti)  ,  g >= 2
                         gcd(Ti)=g
            */
            if ((res += int(1ll * g * sum % mod)) >= mod)
                res -= mod;
        }
        // g = 1
        {
            int siz = int(primes.size());

            int sum = 0;
            int curM = 0, curXM = 0;
            int prvP = 0, curP = 0;

            for (int r = 2; r <= maxX; r++) {
                int idx = primeCount[minPrimeFactors[r]] - 1;
                if ((vM[idx] += powM[r]) >= mod)
                    vM[idx] -= mod;
                if ((vXM[idx] += powXM[r]) >= mod)
                    vXM[idx] -= mod;
            }

            for (int i = siz - 1; i >= 0; i--) {
                if ((curM += vM[i]) >= mod)
                    curM -= mod;
                if ((curXM += vXM[i]) >= mod)
                    curXM -= mod;

                prvP = curP;
                vM[i] = vXM[i] = 0;

                curP = int(1ll * modPowLL(curM, K - 1) * curXM % mod);
                sum = int((sum + 1ll * primes[i] * (curP - prvP + mod)) % mod);
            }

            for (int d = 2; d <= maxX; d++) {
                if ((sum -= dp[d]) < 0)
                    sum += mod;
            }
            dp[1] = sum;

            if ((res += sum) >= mod)
                res -= mod;
        }

        return int(res * (K % mod) % mod);
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
