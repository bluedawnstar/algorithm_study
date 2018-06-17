// memory: O(n)
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.empty())
            return 0;

        int n = (int)prices.size();
        vector<int> dp(n);

        int bestBuy = dp[0] - prices[0];
        for (int i = 1; i < n; i++) {
            int profit = max(0, bestBuy + prices[i]);
            bestBuy = max(bestBuy, (i >= 2 ? dp[i - 2] : 0) - prices[i]);
            dp[i] = max(dp[i - 1], profit);
        }

        return dp.back();
    }
};

// memory: O(1)
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.empty())
            return 0;

        int n = (int)prices.size();
        int pprev = 0, prev = 0, curr = 0;

        int bestBuy = pprev - prices[0];
        for (int i = 1; i < n; i++) {
            pprev = prev;
            prev = curr;

            int profit = max(0, bestBuy + prices[i]);
            bestBuy = max(bestBuy, pprev - prices[i]);
            curr = max(curr, profit);
        }

        return curr;
    }
};