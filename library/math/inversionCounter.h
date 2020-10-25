#pragma once

/*
  1. query
      - F(N,K) % 2, K <= N
      - time complexity  : O(sqrt(K))
      - space complexity : O(1)

  2. solution
    F(N,K) = [x^K] x^0 * (x^0 + x^1) * ... * (x^0 + x^1 + ... + x^(N - 1))

                    N-1
           = [x^K] PROD (x^0 + x^1 + ... + x^i)
                   i=0

                    N-1
           = [x^K] PROD (x^(i + 1) - 1) / (x - 1)
                   i=0

                    N
           = [x^K] PROD (x^i - 1) / (x - 1)
                   i=1

                      N
           = [x^K] { PROD (x^i - 1) } / (x - 1)^N
                     i=1

                      N
           = [x^K] { PROD (x^i - 1) } * (x - 1)^-N
                     i=1

                      N
           = [x^K] { PROD (x^i - 1) } * { C(N-1, 0) * x^0 + C(N, 1) * x^1 + C(N+1, 2) * x^2 + ... }  <- https://codeforces.com/blog/zscoder
                     i=1

                      N                   inf
           = [x^K] { PROD (x^i - 1) } * { SUM C(N - 1 + i, i) * x^i }
                     i=1                  i=0


           =     SUM { (-1)^abs(j) * x^{j*(3*j-1)/2} } * { C(N - 1 + K - j*(3*j-1)/2, K - j*(3*j-1)/2) * x^{K - j*(3*j-1)/2} }
             j=0,1,-1,2,-2,...
             j*(3*j-1)/2 <= K


  3. properties
    3.1) https://codeforces.com/blog/zscoder

           PROD (x - 1)^-N  =  C(N-1, 0) * x^0 + C(N, 1) * x^1 + C(N+1, 2) * x^2 + ...
           i=1

    3.2) Pentagonal number theorem
         // https://en.wikipedia.org/wiki/Pentagonal_number_theorem

         inf
         PROD (1 - x^i) = 1 - x - x^2 + x^5 + x^7 - x^12 - x^15 + x^22 + x^26 - ...
         i=1
                              inf
                        = 1 + SUM (-1)^k ( x^k(3k+1)/2 + x^k(3k-1)/2 )
                              k=1

                        = SUM (-1)^abs(j) * x^{j*(3*j-1)/2}
                        j=0,1,-1,2,-2,...

    3.3) C(n,r) % 2
         - if (r & (n - r)) != 0, then 0
         = if (r & (n - r)) == 0, then 1

  4. related problems and documents
      https://en.wikipedia.org/wiki/Permutation#Inversions
      https://en.wikipedia.org/wiki/Q-Pochhammer_symbol#Relationship_to_other_q-functions

      https://codeforces.com/blog/entry/80480
      https://www.hackerrank.com/contests/101hack43/challenges/k-inversion-permutations/problem

      https://www.codechef.com/problems/INVSMOD2
      https://discuss.codechef.com/t/invsmod2-editorial/79646
*/
struct InversionCounter {
    // the number of permutations of { 1, 2, ..., N } with exactly K inversions
    // O(sqrt(N)), K <= N
    static int countPermutationWithExactInversionsMod2(long long N, long long K) {
        if (K == 0)
            return 1;
        else if ((N == 1 && K == 1) || (N == 2 && K == 2))
            return 0;
        else if (K == 1)
            return ((N & 1) ? 0 : 1);

        int res = 0;
        for (long long j = 0; ;) {
            // kk = j*(3*j - 1) / 2,  j = 0, 1, -1, 2, -2, 3, -3, ...
            long long kk = j * (3 * j - 1) / 2;
            if (kk > K)
                break;

            res ^= combMod2(N - 1 + K - kk, K - kk);
            if (j <= 0)
                j = 1 - j;
            else
                j = -j;
        }

        return res;
    }

    // C(n,r) % 2
    static int combMod2(long long n, long long r) {
        if (r & (n - r))
            return 0;
        else
            return 1;
    }
};
