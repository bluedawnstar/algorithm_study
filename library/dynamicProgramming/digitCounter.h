#pragma once

struct DigitCounter {
    // the number of all digits in range [0, n]
    static vector<long long> countAll(long long n) { 
        vector<long long> res(10, 0);

        if (n >= 0)
            res[0]++;   // include 0

        if (n > 0) {
            for (long long scale = 1; scale <= n; scale *= 10) {
                long long count = (n / scale) / 10;
                for (int j = 0; j < 10; j++)
                    res[j] += count * scale;    // "xxx" + j + "yyy" : 0 <= xxx < n / (scale * 10), 0 <= yyy < scale
                res[0] -= scale;                // subtract the number of leading 0

                // tight(or fit) case
                int d = int((n / scale) % 10);
                for (int j = 0; j < d; j++)
                    res[j] += scale;
                res[d] += (n % scale) + 1;
            }
        }

        return res;
    }

    // the number of all digits in numbers in range [low, high]
    static vector<long long> countAll(long long low, long long high) { 
        auto right = countAll(high);
        auto left = countAll(low - 1);
        
        for (int i = 0; i < 10; i++)
            right[i] -= left[i];

        return right;
    }


    // the number of a digit in range [low, high]
    static long long count(long long low, long long high, int d) {
        auto right = countAll(high);
        auto left = countAll(low - 1);
        return right[d] - left[d];
    }

    // the number of a digit in range [0, n]
    static long long count(long long n, int d) {
        auto res = countAll(n);
        return res[d];
    }
};
