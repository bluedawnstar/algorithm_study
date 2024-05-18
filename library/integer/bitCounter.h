#pragma once

class BitCounter {
    // the number of integer X with the nth bit
    // - 0 < X <= maxX, 0 <= nthBit
    static long long countNumbersWithBit(long long maxX, int nthBit) {
        long long scale = 1ll << nthBit;

        long long res = (maxX >> (nthBit + 1)) * scale;
        if (maxX & (1ll << nthBit))
            res += (maxX & (scale - 1)) + 1;    // tight case

        return res;
    }
};
