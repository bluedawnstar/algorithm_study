// O(N^2)
class Solution {
public:
    string longestPalindrome(string s) {
        if (s.empty())
            return string();

        int N = (int)s.length();
        vector<vector<int>> dp(N, vector<int>(N));

        int start = 0, end = 0;
        for (int i = 0; i < N; i++)
            dp[i][i] = 1;

        for (int i = N - 2; i >= 0; i--) {
            for (int j = i + 1; j < N; j++) {
                if (s[i] == s[j] && (i + 1 == j || dp[i + 1][j - 1] > 0)) {
                    dp[i][j] = dp[i + 1][j - 1] + 2;
                    if (dp[i][j] >(end - start + 1))
                        start = i, end = j;
                }
            }
        }

        return s.substr(start, end - start + 1);
    }
};