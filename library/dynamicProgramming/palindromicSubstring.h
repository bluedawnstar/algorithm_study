#pragma once

inline int countPalindromicSubstring(string s) {
    int N = (int)s.length();
    vector<vector<int>> dp(N, vector<int>(N));  // length

    int res = 0;                                // count

    for (int i = 0; i < N; i++) {
        dp[i][i] = 1;
        res++;
    }

    for (int i = 0; i < N - 1; i++) {
        if (s[i] == s[i + 1]) {
            dp[i][i + 1] = 2;
            res++;
        }
    }

    for (int i = N - 1; i >= 0; i--) {
        for (int j = i + 2; j < N; j++) {
            if (s[i] == s[j] && dp[i + 1][j - 1]) {
                dp[i][j] = dp[i + 1][j - 1] + 2;
                res++;
            }
        }
    }

    return res;
}

inline int lengthOfLongestPalindromicSubstring(string s) {
    int N = (int)s.length();
    vector<vector<int>> dp(N, vector<int>(N));

    int res = 1;

    for (int i = 0; i < N; i++)
        dp[i][i] = 1;

    for (int i = 0; i < N - 1; i++) {
        if (s[i] == s[i + 1]) {
            dp[i][i + 1] = 2;
            res = 2;
        }
    }

    for (int i = N - 1; i >= 0; i--) {
        for (int j = i + 2; j < N; j++) {
            if (s[i] == s[j] && dp[i + 1][j - 1]) {
                dp[i][j] = dp[i + 1][j - 1] + 2;
                res = max(res, dp[i][j]);
            }
        }
    }

    return res;
}

inline string longestPalindromicSubstring(string s) {
    int N = (int)s.length();
    vector<vector<int>> dp(N, vector<int>(N));

    int maxLen = 1;
    int maxPosR = 0;

    for (int i = 0; i < N; i++)
        dp[i][i] = 1;

    for (int i = 0; i < N - 1; i++) {
        if (s[i] == s[i + 1]) {
            dp[i][i + 1] = 2;
            if (maxLen < 2) {
                maxLen = 2;
                maxPosR = i + 1;
            }
        }
    }

    for (int i = N - 1; i >= 0; i--) {
        for (int j = i + 2; j < N; j++) {
            if (s[i] == s[j] && dp[i + 1][j - 1]) {
                dp[i][j] = dp[i + 1][j - 1] + 2;
                if (maxLen < dp[i][j]) {
                    maxLen = dp[i][j];
                    maxPosR = j;
                }
            }
        }
    }

    // construct longest palindromic substring
    string res;
    res.reserve(maxLen);

    int i, j;
    for (i = maxPosR - maxLen + 1, j = maxPosR; i < j && dp[i][j] == dp[i + 1][j - 1] + 2; i++, j--) {
        res.append(1, s[i]);
    }

    int half = (int)res.length();

    if ((maxLen & 1) != 0)
        res.append(1, s[i]);

    for (i = half - 1; i >= 0; i--)
        res.append(1, res[i]);

    return res;
}
