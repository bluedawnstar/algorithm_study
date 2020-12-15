#pragma once

/*
Binomial Coefficients

https://en.wikipedia.org/wiki/Binomial_coefficient
https://cp-algorithms.com/combinatorics/binomial-coefficients.html


1. Basic formula

                       n!
    C(n, k) = ---------------
                  k! * (n - k)!

            = C(n - 1, k) + C(n - 1, k - 1)

            = n / k * C(n - 1, k - 1)

            = (n + 1 - k) / k * C(n, k - 1)

   1) symmetry
       C(n, k) = C(n, n - k)

   2) factoring
       C(n, k) = n / k * C(n - 1, k - 1)

   3) sum over k
        n
       SUM C(n, k) = 2^n
       k=0

   4) sum over n
        n
       SUM C(m, k) = C(n + 1, k + 1)
       m=0
     
   5) sum over n and k
        m
       SUM C(n + k, k) = C(n + m + 1, m)
       k=0

   6) Sum of the squares
        n
       SUM C(n, k)^2 = C(2n, n)
       k=0

   7) ...
        n
       SUM C(n, k) * C(k, q)
       k=q

          n
       = SUM C(n, q) * C(n - q, k - q)
         k=q

       = 2^(n - q) * C(n, q)

       <proof>
          n
         SUM C(n, k) * C(k, q)
         k=q
            n
         = SUM n! / { (n - k)! * k! } * k! / { q! * (k - q)! }
           k=q
            n
         = SUM n! / { (n - k)! } * 1 / { q! * (k - q)! }
           k=q
            n
         = SUM n! / { q! } * 1 / { (n - k)! * (k - q)! }
           k=q
            n
         = SUM n! / { (n - q)! * q! } * (n - q)! / { (n - k)! * (k - q)! }
           k=q
            n
         = SUM n! / { (n - q)! * q! } * (n - q)! / { (n - k)! * (k - q)! }
           k=q
            n
         = SUM C(n, q) * C(n - q, k - q)
           k=q
                      n
         = C(n, q) * SUM C(n - q, k - q)
                     k=q
          = C(n, q) * 2^(n - q)

   8) Weighted sum
        n
       SUM k * C(n, k) = n * 2^(n - 1)
       k=0

        n
       SUM k^2 * C(n, k) = (n + n^2) * 2^(n - 2)
       k=0

   9) Chu-Vandermonde identity
        k
       SUM C(m, j) * C(n - m, k - j) = C(n, k) 
       j=0

   10)
        n
       SUM (-1)^j * C(n, j) = 0
       j=0

   11) Connection with the Fibonacci numbers
        n
       SUM C(n - j, j) = F(n + 1)
       j=0


2. Generating function

   (x + y)^n = C(n,0) * x^n * y^0
             + C(n,1) * x^(n-1) * y^1
             + C(n,2) * x^(n-2) * y^2
             + ...
             + C(n, n) * x^0 * y^n

*/


// Naive calculation
// PRECONDITION: n <= 61
inline long long comb(int n, int r) {
    if (n < r)
        return 0;
    if (n == 0 || r == 0 || n == r)
        return 1;

    if (n - r < r)
        r = n - r;

    long long q = 1;
    for (int i = 1; i <= r; i++)
        q = q * n-- / i;

    return q;
}
