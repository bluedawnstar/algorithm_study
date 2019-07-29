#pragma once

// original pirate game
// https://en.wikipedia.org/wiki/Pirate_game
/*
             N
      ---------------------------------------
             1: M
             2: M
             3: M - 1
             4: M - 1
             5: M - 2
             6: M - 2
             7: M - 3
             8: M - 3
             ...
        base-2: 1
        base-1: 1
          base: 0         (base = M * 2 + 1)
        base+1: 0
        base+2: x
        base+3: 0
        base+4: x
        base+5: x
        base+6: x
        base+7: 0
        ...
*/
struct PirateGame {
    // N = the number of pirates, M = the number of gold pieces
    // O(1)
    static long long solve(long long N, long long M) {
        long long base = M * 2 + 1;

        if (N <= base)
            return M - (N - 1) / 2;

        long long index = N - base;
        if ((index & (index + 1)) == 0)
            return 0ll;
        else
            return -1ll;
    }
};
