#pragma once

// https://www.hackerearth.com/challenges/competitive/december-circuits-21-2/algorithm/beautiful-numbers-4-20d7065b/
// the count of numbers with 2N digits and the sum of the first N digits is equal to the sum of the last N digits
//   in the interval from L to R, inclusive
struct DigitDP_SamePrefixSuffixDigitSum {
    vector<int> digitSum;                   // digitSum[x] = digit sum of x
    vector<vector<int>> digitSumCount;      // digitSumCount[x][sum] = count of numbers whose sum of digits is 'sum' in [1, x]

    vector<int> totalCount;                 // [the number of digits] = total count
    vector<vector<int>> upperSumCount;      // [the number of half digits] = total upper count
    vector<vector<int>> lowerSumCount;      // [the number of half digits] = total lower count

    vector<int> accumulatedCountByUpper;    // [x=upper value] = count in [size / 10, x]

    explicit DigitDP_SamePrefixSuffixDigitSum(int maxDigitN = 8) {
        prepare(maxDigitN);
    }

    // [1 ~ 10^9], O(sqrt(X))
    void prepare(int maxDigitN = 8) {
        int halfDigitN = maxDigitN / 2;

        int halfSize = 1;
        for (int i = 0; i < maxDigitN / 2; i++)
            halfSize *= 10;

        digitSum = vector<int>(halfSize);
        digitSumCount = vector<vector<int>>(halfSize, vector<int>(9 * halfDigitN + 1));

        totalCount = vector<int>(maxDigitN + 1);
        upperSumCount = vector<vector<int>>(halfDigitN + 1, vector<int>(9 * halfDigitN + 1));
        lowerSumCount = vector<vector<int>>(halfDigitN + 1, vector<int>(9 * halfDigitN + 1));
        accumulatedCountByUpper = vector<int>(halfSize);

        for (int i = 1; i < halfSize; i++) {
            digitSum[i] = sumDigits(i);

            memcpy(digitSumCount[i].data(), digitSumCount[i - 1].data(), digitSumCount[0].size() * sizeof(int));
            digitSumCount[i][digitSum[i]]++;
        }

        int scale = 1;
        for (int i = 1; i <= halfDigitN; i++) {
            updateTable(i);

            scale *= 10;
            for (int j = scale / 10; j < scale; j++) {
                accumulatedCountByUpper[j] = accumulatedCountByUpper[j - 1] + lowerSumCount[i][digitSum[j]];
            }
        }
    }

    // O(log10(X))
    int count(int x) {
        if (x <= 10)
            return 0;

        vector<int> digits = getDigits(x);
        int N = int(digits.size());
        reverse(digits.begin(), digits.end());

        int res = 0;

        if ((N & 1) == 0) {
            int scale = 1;
            for (int i = 0; i < N / 2; i++)
                scale *= 10;

            int upper = x / scale;
            int lower = x % scale;

            // about first half values in [scale / 10, upper - 1]
            res += accumulatedCountByUpper[upper - 1] - accumulatedCountByUpper[scale / 10 - 1];

            // about first half value of x
            int sumL = digitSum[upper];
            res += digitSumCount[lower][sumL];
        }

        for (int i = 2; i < N; i += 2)
            res += totalCount[i];

        return res;
    }

    // O(log10(X))
    int count(int L, int R) {
        return count(R) - count(L - 1);
    }

private:
    static int sumDigits(int x) {
        int sum = 0;
        for (; x; x /= 10)
            sum += x % 10;
        return sum;
    }

    static vector<int> getDigits(int x) {
        vector<int> digit;
        while (x) {
            digit.push_back(x % 10);
            x /= 10;
        }
        return digit;
    }

    void updateTable(int halfDigitN) {
        int maxSum = halfDigitN * 9;

        int maxSize = 1;
        for (int i = 0; i < halfDigitN; i++)
            maxSize *= 10;

        int maxSize10 = maxSize / 10;
        for (int i = 1; i < maxSize; i++) {
            int sum = digitSum[i];

            lowerSumCount[halfDigitN][sum]++;
            if (i >= maxSize10)
                upperSumCount[halfDigitN][sum]++;
        }

        int sum = 0;
        for (int i = 0; i <= maxSum; i++)
            sum += upperSumCount[halfDigitN][i] * lowerSumCount[halfDigitN][i];

        totalCount[halfDigitN * 2] = sum;
    }
};
