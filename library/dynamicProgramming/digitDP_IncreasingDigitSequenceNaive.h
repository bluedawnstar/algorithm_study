#pragma once

struct IncreasingDigitSequenceNaive {
    // including '0';
    static vector<vector<long long>> digitCount(long long x) {
        vector<int> digits = getDigits(x);
        reverse(digits.begin(), digits.end());
        return digitCount(digits);
    }

    // range [L, R]
    static vector<vector<long long>> digitCount(long long L, long long R) {
        vector<int> dR = getDigits(R);
        reverse(dR.begin(), dR.end());
        auto dpR = digitCount(dR);

        if (L <= 0)
            return dpR;

        vector<int> dL = getDigits(L - 1);
        dL.resize(dR.size(), 0);
        reverse(dL.begin(), dL.end());
        auto dpL = digitCount(dL);

        vector<vector<long long>> res(dpL.size(), vector<long long>(10));
        for (int i = 0; i < int(dpL.size()); i++) {
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

        unordered_map<long long, long long> dpM;  // non-tight
        unordered_map<long long, long long> dpM2; // tight

        for (int j = 0; j <= digits[0]; j++) {
            if (j < digits[0])
                dpM[j] = 1;
            else
                dpM2[j] = 1;
        }

        for (int i = 1; i < N; i++) {
            unordered_map<long long, long long> M;
            unordered_map<long long, long long> M2;

            // non-tight -> non-tight
            {
                long long x, cnt;
                for (auto& it : dpM) {
                    tie(x, cnt) = it;

                    M[x] += cnt;
                    M[x * 10 + 9] += cnt;

                    long long scale = 1;
                    long long q = x, d = x % 10, r = 0;
                    for (int j = 8; j > 0; j--) {
                        while (j < d) {
                            r += d * scale;
                            q /= 10;
                            d = q % 10;
                            scale *= 10;
                        }
                        M[(q * 10 + j) * scale + r] += cnt;
                    }
                }
            }
            // tight -> non-tight
            if (digits[i] > 0) {
                long long x, cnt;
                for (auto& it : dpM2) {
                    tie(x, cnt) = it;

                    M[x] += cnt;

                    long long scale = 1;
                    long long q = x, d = x % 10, r = 0;
                    for (int j = digits[i] - 1; j > 0; j--) {
                        while (j < d) {
                            r += d * scale;
                            q /= 10;
                            d = q % 10;
                            scale *= 10;
                        }
                        M[(q * 10 + j) * scale + r] += cnt;
                    }
                }
            }
            // tight -> tight
            {
                long long x, cnt;
                for (auto& it : dpM2) {
                    tie(x, cnt) = it;

                    long long scale = 1;
                    long long q = x, d = x % 10, r = 0;
                    while (digits[i] < d) {
                        r += d * scale;
                        q /= 10;
                        d = q % 10;
                        scale *= 10;
                    }
                    M2[(q * 10 + digits[i]) * scale + r] += cnt;
                }
            }

            swap(dpM, M);
            swap(dpM2, M2);
        }

        vector<vector<long long>> res(N, vector<long long>(10));
        for (auto& it : dpM) {
            long long x = it.first;
            long long cnt = it.second;
            for (int i = 0; i < N; i++) {
                int d = int(x % 10);
                res[i][d] += cnt;
                x /= 10;
            }            
        }
        for (auto& it : dpM2) {
            long long x = it.first;
            long long cnt = it.second;
            for (int i = 0; i < N; i++) {
                int d = int(x % 10);
                res[i][d] += cnt;
                x /= 10;
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
struct IncreasingDigitSequenceNaiveMod {
    // including '0';
    static vector<vector<long long>> digitCount(long long x) {
        vector<int> digits = getDigits(x);
        reverse(digits.begin(), digits.end());
        return digitCount(digits);
    }

    // range [L, R]
    static vector<vector<long long>> digitCount(long long L, long long R) {
        vector<int> dR = getDigits(R);
        reverse(dR.begin(), dR.end());
        auto dpR = digitCount(dR);

        if (L <= 0)
            return dpR;

        vector<int> dL = getDigits(L - 1);
        dL.resize(dR.size(), 0);
        reverse(dL.begin(), dL.end());
        auto dpL = digitCount(dL);

        vector<vector<long long>> res(dpL.size(), vector<long long>(10));
        for (int i = 0; i < int(dpL.size()); i++) {
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

        unordered_map<long long, long long> dpM;  // non-tight
        unordered_map<long long, long long> dpM2; // tight

        for (int j = 0; j <= digits[0]; j++) {
            if (j < digits[0])
                dpM[j] = 1;
            else
                dpM2[j] = 1;
        }

        for (int i = 1; i < N; i++) {
            unordered_map<long long, long long> M;
            unordered_map<long long, long long> M2;

            // non-tight -> non-tight
            {
                long long x, cnt;
                for (auto& it : dpM) {
                    tie(x, cnt) = it;
                    cnt %= mod;

                    M[x] += cnt;
                    M[x * 10 + 9] += cnt;

                    long long scale = 1;
                    long long q = x, d = x % 10, r = 0;
                    for (int j = 8; j > 0; j--) {
                        while (j < d) {
                            r += d * scale;
                            q /= 10;
                            d = q % 10;
                            scale *= 10;
                        }
                        M[(q * 10 + j) * scale + r] += cnt;
                    }
                }
            }
            // tight -> non-tight
            if (digits[i] > 0) {
                long long x, cnt;
                for (auto& it : dpM2) {
                    tie(x, cnt) = it;
                    cnt %= mod;

                    M[x] += cnt;

                    long long scale = 1;
                    long long q = x, d = x % 10, r = 0;
                    for (int j = digits[i] - 1; j > 0; j--) {
                        while (j < d) {
                            r += d * scale;
                            q /= 10;
                            d = q % 10;
                            scale *= 10;
                        }
                        M[(q * 10 + j) * scale + r] += cnt;
                    }
                }
            }
            // tight -> tight
            {
                long long x, cnt;
                for (auto& it : dpM2) {
                    tie(x, cnt) = it;
                    cnt %= mod;

                    long long scale = 1;
                    long long q = x, d = x % 10, r = 0;
                    while (digits[i] < d) {
                        r += d * scale;
                        q /= 10;
                        d = q % 10;
                        scale *= 10;
                    }
                    M2[(q * 10 + digits[i]) * scale + r] += cnt;
                }
            }

            swap(dpM, M);
            swap(dpM2, M2);
        }

        vector<vector<long long>> res(N, vector<long long>(10));
        for (auto& it : dpM) {
            long long x = it.first;
            long long cnt = it.second % mod;
            for (int i = 0; i < N; i++) {
                int d = int(x % 10);
                res[i][d] += cnt;
                if (res[i][d] >= mod)
                    res[i][d] -= mod;
                x /= 10;
            }            
        }

        for (auto& it : dpM2) {
            long long x = it.first;
            long long cnt = it.second % mod;
            for (int i = 0; i < N; i++) {
                int d = int(x % 10);
                res[i][d] += cnt;
                if (res[i][d] >= mod)
                    res[i][d] -= mod;
                x /= 10;
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
