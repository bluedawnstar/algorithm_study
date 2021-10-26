#pragma once

// https://www.hackerearth.com/practice/basic-programming/bit-manipulation/basics-of-bit-manipulation/practice-problems/algorithm/maximal-and-5477d939/

struct MaxSubsetANDValue {
    // count the number of values that have mask bits in [1, R], O(logN)
    static long long countValuesWithMaskBits(long long R, long long mask) {
        if (R < mask)
            return 0;

        long long bitMask = 1;
        while ((bitMask << 1) <= R) {
            bitMask <<= 1;
        }

        int from = 0;
        int to = 1;

        long long dp[2][2][2];      // [from|to][tight][last bit] = count
        memset(dp[to], 0, sizeof(dp[to]));

        if (mask & bitMask) {
            dp[to][0][0] = 0;
            dp[to][1][1] = 1;
        } else {
            dp[to][0][0] = 1;
            dp[to][1][1] = 1;
        }

        for (bitMask >>= 1; bitMask; bitMask >>= 1) {
            swap(from, to);
            if (mask & bitMask) {
                dp[to][0][0] = 0;
                dp[to][0][1] = dp[from][0][0] + dp[from][0][1];
                dp[to][1][0] = 0;
                if (R & bitMask)
                    dp[to][1][1] = dp[from][1][0] + dp[from][1][1];
                else
                    dp[to][1][1] = 0;
            } else {
                dp[to][0][0] = dp[from][0][0] + dp[from][0][1];
                dp[to][0][1] = dp[from][0][0] + dp[from][0][1];
                if ((R & bitMask) == 0) {
                    dp[to][1][0] = dp[from][1][0] + dp[from][1][1];
                    dp[to][1][1] = 0;
                } else {
                    dp[to][0][0] += dp[from][1][0] + dp[from][1][1];
                    dp[to][1][0] = 0;
                    dp[to][1][1] = dp[from][1][0] + dp[from][1][1];
                }
            }
        }

        return dp[to][0][0] + dp[to][0][1]
             + dp[to][1][0] + dp[to][1][1];
    }

    // the maximum bitwise AND value of a subset of size K in the segment [L, R].
    // O((logN)^2)
    static long long calcMaxSubsetAND(long long L, long long R, long long K) {
        if (K == 1)
            return R;

        long long res = 0;

        long long mask = R;
        while (mask & (mask - 1))
            mask &= mask - 1;

        for (; mask; mask >>= 1) {
            long long cnt = countValuesWithMaskBits(R, res | mask) - countValuesWithMaskBits(L - 1, res | mask);
            if (cnt >= K)
                res |= mask;
        }

        return res;
    }
};
