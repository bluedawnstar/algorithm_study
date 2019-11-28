#pragma once

/*
https://en.wikipedia.org/wiki/Catalan_number

                   1                     comb(0,0) = 1
                1     1
             1     2     1               comb(2,1) = 2
          1     3     3     1
       1     4     6     4     1         comb(4,2) = 6
    1     5    10    10     5     1 
 1     6    15    20    15     6     1   comb(6,3) = 20
                 ....

     n                 C(n)
 -----------------------------
     0                    1
     1                    1
     2                    2
     3                    5
     4                   14
     5                   42
     6                  132
     7                  429
     8                 1430
     9                 4862
    10                16796
    11                58786
    12               208012
    13               742900
    14              2674440
    15              9694845
    16             35357670
    17            129644790
    18            477638700
    19           1767263190
    20           6564120420
    21          24466267020
    22          91482563640
    23         343059613650
    24        1289904147324
    25        4861946401452
    26       18367353072152
    27       69533550916004
    28      263747951750360
    29     1002242216651368
    30     3814986502092304
    31    14544636039226909
    32    55534064877048198
    33   212336130412243110
    34   812944042149730764
    35  3116285494907301262
    36 11959798385860453492
    37 45950804324621742364
    ¡¦

1. Basic formula
   1)
             comb(2n, n)
      C(n) = -----------                   , n >= 0
               (n + 1)

                 (2n)!
           = -------------
             (n + 1)! * n!

             (n + n) * (n + n - 1) * (n + n - 2) * ... * (n + 2)
           = ---------------------------------------------------
                n    *   (n - 1)   *   (n - 2)   * ... *    2

      C(n) = comb(2n, n) - comb(2n, n + 1)
           = comb(2n, n) - comb(2n, n - 1)
             comb(2n, n)
           = -----------                   , n >= 0
               (n + 1)

              2(2n - 1)
      C(n) = ----------- C(n - 1)          , C(0) = 1
                n + 1

   2) 
      C(0) = 1

             n-1
      C(n) = SUM C(i) * C(n - 1 - i)       , n >= 1
             i=0

2. Generating function



3. Applications
   1) the number of Dyck words of length 2*n
        XXXYYY   |   XYXXYY   |   XYXYXY   |   XXYYXY   |   XXYXYY

   2) the number of expressions containing n pairs of parentheses which are correctly matched
        ((()))   |   ()(())   |   ()()()   |   (())()   |   (()())
   
   3) the number of different ways n + 1 factors can be completely parenthesized
       ((ab)c)d  |  (a(bc))d  |  (ab)(cd)  |  a((bc)d)  |  a(b(cd))

   4) the number of full binary trees with n + 1 leaves
           *          *           *           *          *
          / \        / \        /   \        / \        / \
         *   x      *   x      *     *      x   *      x   *
        / \        / \        / \   / \        / \        / \
       *   x      x   *      x   x x   x      *   x      x   *
      / \            / \                     / \            / \
     x   x          x   x                   x   x          x   x

   5) the number of non-isomorphic ordered trees with n + 1 vertices

   6) the number of monotonic lattice paths along the edges of a grid with n ¡¿ n square cells
       +******     +****-+     +****-+     +**-+-+     +**-+-+
       |\| | *     |\| * |     |\| * |     |\* | |     |\* | |
       +-+-+-*     +-+-***     +-+-*-+     +-*****     +-***-+
       | |\| *     | |\| *     | |\* |     | |\| *     | |\* |
       +-+-+-*     +-+-+-*     +-+-***     +-+-+-*     +-+-***
       | | |\*     | | |\*     | | |\*     | | |\*     | | |\*
       +-+-+-+     +-+-+-+     +-+-+-+     +-+-+-+     +-+-+-+

   7) The number of triangulations of a convex polygon with n + 2 sides

   8) The number of ways to connect the 2n points on a circle to form n disjoint chords.

   9) Number of permutations of length n that can be stack sorted
      (ex: it can be shown that the rearrangement is stack sorted
           if and only if there is no such index i < j < k, such that a[k] < a[i] < a[j])

   10) the number of permutations with no n-term increasing subsequence
       132  |  213  |  231  |  312  |  321

   11) the number of ways to tile a stairstep shape of height n with n rectangles
       111     111     123     122     112
       22      23      12      13      11
       3       2       1       1       3

   12) the number of ways to form a "mountain range" with n upstrokes and n downstrokes
       that all stay above a horizontal line
                                             /\
                   /\     /\       /\/\     /  \
       /\/\/\ , /\/  \ , /  \/\ , /    \ , /    \

   12) the number of ways that the vertices of a convex 2n-gon can be paired
       so that the line segments joining paired vertices do not intersect

*/

template <typename T>
struct Catalan {
    vector<T> C;

    // O(N^2)
    void build(int N) {
        C.resize(N + 1);

        C[0] = C[1] = 1;
        for (int i = 2; i <= N; i++) {
            C[i] = 0;
            for (int j = 0; j < i; j++)
                C[i] += C[j] * C[i - j - 1];
        }
    }

    T operator [](int n) const {
        return C[n];
    }
};

template <typename T, T mod = 1000000007>
struct CatalanMod {
    vector<T> C;

    // O(N^2)
    void build(int N) {
        C.resize(N + 1);

        C[0] = C[1] = 1;
        for (int i = 2; i <= N; i++) {
            long long catalan = 0;
            for (int j = 0; j < i; j++)
                catalan += 1ll * C[j] * C[i - j - 1] % mod;
            C[i] = T(catalan % mod);
        }
    }

    // O(N)
    void buildFast(int N) {
        C.resize(N + 1);

        vector<T> inv(N + 2);
        inv[0] = 0;
        inv[1] = 1;
        inv[2] = T((mod - 1ll * (mod / 2) * inv[mod % 2] % mod) % mod);

        C[0] = C[1] = 1;
        for (int i = 2; i <= N; i++) {
            inv[i + 1] = int((mod - 1ll * (mod / (i + 1)) * inv[mod % (i + 1)] % mod) % mod);
            C[i] = T(2ll * (2ll * i - 1) * inv[i + 1] % mod * C[i - 1] % mod);
        }
    }

    T operator [](int n) const {
        return T(C[n]);
    }
};
