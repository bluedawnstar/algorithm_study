#pragma once

// https://leetcode.com/problems/valid-parenthesis-string/

// '(', ')', '*'
// - '*' could be treated as
//    > a single right parenthesis ')'
//    > or a single left parenthesis '('
//    > or an empty string.
// - An empty string is also valid.
struct BalancedBracketSequencesWithStar {
    // DP solution, O(n^3)
    static bool checkValidDP(const string& s) {
        int n = int(s.length());
        if (n == 0)
            return true;

        vector<vector<bool>> dp(n, vector<bool>(n));

        for (int i = 0; i < n; i++) {
            if (s[i] == '*')
                dp[i][i] = true;
        }
        for (int i = 0; i + 1 < n; i++) {
            if ((s[i] == '(' || s[i] == '*') && (s[i + 1] == ')' || s[i + 1] == '*'))
                dp[i][i + 1] = true;
        }

        for (int i = n - 1; i >= 0; i--) {
            for (int j = i + 2; j < n; j++) {
                dp[i][j] = ((s[i] == '*' || s[i] == '(') && (s[j] == '*' || s[j] == ')') && dp[i + 1][j - 1]) // *X*, *X), (X*, (X)
                        || (s[i] == '*' && dp[i + 1][j])    // *X
                        || (s[j] == '*' && dp[i][j - 1]);   // X*

                // X + Y
                for (int k = i; k < j && !dp[i][j]; k++) {
                    dp[i][j] = dp[i][k] && dp[k + 1][j];
                }
            }
        }

        return dp[0][n - 1];
    }

    // Greedy solution, O(n)
    static bool checkValid(const string& s) {
        int lo = 0;
        int hi = 0;
        for (auto c : s) {
#if 0
            lo += (c == '(') ? 1 : -1;
            hi += (c != ')') ? 1 : -1;
#else
            if (c == '(') {
                lo++;
                hi++;
            } else if (c == '*') {
                lo--;
                hi++;
            } else {
                lo--;
                hi--;
            }
#endif
            if (hi < 0)
                return false;
            if (lo < 0)
                lo = 0;
        }
        return lo == 0;
    }
};
