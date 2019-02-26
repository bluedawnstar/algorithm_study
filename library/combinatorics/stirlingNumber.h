#pragma once

/*

1. Stirling numbers of the 1st kind

    s(n, k) = s(n - 1, k - 1) + (n - 1) * s(n - 1, k)

    n\k | 0     1      2      3     4     5    6    7    8    9
    ----+----------------------------------------------------------
      0 | 1
      1 | 0     1
      2 | 0     1      1
      3 | 0     2      3      1
      4 | 0     6     11      6     1
      5 | 0    24     50     35    10     1
      6 | 0   120    274    225    85    15    1
      7 | 0   720   1764   1624   735   175   21    1
      8 | 0  5040  13068  13132  6769  1960  322   28    1
      9 | 0 40320 109584 118124 67284 22449 4536  546   36    1

    1) unsigned Stirling numbers

       s(n, k) = s(n - 1, k - 1) + (n - 1) * s(n - 1, k)

                                    n
       x*(x + 1)*...*(x + n - 1) = SUM s(n,k)*x^k
                                   k=0

    2) signed Stirling numbers

       s(n, k) = s(n - 1, k - 1) - (n - 1) * s(n - 1, k)

                                    n
       x*(x - 1)*...*(x - n + 1) = SUM s(n,k)*x^k
                                   k=0

    3) s(n, 1) = (n - 1)!
       s(n, n - 1) = C(n, 2)

         n
        SUM s(n,k) = n!
        k=0
      

2. Stirling numbers of the 2nd kind

    S(n, k) = S(n - 1, k - 1) + k * S(n - 1, k),

    n\k | 0   1    2     3      4      5      6     7    8    9    10
    ----+--------------------------------------------------------------
      0 | 1
      1 | 0   1
      2 | 0   1    1
      3 | 0   1    3     1
      4 | 0   1    7     6      1
      5 | 0   1   15    25     10      1
      6 | 0   1   31    90     65     15      1
      7 | 0   1   63   301    350    140     21     1
      8 | 0   1  127   966   1701   1050    266    28    1
      9 | 0   1  255  3025   7770   6951   2646   462   36    1
     10 | 0   1  511  9330  34105  42525  22827  5880  750   45    1

    1)
            n
           SUM S(n,k) * x*(x - 1)*...*(x - k + 1) = x^n
           k=0
            n
      <=>  SUM S(n,k) * C(n,k)*k! = x^n
           k=0

    2) 
                            k
           S(n,k) = 1/k! * SUM (-1)^i * C(k, i) * (k - i)^n
                           i=0
                            k
                  = 1/k! * SUM (-1)^(k+i) * C(k, i) * i^n
                           i=0

    3) S(n, 2) = 2^(n-1) - 1
       S(n, n - 1) = C(n, 2)


3. Special properties

   1) Inversion relations and the Stirling transform
      
      F = { f(1), f(2), f(3), ..., f(n) }
      G = { g(1), g(2), g(3), ..., g(n) }

                 n
         g(n) = SUM S(n,k) * f(k)
                k=1
                 n
      => f(n) = SUM s(n,k) * (-1)^(n-k) * g(k)
                k=1


4. Algorithms

   1) finding F when G is given
              n
      g(n) = SUM S(n,k) * f(k)
             k=1

      1.1) calculate unsigned Stirling number of 1st kind

                                      n
         x*(x + 1)*...*(x + n - 1) = SUM s(n,k)*x^k
                                     k=0

         => s(n,k) is the coefficient of x^k
         => calculate the rising factorial with FFT

      1.2) calculate f(n) to use Inversion relations and the Stirling transform
                n
        f(n) = SUM s(n,k) * (-1)^(n-k) * g(k)
               k=1

*/
