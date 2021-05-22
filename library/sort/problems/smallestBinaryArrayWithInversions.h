#pragma once

// https://www.hackerearth.com/problem/algorithm/binary-inversions-cf645357/

// lexicographically smallest binary array such that the number of inversions in the array is equal to 'inversionCount'
struct SmallestBinaryArrayWithInversions {
    // - zeroN = the number of 0
    // - oneN = the number of 1
    // - inversionCount = the number of inversions
    // O(N)
    static vector<int> solve(int zeroN, int oneN, long long inversionCount) {
        vector<int> res;
        if (1ll * zeroN * oneN < inversionCount)
            return res; // none

        res.reserve(zeroN + oneN);

        /*
           00...0011...11011...1100...00
           |<--->||<--->| |<--->||<--->|
              A      B       C      D

              |A| = zeroN - (inversionCount / oneN) - 1
              |B| = inversionCount % oneN
              |C| = oneN - (inversionCount % oneN)
              |D| = (inversionCount / oneN)
        */
        int k = int(inversionCount / oneN);
        for (int i = 0; i < zeroN - k - 1; i++)
            res.push_back(0);
        for (int i = 0; i < inversionCount % oneN; i++)
            res.push_back(1);
        res.push_back(0);
        for (int i = 0; i < oneN - (inversionCount % oneN); i++)
            res.push_back(1);
        for (int i = 0; i < k; i++)
            res.push_back(0);

        return res;
    }
};
