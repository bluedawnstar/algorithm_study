// O(nk)
class Solution {
public:
    int maxProfit(int k, vector<int>& prices) {
        int n = (int)prices.size();
        if (k <= 0 || n < 2)
            return 0;

        if (k >= n / 2) {
            int ans = 0;
            for (int i = 1; i < n; i++)
                ans += max(0, prices[i] - prices[i - 1]);
            return ans;
        }

        vector<int> dp(n);
        for (int i = 0; i < k; i++) {
            int bestBuy = dp[0] - prices[0];
            for (int j = 1; j < n; j++) {
                int sellProfit = bestBuy + prices[j];
                bestBuy = max(bestBuy, dp[j] - prices[j]);
                dp[j] = max(dp[j - 1], sellProfit);
            }
        }

        return dp.back();
    }
};