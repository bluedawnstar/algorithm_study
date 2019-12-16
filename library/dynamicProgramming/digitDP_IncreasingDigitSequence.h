#pragma once

struct IncreasingDigitSequence {
    // including '0';
    static vector<vector<long long>> digitCount(long long x) {
        vector<int> digits = getDigits(x);
        reverse(digits.begin(), digits.end());
        return digitCount(digits);
    }

    // range [L, R]
    static vector<vector<long long>> digitCount(long long L, long long R) {
        vector<int> digitR = getDigits(R);
        reverse(digitR.begin(), digitR.end());
        auto dpR = digitCount(digitR);

        if (L <= 0)
            return dpR;

        vector<int> digitL = getDigits(L - 1);
        digitL.resize(digitR.size(), 0);
        reverse(digitL.begin(), digitL.end());
        auto dpL = digitCount(digitL);

        vector<vector<long long>> res(dpR.size(), vector<long long>(10));
        for (int i = 0; i < int(dpR.size()); i++) {
            for (int j = 0; j < 10; j++)
                res[i][j] = dpR[i][j] - dpL[i][j];
        }

        return res;
    }

    // digits : most signigicant digit first (including leading zeros and "000...000")
    // result : the number of numbers to be grouped by inceasing digit sequence (ex: 3102 -> 0123)  
    //  - result[i][d] = the number of integers that i'th digit is d when the number converted to an increasing digit sequence
    static vector<vector<long long>> digitCount(const vector<int>& digits) {
        int N = int(digits.size());

        int from = 0;
        int to = 1;

        const int MAXD = 20;

        long long dpCntGE[2][2][10][MAXD];  // [from|to][tight][d][the number of digits (>= d)] = count
        long long dpCntGT[2][2][10][MAXD];  // [from|to][tight][d][the number of digits (>  d)] = count
        memset(dpCntGE[to], 0, sizeof(dpCntGE[to]));
        memset(dpCntGT[to], 0, sizeof(dpCntGT[to]));

        // first digit (most significant digit)
        for (int j = 0; j < 10; j++) {
            if (j < digits[0]) {
                dpCntGE[to][0][j][0] = j;
                dpCntGE[to][0][j][1] = digits[0] - j;

                dpCntGT[to][0][j][0] = j + 1;
                dpCntGT[to][0][j][1] = digits[0] - (j + 1);
            } else {
                dpCntGE[to][0][j][0] = digits[0];
                dpCntGT[to][0][j][0] = digits[0];
            }
        }
        for (int j = 0; j < 10; j++) {
            if (j < digits[0]) {
                dpCntGE[to][1][j][1] = 1;
                dpCntGT[to][1][j][1] = 1;
            } else if (j == digits[0]) {
                dpCntGE[to][1][j][1] = 1;
                dpCntGT[to][1][j][0] = 1;
            } else {
                dpCntGE[to][1][j][0] = 1;
                dpCntGT[to][1][j][0] = 1;
            }
        }

        // digits from second to last
        for (int i = 1; i < N; i++) {
            swap(from, to);
            memset(dpCntGE[to], 0, sizeof(dpCntGE[to]));
            memset(dpCntGT[to], 0, sizeof(dpCntGT[to]));

            // non-tight -> non-tight
            for (int j = 0; j < 10; j++) {
                for (int d = 0; d < 10; d++) {
                    if (d < j) {
                        for (int k = 0; k <= N; k++) {
                            dpCntGE[to][0][j][k] += dpCntGE[from][0][j][k];
                            dpCntGT[to][0][j][k] += dpCntGT[from][0][j][k];
                        }
                    } else if (d == j) {
                        for (int k = 1; k <= N; k++) {
                            dpCntGE[to][0][j][k] += dpCntGE[from][0][j][k - 1];
                            dpCntGT[to][0][j][k] += dpCntGT[from][0][j][k];
                        }
                        dpCntGT[to][0][j][0] += dpCntGT[from][0][j][0];
                    } else {
                        for (int k = 1; k <= N; k++) {
                            dpCntGE[to][0][j][k] += dpCntGE[from][0][j][k - 1];
                            dpCntGT[to][0][j][k] += dpCntGT[from][0][j][k - 1];
                        }
                    }
                }
            }

            // tight -> non-tight
            for (int j = 0; j < 10; j++) {
                for (int d = 0; d < digits[i]; d++) {
                    if (d < j) {
                        for (int k = 0; k <= N; k++) {
                            dpCntGE[to][0][j][k] += dpCntGE[from][1][j][k];
                            dpCntGT[to][0][j][k] += dpCntGT[from][1][j][k];
                        }
                    } else if (d == j) {
                        for (int k = 1; k <= N; k++) {
                            dpCntGE[to][0][j][k] += dpCntGE[from][1][j][k - 1];
                            dpCntGT[to][0][j][k] += dpCntGT[from][1][j][k];
                        }
                        dpCntGT[to][0][j][0] += dpCntGT[from][1][j][0];
                    } else {
                        for (int k = 1; k <= N; k++) {
                            dpCntGE[to][0][j][k] += dpCntGE[from][1][j][k - 1];
                            dpCntGT[to][0][j][k] += dpCntGT[from][1][j][k - 1];
                        }
                    }
                }
            }

            // tight -> tight
            for (int j = 0; j < 10; j++) {
                int d = digits[i];
                {
                    if (d < j) {
                        for (int k = 0; k <= N; k++) {
                            dpCntGE[to][1][j][k] += dpCntGE[from][1][j][k];
                            dpCntGT[to][1][j][k] += dpCntGT[from][1][j][k];
                        }
                    } else if (d == j) {
                        for (int k = 1; k <= N; k++) {
                            dpCntGE[to][1][j][k] += dpCntGE[from][1][j][k - 1];
                            dpCntGT[to][1][j][k] += dpCntGT[from][1][j][k];
                        }
                        dpCntGT[to][1][j][0] += dpCntGT[from][1][j][0];
                    } else {
                        for (int k = 1; k <= N; k++) {
                            dpCntGE[to][1][j][k] += dpCntGE[from][1][j][k - 1];
                            dpCntGT[to][1][j][k] += dpCntGT[from][1][j][k - 1];
                        }
                    }
                }
            }
        }

        vector<vector<long long>> res(N, vector<long long>(10));
        for (int j = 0; j < 10; j++) {
            long long x = 0;
            for (int i = 1; i <= N; i++) {
                x += dpCntGE[to][0][j][N - i + 1] - dpCntGT[to][0][j][N - i + 1]
                    + dpCntGE[to][1][j][N - i + 1] - dpCntGT[to][1][j][N - i + 1];
                res[i - 1][j] = x;
            }
        }

        return res;
    }

private:
    static vector<int> getDigits(long long x) {
        vector<int> digit;
        if (x == 0)
            digit.push_back(0);
        else {
            while (x) {
                digit.push_back(x % 10);
                x /= 10;
            }
        }
        return digit;
    }
};

