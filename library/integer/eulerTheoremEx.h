#pragma once

/*
1. basic formula

    1)
       phi(1) = 1, phi(2) = 1, phi(3) = 2, phi(4) = 2, phi(5) = 4, phi(6) = 2, ...

       phi(mn)  | = phi(m) * phi(n)                 ...... gcd(m,n) = 1
                | = phi(m) * phi(n) * {d / phi(d)}  ...... gcd(m,n) = d

       phi(2m)  | = 2 * phi(m)  .......................... if m is even
                | = phi(m)      .......................... if m is odd

       phi(n) = the number of coprimes to n in [1, n]
              = n*(1 - 1/p1)*(1 - 1/p2)*...*(1 - 1/pm) ... (p1, p2... : prime factors of n)

       phi(n) = n - 1  ................................... if n is a prime number

       phi(n^m) = n^(m - 1) * phi(n)

       phi(lcm(m,n)) * phi(gcd(m,n)) = phi(m) * phi(n)

    2) phi(n) = SUM mu(d)*(n/d)
                d|n

    3) SUM phi(d) = n
       d|n


2. extended formula
       
    1)         n             n   k
       f(n) = SUM phi(k) =  SUM SUM 1
              k=1           k=1 i=1,
                              gcd(i,k)=1

                             n
            = n*(n + 1)/2 - SUM f(n/i)
                            i=2

                           sqrt(n)      sqrt(n)
            = n*(n + 1)/2 - SUM f(n/i) - SUM {n/d - n/(d + 1)}*f(d)  ..... if d = n/d then remove duplicates
                            i=2          d=1

       --- prove ---

        n   k                     
       SUM SUM 1 = n*(n + 1)/2
       k=1 i=1
                    n   n   k       n  n/g  k
                 = SUM SUM SUM 1 = SUM SUM SUM 1
                   g=1 k=1 i=1,    g=1 k=1 i=1
                         gcd(i,k)=g      gcd(i,k)=1

                    n
                 = SUM f(n/g)
                   g=1

                          n
       <=> n*(n + 1)/2 = SUM f(n/g) + f(n)
                         g=2
                                 n
       <=> f(n) = n*(n + 1)/2 - SUM f(n/i)
                                i=2

           n/i = d  <=>  i*d <= n < i*(d + 1)
                    <=>  n/(d+1) < i <= n/d

           range size = n/d - n/(d+1) = n/(d*(d+1))
           therefore d <= sqrt(n)

                                 n
       <=> f(n) = n*(n + 1)/2 - SUM f(n/i)
                                i=2

                               sqrt(n)      sqrt(n)
                = n*(n + 1)/2 - SUM f(n/i) - SUM {n/d - n/(d + 1)}*f(d)  ..... if d = n/d then remove duplicates
                                i=2          d=1

    https://mathproblems123.wordpress.com/2018/05/10/sum-of-the-euler-totient-function/
    https://discuss.codechef.com/t/summation-of-eulers-totient-function/9848/2
    https://codeforces.com/blog/entry/7308
*/

/*
     n
    SUM phi(i)
    i=1
*/
struct PhiSum {
    vector<long long> dp;

    explicit PhiSum(int maxN) {
        dp.resize(maxN + 1);
    }

    //                    sqrt(n)      sqrt(n)
    // f(n) = n*(n + 1)/2 - SUM f(n/i) - SUM {n/d - n/(d + 1)}*f(d)  ..... if d = n/d then remove duplicates
    //                      i=2          d=1
    // O(N^(2/3))
    long long sum(int n) {
        if (n <= 0)
            return 0ll;
        else if (n <= 1)
            return 1ll;

        if (n < int(dp.size()) && dp[n])
            return dp[n];

        int sqrtN = int(sqrt(n));

        long long res = 1ll * n * (n + 1) / 2;

        for (int i = 2; i <= sqrtN; i++)
            res -= sum(n / i);

        // remove duplicates
        if (sqrtN == n / sqrtN)
            sqrtN--;

        for (int d = 1; d <= sqrtN; d++)
            res -= (n / d - n / (d + 1)) * sum(d);

        if (n < int(dp.size()))
            dp[n] = res;

        return res;
    }
};
