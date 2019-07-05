#pragma once

/*

  1. Combination
    1)
         (1 + x + x^2 + x^3 + ...)(1 + y + y^2 + y^3 + ...)(1 + z + z^2 + z^3 + ...)

       = 1 + SUM{x^p * y^q * z^r} + SUM{x^p * y^q * z^r} + SUM{x^p * y^q * z^r} + ...
            p+q+r=1                p+q+r=2                p+q+r=3
            0<=p,q,r               0<=p,q,r               0<=p,q,r

    2)
         (1 + x + x^2 + x^3 + ...)^n

       = H(n,0) + H(n,1)*x + H(n,2)*x^2 + H(n,3)*x^3 + ...


  2. Partition
    1) p(n,r) : the number of partitions of n as the sum of r or less

       s1 + 2*s2 + 3*s3 + ... + r*sr = n (0 <= s1, s2, s3, ..., sr)


         (1 + x + x^2 + ...)(1 + x^2 + x^4 + ...)...(1 + x^r + x^2r + ...)

       = p(0,r) + p(1,r)*x + p(2,r)*x^2 + p(3,r)*x^3 + ... + p(n,r)*x^n + ...

    2) Stirling numbers of the 1st kind

       s(n, k) = s(n - 1, k - 1) + (n - 1) * s(n - 1, k)

                                    n
       x*(x + 1)*...*(x + n - 1) = SUM s(n,k)*x^k
                                   k=0

  3. Exponential
    1) e^x = x^0/0! + x^1/1! + x^2/2! + x^3/3! + ...

       e^x - 1 = x^1/1! + x^2/2! + x^3/3! + ...

       (e^x + e^-x)/2 = x^0/0! + x^2/2! + x^4/4! + ...

    2) (1 + x)^n = C(n,0)*x^0 + C(n,1)*x^1 + C(n,2)*x^2 + ... + C(n,n)*x^n

                 = P(n,0)*x^0/0! + P(n,1)*x^1/1! + P(n,2)*x^2/2! + ... + P(n,n)*x^n/n!

    3) permutation
       when p + q + r = n,

     3.1)
         (x^0/0! + x^1/1! + x^2/2! + ...)^3 = (e^x)^3 = e^3x

       = 1 + SUM{ 1/p!q!r! * x^p * x^q * x^r } + SUM{ 1/p!q!r! * x^p * x^q * x^r } + ...
            p+q+r=1                             p+q+r=2

       = 1 + SUM{ 1/p!q!r! } * x^1 + SUM{ 1/p!q!r! } * x^2 + ...
            p+q+r=1                 p+q+r=2

       = 1 + SUM{ 1!/p!q!r! } * x^1/1! + SUM{ 2!/p!q!r! } * x^2/2! + ...
            p+q+r=1                     p+q+r=2

             inf
       = 1 + SUM{ SUM{ n!/p!q!r! } * x^n/n! }
             n=1  p+q+r=n

     3.2)
         (1^k * x^0/0! + 2^k * x^1/1! + 3^k * x^2/2! + ...)^3

             inf
       = 1 + SUM{ SUM{ p^k * q^k * r^k } * x^n/n! }
             n=1  p+q+r=n


    4) Stirling numbers of the 2nd kind

       S(n, k) = S(n - 1, k - 1) + k * S(n - 1, k)

                     inf
       (e^x - 1)^k = SUM T(n,k) * x^n/n!   , T(n,k) = k! * S(n,k)
                     n=0

*/