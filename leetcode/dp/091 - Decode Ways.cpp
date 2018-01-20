// O(n)
class Solution {
public:
    int numDecodings(string s) {
        if (s.empty())
            return 0;

        int n = (int)s.length();
        vector<int> dp(n + 1);

        dp[0] = 1;
        for (int i = 0; i < n; i++) {
            if (s[i] == '0')
                continue;

            if (s[i] == '1') {
                if (i + 1 < n)
                    dp[i + 2] += dp[i];
            } else if (s[i] == '2') {
                if (i + 1 < n && s[i + 1] <= '6')
                    dp[i + 2] += dp[i];
            }
            dp[i + 1] += dp[i];
        }

        return dp.back();
    }
};
