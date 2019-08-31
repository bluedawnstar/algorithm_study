#pragma once

/////////// Palindromic Substrings ////////////////////////////////////////////

// DP solutions
struct PalindromicSubstringDP {
    // O(N^2)
    static int countPalindromicSubstring(const char* s, int N) {
        vector<vector<int>> dp(N, vector<int>(N));

        int res = 0;

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

        for (int i = N - 2; i >= 0; i--) {
            for (int j = i + 2; j < N; j++) {
                if (s[i] == s[j] && dp[i + 1][j - 1]) {
                    dp[i][j] = dp[i + 1][j - 1] + 2;
                    res++;
                }
            }
        }

        return res;
    }

    static int countPalindromicSubstring(const string& s) {
        return countPalindromicSubstring(s.c_str(), int(s.length()));
    }


    // O(N^2)
    static int lengthOfLongestPalindromicSubstring(const char* s, int N) {
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

        for (int i = N - 2; i >= 0; i--) {
            for (int j = i + 2; j < N; j++) {
                if (s[i] == s[j] && dp[i + 1][j - 1]) {
                    dp[i][j] = dp[i + 1][j - 1] + 2;
                    res = max(res, dp[i][j]);
                }
            }
        }

        return res;
    }

    static int lengthOfLongestPalindromicSubstring(const string& s) {
        return lengthOfLongestPalindromicSubstring(s.c_str(), int(s.length()));
    }


    // O(N^2)
    static string longestPalindromicSubstring(const char* s, int N) {
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

        for (int i = N - 2; i >= 0; i--) {
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

        int half = int(res.length());

        if ((maxLen & 1) != 0)
            res.append(1, s[i]);

        for (i = half - 1; i >= 0; i--)
            res.append(1, res[i]);

        return res;
    }

    static string longestPalindromicSubstring(const string& s) {
        return longestPalindromicSubstring(s.c_str(), int(s.length()));
    }
};
