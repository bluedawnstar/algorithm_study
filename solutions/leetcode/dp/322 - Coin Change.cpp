// O(n*amount)
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        if (coins.empty())
            return -1;

        vector<int> dp(amount + 1, -1);
        dp[0] = 0;
        for (int i = 0; i < (int)coins.size(); i++) {
            for (int j = 0; j <= amount; j++) {
                if (dp[j] == -1)
                    continue;

                int k = j + coins[i];
                if (k >= 0 && k <= amount)
                    dp[k] = dp[k] <= 0 ? dp[j] + 1 : min(dp[k], dp[j] + 1);
            }
        }

        return dp.back();
    }
};