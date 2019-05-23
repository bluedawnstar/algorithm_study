#pragma once

/*
  https://en.wikipedia.org/wiki/Multinomial_theorem
  = the generalization of the binomial theorem

  1. Theorem
    
      (x1 + x2 + .. + xm)^n =  SUM C(n, {k1,k2,...,km})*(x1^k1 * x2^k2 * ... * xm^km)
                          k1+k2+...+km=n

      where 

      C(n, {k1,k2,...,km}) = n! / (k1! * k2! * ... * km!) , ki >= 0


    1.1 binomial case
                   n
      (x + y)^n = SUM n! / (k! * (n - k)!) * x^k * y^(n - k)
                  k=0

  2. Multinomial coefficients

    2.1 C(n, {k1,k2,...,km}) = n! / (k1! * k2! * ... * km!)
                             = C(k1, k1) * C(k1+k2, k2) * C(k1+k2+k3, k3) * ... * C(k1+k2+...+km, km)

    2.2 Number of multinomial coefficients
           SUM C(n, {k1,k2,...,km}) = m^n
       k1+k2+...+km=n

  3. Number of multinomial coefficients

    #(n,m) = C(n + m - 1, m - 1)
*/
/*
  <Multinomial distribution>
  https://en.wikipedia.org/wiki/Multinomial_distribution

  1. Probability mass function
    Pr(X1 = x1 and ... and Xk = xk)
    =  n1 / (x1!*x2!*...*xk!) * p1^x1 * p2^x2 * ... * pk^xk,   when x1+x2+...+xn = n
       0                                                   ,   otherwise
*/