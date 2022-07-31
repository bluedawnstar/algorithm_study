#pragma once

/*
    Sum of all the subsequences of the given number

    X = x[0]x[1]...x[i-1]x[i]x[i+1]x[i+2]...x[n-2]x[n-1]

             n-1
    result = SUM { 2^i * (x[i] - '0') * (10 + 1)^{n - i - 1} }
             i=0
*/
template <int mod = 1'000'000'007>
struct SubsequenceSum {
    vector<int> pow2;
    vector<int> pow11;

    void prepare(int maxN) {
        pow2 = vector<int>(maxN + 1);
        pow11 = vector<int>(maxN + 1);

        pow2[0] = 1;
        pow11[0] = 1;
        for (int i = 1; i <= maxN; i++) {
            pow2[i] = int(pow2[i - 1] * 2ll % mod);
            pow11[i] = int(pow11[i - 1] * 11ll % mod);
        }
    }

    int solve(string s) {
        reverse(s.begin(), s.end());
        int N = int(s.length());

        int res = 0;
        for (int i = 0; i < N; i++) {
            res = int((res + 1ll * (s[i] - '0') * pow11[i] % mod * pow2[N - i - 1]) % mod);
        }

        return res;
    }
};
