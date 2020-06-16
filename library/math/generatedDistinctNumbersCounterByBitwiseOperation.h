#pragma once

// https://www.hackerearth.com/problem/algorithm/or-b3c038d1/description/

// How many distinct numbers can be generated by doing bitwise operation of one or more integers between A and B (inclusive)?
struct GeneratedDistinctNumbersCounterByBitwiseOperation {
    // A <= B, inclusive
    static long long countWithSubsetBitwiseOr(long long A, long long B) {
        if (A == B)
            return 1ll;

        int bitWidth = 0;
        while ((A >> bitWidth) != (B >> bitWidth))
            bitWidth++;

        //   A = 0xx...xx
        //   B = 1yy...yy
        // msb = 100...00
        A &= (1ll << bitWidth) - 1;
        B &= (1ll << bitWidth) - 1;
        long long msb = 1ll << (bitWidth - 1);

        // part1: A ~ 011...11
        long long res = msb - A;

        // part2: 100...00 ~ max_value_to_make_with_bits_of(B)
        //     + (100...00 | A) ~ 111...11
        res += min((1ll << countBitWidth(B - msb)) + msb - A, msb);

        return res;
    }

private:
    static int countBitWidth(long long x) {
        int res = 0;
        while ((1ll << res) <= x)
            res++;
        return res;
    };
};