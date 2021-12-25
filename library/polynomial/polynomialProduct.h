#pragma once

/*
https://www.hackerearth.com/problem/algorithm/stange-product-sums-e3bf6340/
https://www.hackerearth.com/problem/algorithm/stange-product-sums-e3bf6340/editorial/


    Q(x) = (1 + (b_1 * x) + (b_1 * x)^2 + ... + (b_1 * x)^K)
         * (1 + (b_2 * x) + (b_2 * x)^2 + ... + (b_2 * x)^K)
         * (1 + (b_3 * x) + (b_3 * x)^2 + ... + (b_3 * x)^K)
         * ...
         * (1 + (b_N * x) + (b_N * x)^2 + ... + (b_N * x)^K)
    

  1) Q(x) is ...

            N        1           N
    Q(x) = PROD ------------- = PROD (1 - b_i * x)^-1
           i=1   1 - b_i * x    i=1
    
         = e^(-ln(1 - b_1 * x)) * e^(-ln(1 - b_2 * x)) * ... * e^(-ln(1 - b_N * x))

                 N
         = e^{- SUM ln(1 - b_i * x) }
                i=1

                           <===  ln(1 - x) = - x/1 - x^2/2 - x^3/3 - x^4/4 - ...

                N    (b_i * x)   (b_i * x)^2   (b_i * x)^3   (b_i * x)^4
         = e^{ SUM { --------- + ----------- + ----------- + ----------- + ... } }
               i=1       1            2             3             4 

               inf   x^k    N     
         = e^{ SUM { --- * SUM (b_j)^k } }
               k=1    k    j=1


  2) EGF(exponential generating function)

                N
     Let g_k = SUM (b_j)^k
               j=1

     EGF(exponential generating function) of g_k

             inf         x^k     inf      N              x^k
      G(x) = SUM { g_k * --- } = SUM { { SUM (b_j)^k } * --- }
             k=0         k!      k=0     j=1             k!

              N    inf (b_j * x)^k
           = SUM { SUM ----------- }
             j=1   k=0     k!

              N
           = SUM e^(b_j * x)
             j=1


                 inf   x^k    N
      Q(x) = e^{ SUM { --- * SUM (b_j)^k } }
                 k=1    k    j=1

                 inf     N  (b_j * x)^k
           = e^{ SUM  { SUM ----------- } }
                 k=1    j=1     k

                  N     inf  (b_j * x)^k
           = e^{ SUM  { SUM ----------- } }
                 j=1    k=1     k

           = e^{ [x^1]G(x) * 0! + [x^2]G(x) * 1! + [x^3]G(x) * 2! + ... }

                 inf
           = e^{ SUM { [x^k]G(x) * (k-1)! } * x^k }
                 k=1


  3) b_j = a + (j - 1) * d 
           
              N
      G(x) = SUM e^(b_j * x)
             j=1

           = e^(a*x) * { e^0 + e^(d*x) + e^(2*d*x) + ... + e^((N-1)*d*x) }

                        e^(N*d*x) - 1
           = e^(a*x) * ---------------
                         e^(d*x) - 1

                           <=== e^x = x^0/0! + x^1/1! + x^2/2! + x^3/3! + ...

                        (N*d*x)^1/1! + (N*d*x)^2/2! + (N*d*x)^3/3! + ...
           = e^(a*x) * --------------------------------------------------
                        (d*x)^1/1! + (d*x)^2/2! + (d*x)^3/3! + ...

                        N*d*x * { (N*d*x)^0/1! + (N*d*x)^1/2! + (N*d*x)^2/3! + ... }
           = e^(a*x) * --------------------------------------------------------------
                        d*x * { (d*x)^0/1! + (d*x)^1/2! + (d*x)^2/3! + ... }

                        N * { (N*d*x)^0/1! + (N*d*x)^1/2! + (N*d*x)^2/3! + ... }
           = e^(a*x) * ----------------------------------------------------------
                            { (d*x)^0/1! + (d*x)^1/2! + (d*x)^2/3! + ... }
*/

