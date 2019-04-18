#pragma once

// https://www.codechef.com/APRIL19A/problems/KLPM
// the number of ways to choose an unordered pair of non-overlapping non-empty substrings
// of this string (let's denote them by s1 and s2 in such a way that s2 starts after s1 ends)
// such that their concatenation s1 + s2 is a palindrome
// O(N^2)
inline long long countConcatenatedPalindromicSubstring(string s) {
    int N = int(s.length());

    if (N == 1)
        return 0ll;
    else if (N == 2)
        return (long long)(s[0] == s[1]);

    //--- prepare

    vector<vector<bool>> dpFull(N, vector<bool>(N));    // left & right aligned palindromic substring
    vector<vector<int>> dpL(N, vector<int>(N));         // the number of left aligned palindromic substrings
    vector<vector<int>> dpR(N, vector<int>(N));         // the number of right aligned palindromic substrings

    for (int i = 0; i < N; i++) {
        dpFull[i][i] = true;
        dpL[i][i] = 1;
        dpR[i][i] = 1;
    }

    for (int i = 0; i < N - 1; i++) {
        if (s[i] == s[i + 1])
            dpFull[i][i + 1] = true;
        dpL[i][i + 1] = dpL[i][i] + dpFull[i][i + 1];
        dpR[i][i + 1] = dpR[i][i] + dpFull[i][i + 1];
    }

    for (int i = N - 1; i >= 0; i--) {
        for (int j = i + 2; j < N; j++) {
            if (s[i] == s[j])
                dpFull[i][j] = dpFull[i + 1][j - 1];
            dpL[i][j] = dpL[i][j - 1] + dpFull[i][j];
            dpR[i][j] = dpR[i + 1][j] + dpFull[i][j];
        }
    }

    //-- calculate
    long long res = 0;
    vector<vector<int>> dp(N, vector<int>(N));

    for (int i = 0; i < N - 1; i++) {
        if (s[i] == s[i + 1])
            dp[i][i + 1] = 1;
        res += dp[i][i + 1];                    // x
    }

    for (int i = N - 1; i >= 0; i--) {
        if (i + 2 < N) {
            if (s[i] == s[i + 2]) {
                dp[i][i + 2] = 3;
                res += dp[i][i + 2];            // x|yx, xy|x, x.x
            }
        }
        for (int j = i + 3; j < N; j++) {
            if (s[i] == s[j]) {
                if (dpFull[i + 1][j - 1])
                    dp[i][j] = 2;               // x|y***yx, xy***y|x

                dp[i][j] += dp[i + 1][j - 1]    // xy***yx
                         + dpL[i + 1][j - 2]    // xy**y.x
                         + dpR[i + 2][j - 1]    // x.y**yx
                         + 1;                   // x.....x

                res += dp[i][j];
            }
        }
    }

    return res;
}