template <int mod = 1000000007>
struct IncreasingDigitSequenceMod {
    // including '0';
    static vector<vector<long long>> digitCount(long long x) {
        vector<int> digits = getDigits(x);
        reverse(digits.begin(), digits.end());
        return digitCount(digits);
    }

    // range [L, R]
    static vector<vector<long long>> digitCount(long long L, long long R) {
        vector<int> digitR = getDigits(R);
        reverse(digitR.begin(), digitR.end());
        auto dpR = digitCount(digitR);

        if (L <= 0)
            return dpR;

        vector<int> digitL = getDigits(L - 1);
        digitL.resize(digitR.size(), 0);
        reverse(digitL.begin(), digitL.end());
        auto dpL = digitCount(digitL);

        vector<vector<long long>> res(dpR.size(), vector<long long>(10));
        for (int i = 0; i < int(dpR.size()); i++) {
            for (int j = 0; j < 10; j++) {
                res[i][j] = dpR[i][j] - dpL[i][j];
                if (res[i][j] < 0)
                    res[i][j] += mod;
            }
        }

        return res;
    }

    // digits : most signigicant digit first (including leading zeros and "000...000")
    // result : the number of numbers to be grouped by inceasing digit sequence (ex: 3102 -> 0123)  
    //  - result[i][d] = the number of integers that i'th digit is d when the number converted to an increasing digit sequence
    static vector<vector<long long>> digitCount(const vector<int>& digits) {
        int N = int(digits.size());

        int from = 0;
        int to = 1;

        const int MAXD = 20;

        long long dpCntGE[2][2][10][MAXD];  // [from|to][tight][d][the number of digits (>= d)] = count
        long long dpCntGT[2][2][10][MAXD];  // [from|to][tight][d][the number of digits (>  d)] = count
        memset(dpCntGE[to], 0, sizeof(dpCntGE[to]));
        memset(dpCntGT[to], 0, sizeof(dpCntGT[to]));

        // first digit (most significant digit)
        for (int j = 0; j < 10; j++) {
            if (j < digits[0]) {
                dpCntGE[to][0][j][0] = j;
                dpCntGE[to][0][j][1] = digits[0] - j;

                dpCntGT[to][0][j][0] = j + 1;
                dpCntGT[to][0][j][1] = digits[0] - (j + 1);
            } else {
                dpCntGE[to][0][j][0] = digits[0];
                dpCntGT[to][0][j][0] = digits[0];
            }
        }
        for (int j = 0; j < 10; j++) {
            if (j < digits[0]) {
                dpCntGE[to][1][j][1] = 1;
                dpCntGT[to][1][j][1] = 1;
            } else if (j == digits[0]) {
                dpCntGE[to][1][j][1] = 1;
                dpCntGT[to][1][j][0] = 1;
            } else {
                dpCntGE[to][1][j][0] = 1;
                dpCntGT[to][1][j][0] = 1;
            }
        }

        // digits from second to last
        for (int i = 1; i < N; i++) {
            swap(from, to);
            memset(dpCntGE[to], 0, sizeof(dpCntGE[to]));
            memset(dpCntGT[to], 0, sizeof(dpCntGT[to]));

            // non-tight -> non-tight
            for (int j = 0; j < 10; j++) {
                for (int d = 0; d < 10; d++) {
                    if (d < j) {
                        for (int k = 0; k <= N; k++) {
                            dpCntGE[to][0][j][k] += dpCntGE[from][0][j][k];
                            dpCntGT[to][0][j][k] += dpCntGT[from][0][j][k];
                        }
                    } else if (d == j) {
                        for (int k = 1; k <= N; k++) {
                            dpCntGE[to][0][j][k] += dpCntGE[from][0][j][k - 1];
                            dpCntGT[to][0][j][k] += dpCntGT[from][0][j][k];
                        }
                        dpCntGT[to][0][j][0] += dpCntGT[from][0][j][0];
                    } else {
                        for (int k = 1; k <= N; k++) {
                            dpCntGE[to][0][j][k] += dpCntGE[from][0][j][k - 1];
                            dpCntGT[to][0][j][k] += dpCntGT[from][0][j][k - 1];
                        }
                    }
                }
            }

            // tight -> non-tight
            for (int j = 0; j < 10; j++) {
                for (int d = 0; d < digits[i]; d++) {
                    if (d < j) {
                        for (int k = 0; k <= N; k++) {
                            dpCntGE[to][0][j][k] += dpCntGE[from][1][j][k];
                            dpCntGT[to][0][j][k] += dpCntGT[from][1][j][k];
                        }
                    } else if (d == j) {
                        for (int k = 1; k <= N; k++) {
                            dpCntGE[to][0][j][k] += dpCntGE[from][1][j][k - 1];
                            dpCntGT[to][0][j][k] += dpCntGT[from][1][j][k];
                        }
                        dpCntGT[to][0][j][0] += dpCntGT[from][1][j][0];
                    } else {
                        for (int k = 1; k <= N; k++) {
                            dpCntGE[to][0][j][k] += dpCntGE[from][1][j][k - 1];
                            dpCntGT[to][0][j][k] += dpCntGT[from][1][j][k - 1];
                        }
                    }
                }
            }

            // tight -> tight
            for (int j = 0; j < 10; j++) {
                int d = digits[i];
                {
                    if (d < j) {
                        for (int k = 0; k <= N; k++) {
                            dpCntGE[to][1][j][k] += dpCntGE[from][1][j][k];
                            dpCntGT[to][1][j][k] += dpCntGT[from][1][j][k];
                        }
                    } else if (d == j) {
                        for (int k = 1; k <= N; k++) {
                            dpCntGE[to][1][j][k] += dpCntGE[from][1][j][k - 1];
                            dpCntGT[to][1][j][k] += dpCntGT[from][1][j][k];
                        }
                        dpCntGT[to][1][j][0] += dpCntGT[from][1][j][0];
                    } else {
                        for (int k = 1; k <= N; k++) {
                            dpCntGE[to][1][j][k] += dpCntGE[from][1][j][k - 1];
                            dpCntGT[to][1][j][k] += dpCntGT[from][1][j][k - 1];
                        }
                    }
                }
            }

            for (int j = 0; j < 10; j++) {
                for (int k = 0; k < MAXD; k++) {
                    dpCntGE[to][0][j][k] %= mod;
                    dpCntGE[to][1][j][k] %= mod;
                }
            }
        }

        vector<vector<long long>> res(N, vector<long long>(10));
        for (int j = 0; j < 10; j++) {
            long long x = 0;
            for (int i = 1; i <= N; i++) {
                x += (dpCntGE[to][0][j][N - i + 1] - dpCntGT[to][0][j][N - i + 1]
                    + dpCntGE[to][1][j][N - i + 1] - dpCntGT[to][1][j][N - i + 1]) % mod;
                if (x < 0)
                    x += mod;
                res[i - 1][j] = x;
            }
        }

        return res;
    }

private:
    static vector<int> getDigits(long long x) {
        vector<int> digit;
        if (x == 0)
            digit.push_back(0);
        else {
            while (x) {
                digit.push_back(x % 10);
                x /= 10;
            }
        }
        return digit;
    }
};
