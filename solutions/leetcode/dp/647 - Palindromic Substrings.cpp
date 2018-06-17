class Solution {
public:
    int countSubstrings(string s) {
        if (s.empty())
            return 0;

        int n = (int)s.length();
        vector<vector<bool>> dp(n, vector<bool>(n));
        for (int i = 0; i < n; i++)
            dp[i][i] = true;

        int ans = n;
        for (int L = 0, R = 1; R < n; L++, R++) {
            if (s[L] == s[R]) {
                dp[L][R] = true;
                ans++;
            }
        }

        for (int i = 2; i < n; i++) {
            for (int L = 0, R = i; R < n; L++, R++) {
                if (s[L] == s[R] && dp[L + 1][R - 1]) {
                    dp[L][R] = true;
                    ans++;
                }
            }
        }

        return ans;
    }
};
