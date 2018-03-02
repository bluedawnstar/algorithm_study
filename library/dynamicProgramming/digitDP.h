#pragma once

struct DigitDP {
    static const int N = 20;
    long long (*dp)[180][2];

    DigitDP() {
        dp = new long long[N][180][2];
    }

    ~DigitDP() {
        delete[] dp;
    }

    // returns sum of digits in numbers in range [a, b]
    long long rangeDigitSum(int a, int b) {
        memset(dp, -1, N * 180 * 2 * sizeof(long long));

        vector<int> digitA = getDigits(a - 1);
        long long ans1 = digitSum(digitA, (int)digitA.size() - 1, 0, true);

        vector<int> digitB = getDigits(b);
        long long ans2 = digitSum(digitB, (int)digitB.size() - 1, 0, true);

        return ans2 - ans1;
    }

private:
    vector<int> getDigits(long long x) {
        vector<int> digit;
        while (x) {
            digit.push_back(x % 10);
            x /= 10;
        }
        return digit;
    }

    // return sum of digits from 1 to integer in digit vector
    long long digitSum(const vector<int>& digit, int idx, int sum, bool tight) {
        if (idx == -1)
            return sum;

        if (dp[idx][sum][tight] != -1 && !tight)
            return dp[idx][sum][tight];

        long long ret = 0;

        int k = tight ? digit[idx] : 9;
        for (int i = 0; i <= k; i++) {
            bool newTight = (digit[idx] == i) ? tight : false;
            ret += digitSum(digit, idx - 1, sum + i, newTight);
        }

        if (!tight)
            dp[idx][sum][tight] = ret;

        return ret;
    }
};
