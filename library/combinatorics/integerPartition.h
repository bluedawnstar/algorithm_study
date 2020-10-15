#pragma once

// see also "../dynamicProgramming/integerPartition.h" (with max group count)

/*
    https://en.wikipedia.org/wiki/Partition_(number_theory)
    https://oeis.org/A000041

    p(n) = the number of partitions of n

    p(5) = 7
      -> 5
      -> 4 + 1
      -> 3 + 2
      -> 3 + 1 + 1
      -> 2 + 2 + 1
      -> 2 + 1 + 1 + 1
      -> 1 + 1 + 1 + 1 + 1
          
    p( 0) = 1
    p( 1) = 1
    P( 2) = 2
    p( 3) = 3
    p( 4) = 5
    p( 5) = 7
    p( 6) = 11
    p( 7) = 15
    p( 8) = 22
    p( 9) = 30
    p(10) = 42
    ...


    p(n) = SUM (-1)^(k-1) * p(n - g(j))
           j!=0

         = p(n - 1) + p(n - 2) - p(n - 5) - p(n - 7) + ...


    g(j) = j-th generalized pentagonal number (j != 0)

         = j*(3*j - 1) / 2            ,  j = 1, -1, 2, -2, 3, -3, ...

         (see "../polynomial/pentagonalNumber.h")
*/
struct IntegerPartition {
    vector<long long> dp;

    // O(N*sqrt(N))
    void build(int maxN) {
        dp.resize(maxN + 1);
        dp[0] = 1;

        for (int i = 1; i <= maxN; i++) {
            long long p = 0;

            for (int j = 1;;) {
                int k = j * (3 * j - 1) / 2;    // k = j*(3*j - 1) / 2,  j = 1, -1, 2, -2, 3, -3, ...
                if (k > i)
                    break;

                if (j & 1)
                    p += dp[i - k];
                else
                    p -= dp[i - k];

                if (j <= 0)
                    j = 1 - j;
                else
                    j = -j;
            }

            dp[i] = p;
        }
    }

    // partition function p(n)
    long long partition(int n) {
        return dp[n];
    }
};
