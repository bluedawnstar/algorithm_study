class Solution {
public:
    int numSquares(int n) {
        if (n <= 2)
            return n;

        vector<int> dp(n + 1, n);

        dp[0] = 0;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j * j <= i; j++) {
                dp[i] = min(dp[i], dp[i - j * j] + 1);
            }
        }

        return dp[n];
    }
};