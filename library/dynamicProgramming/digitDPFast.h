#pragma once

struct DigitDPFast {
    // returns sum of digits in numbers in range [a, b]
    static long long rangeDigitSum(long long a, long long b) {
        return digitSum(b) - digitSum(a - 1);
    }

    // return sum of digits from 1 to integer in digit vector
    static long long digitSum(long long x) {
        if (x <= 0)
            return 0;
        else if (x < 10)
            return x * (x + 1) / 2;

        vector<int> digits = getDigits(x);
        int N = int(digits.size());
        reverse(digits.begin(), digits.end());

        int from = 0;
        int to = 1;

        long long dpSum[2][2][10];          // [from|to][tight][last digit] = count
        long long dpCnt[2][2][10];          // [from|to][tight][last digit] = count
        memset(dpSum[to], 0, sizeof(dpSum[to]));
        memset(dpCnt[to], 0, sizeof(dpCnt[to]));

        for (int j = 1; j <= digits[0]; j++) {
            if (j < digits[0]) {
                dpSum[to][0][j] = j;
                dpCnt[to][0][j]++;
            } else {
                dpSum[to][1][j] = j;
                dpCnt[to][1][j]++;
            }
        }

        for (int i = 1; i < N; i++) {
            swap(from, to);
            memset(dpSum[to], 0, sizeof(dpSum[to]));
            memset(dpCnt[to], 0, sizeof(dpCnt[to]));

            for (int j = 0; j < 10; j++) {
                if (j > 0) {    // start here
                    dpSum[to][0][j] += j;
                    dpCnt[to][0][j]++;
                }
                for (int k = 0; k < 10; k++) {
                    if (!dpSum[from][0][k])
                        continue;
                    dpSum[to][0][j] += dpSum[from][0][k] + j * dpCnt[from][0][k];
                    dpCnt[to][0][j] += dpCnt[from][0][k];
                }
            }

            for (int j = 0; j < digits[i]; j++) {
                dpSum[to][0][j] += dpSum[from][1][digits[i - 1]] + j * dpCnt[from][1][digits[i - 1]];
                dpCnt[to][0][j] += dpCnt[from][1][digits[i - 1]];
            }
            dpSum[to][1][digits[i]] += dpSum[from][1][digits[i - 1]] + digits[i] * dpCnt[from][1][digits[i - 1]];
            dpCnt[to][1][digits[i]] += dpCnt[from][1][digits[i - 1]];
        }

        long long res = 0;
        for (int i = 0; i < 10; i++) {
            res += dpSum[to][0][i];
            res += dpSum[to][1][i];
        }

        return res;
    }

    //---

    // returns the number of a digit in numbers in range [a, b]
    static long long rangeDigitCount(long long a, long long b, int d) {
        return digitCount(b, d) - digitCount(a - 1, d);
    }

    // return the number of a digit from 0 to integer in digit vector
    static long long digitCount(long long x, int d) {
        if (x < 0)
            return 0;
        else if (x < 10)
            return (d <= x);

        vector<int> digits = getDigits(x);
        int N = int(digits.size());
        reverse(digits.begin(), digits.end());

        int from = 0;
        int to = 1;

        long long dpSum[2][2][10];          // [from|to][tight][last digit] = count
        long long dpCnt[2][2][10];          // [from|to][tight][last digit] = count
        memset(dpSum[to], 0, sizeof(dpSum[to]));
        memset(dpCnt[to], 0, sizeof(dpCnt[to]));

        for (int j = 1; j <= digits[0]; j++) {
            if (j < digits[0])
                dpCnt[to][0][j]++;
            else
                dpCnt[to][1][j]++;
        }
        if (0 < d && d <= digits[0])    // first digit
            dpSum[to][d == digits[0]][d]++;

        for (int i = 1; i < N; i++) {
            swap(from, to);
            memset(dpSum[to], 0, sizeof(dpSum[to]));
            memset(dpCnt[to], 0, sizeof(dpCnt[to]));

            if (d > 0)  // start here
                dpSum[to][0][d]++;

            for (int j = 0; j < 10; j++) {
                if (j > 0)  // start here
                    dpCnt[to][0][j]++;
                for (int k = 0; k < 10; k++) {
                    dpSum[to][0][j] += dpSum[from][0][k] + (j == d ? dpCnt[from][0][k] : 0);
                    dpCnt[to][0][j] += dpCnt[from][0][k];
                }
            }

            for (int j = 0; j < digits[i]; j++) {
                dpSum[to][0][j] += dpSum[from][1][digits[i - 1]] + (j == d ? dpCnt[from][1][digits[i - 1]]: 0);
                dpCnt[to][0][j] += dpCnt[from][1][digits[i - 1]];
            }
            dpSum[to][1][digits[i]] += dpSum[from][1][digits[i - 1]] + (digits[i] == d ? dpCnt[from][1][digits[i - 1]] : 0);
            dpCnt[to][1][digits[i]] += dpCnt[from][1][digits[i - 1]];
        }

        long long res = 0;
        for (int i = 0; i < 10; i++) {
            res += dpSum[to][0][i];
            res += dpSum[to][1][i];
        }

        return res + (d == 0);  // include 0 when d is 0
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
