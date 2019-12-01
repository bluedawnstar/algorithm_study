#pragma once

/*
Pillai's arithmetical function (OEIS A018804)

            1    2    3    4    5    6    7    8    9   10
        ----------------------------------------------------
    P = {   1,   3,   5,   8,   9,  15,  13,  20,  21,  27,
           21,  40,  25,  39,  45,  48,  33,  63,  37,  72,
           65,  63,  45, 100,  65,  75,  81, 104,  57, 135,
           61, 112, 105,  99, 117, 168,  73, 111, 125, 180,
           81, 195,  85, 168, 189, 135,  93, 240, 133, 195,
          165, 200, 105, 243, 189, 260, 185, 171, 117, 360 }

1)
            n
    P(n) = SUM gcd(i, n)
           i=1

                 n
         = SUM  SUM d
           d|n  i=1
              gcd(i,n)=d

         = SUM d * phi(n / d)           , d is a divisor of n
           d|n

         = n * SUM phi(d) / d
               d|n

         = SUM d * D(d) * mu(n / d)     , D(n) is the number of divisors
           d|n

2) https://forthright48.com/gcd-sum-function/

    n = p1^a1 * p2^a2 * ... * pk^ak

    P(n) = { (a_1 + 1) * p_1^a_1 - a_1 * p_1^(a_1 - 1) }
         * { (a_2 + 1) * p_2^a_2 - a_2 * p_2^(a_2 - 1) }
         * ...
         * { (a_k + 1) * p_k^a_k - a_k * p_k^(a_k - 1) }

*/
