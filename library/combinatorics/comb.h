#pragma once

/*
Binomial Coefficients

https://en.wikipedia.org/wiki/Binomial_coefficient
https://cp-algorithms.com/combinatorics/binomial-coefficients.html


1. Basic formula

                       n!
    comb(n, k) = ---------------
                  k! * (n - k)!

               = comb(n - 1, k) + comb(n - 1, k - 1)

               = n / k * comb(n - 1, k - 1)

               = (n + 1 - k) / k * comb(n, k - 1)

   1) symmetry
       comb(n, k) = comb(n, n - k)

   2) factoring
       comb(n, k) = n / k * comb(n - 1, k - 1)

   3) sum over k
        n
       SUM comb(n, k) = 2^n
       k=0

   4) sum over n
        n
       SUM comb(m, k) = comb(n + 1, k + 1)
       m=0
     
   5) sum over n and k
        m
       SUM comb(n + k, k) = comb(n + m + 1, m)
       k=0

   6) Sum of the squares
        n
       SUM comb(n, k)^2 = comb(2n, n)
       k=0

   7) Weighted sum
        n
       SUM k * comb(n, k) = n * 2^(n - 1)
       k=0

        n
       SUM k^2 * comb(n, k) = (n + n^2) * 2^(n - 2)
       k=0

   8) Chu-Vandermonde identity
        k
       SUM comb(m, j) * comb(n - m, k - j) = comb(n, k) 
       j=0

   9)
        n
       SUM (-1)^j * comb(n, j) = 0
       j=0

   10) Connection with the Fibonacci numbers
        n
       SUM comb(n - j, j) = F(n + 1)
       j=0


2. Generating function

   (x + y)^n = comb(n,0) * x^n * y^0 + comb(n,1) * x^(n-1) * y^1
             + comb(n,2) * x^(n-2) * y^2 + ... + comb(n, n) * x^0 * y^n

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
