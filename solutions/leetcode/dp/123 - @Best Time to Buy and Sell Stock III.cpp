class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = (int)prices.size();
        if (n < 2)
            return 0;

        vector<int> dp(n);
        for (int i = 0; i < 2; i++) {
            int bestBuy = dp[0] - prices[0];
            for (int j = 1; j < n; j++) {
                int sellProfit = max(0, bestBuy + prices[j]);
                bestBuy = max(bestBuy, dp[j] - prices[j]);
                dp[j] = max(dp[j - 1], sellProfit);
            }
        }

        return dp.back();
    }
};

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = (int)prices.size();
        if (n < 2)
            return 0;

        vector<vector<int>> dp(3, vector<int>(n));
        for (int i = 1; i <= 2; i++) {
            int bestBuy = dp[i - 1][0] - prices[0];
            for (int j = 1; j < n; j++) {
                int sellProfit = max(0, bestBuy + prices[j]);
                bestBuy = max(bestBuy, dp[i - 1][j] - prices[j]);
                dp[i][j] = max(dp[i][j - 1], sellProfit);
            }
        }

        return dp.back().back();
    }
};
