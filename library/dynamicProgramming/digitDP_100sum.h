#pragma once

// https://www.hackerearth.com/problem/algorithm/find-the-number-3-a38198db/

// '100sum' is a number that adds up to 100 for each digit.
struct DigitDP_100Sum {
    static long long count(long long x) {
        if (x < 199999999999ll)
            return 0;

        vector<int> digits = getDigits(x);
        int N = int(digits.size());
        reverse(digits.begin(), digits.end());

        int from = 0;
        int to = 1;

        long long dpCnt[2][2][101];    // [from|to][tight][digit sum] = count
        memset(dpCnt[to], 0, sizeof(dpCnt[to]));

        for (int j = 0; j <= digits[0]; j++) {
            if (j < digits[0])
                dpCnt[to][0][j]++;
            else
                dpCnt[to][1][j]++;
        }

        for (int i = 1; i < N; i++) {
            swap(from, to);
            memset(dpCnt[to], 0, sizeof(dpCnt[to]));

            for (int j = 0; j < 10; j++) {
                for (int k = 100 - j; k >= 0; k--)
                    dpCnt[to][0][k + j] += dpCnt[from][0][k];
            }
            for (int j = 0; j < digits[i]; j++) {
                for (int k = 100 - j; k >= 0; k--)
                    dpCnt[to][0][k + j] += dpCnt[from][1][k];
            }

            for (int k = 100 - digits[i]; k >= 0; k--)
                dpCnt[to][1][k + digits[i]] += dpCnt[from][1][k];
        }

        return dpCnt[to][0][100] + dpCnt[to][1][100];
    }

    static long long kth(long long k) {
        long long hi = numeric_limits<long long>::max();
        long long lo = 199999999999ll;

        while (lo <= hi) {
            long long mid = lo + ((hi - lo) >> 1);
            if (count(mid) >= k)
                hi = mid - 1;
            else
                lo = mid + 1;
        }

        return lo;
    }

private:
    static vector<int> getDigits(long long x) {
        vector<int> digit;
        while (x) {
            digit.push_back(x % 10);
            x /= 10;
        }
        return digit;
    }
};
