// O(n)
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.empty())
            return 0;

        int ans = 0;
        int minPrice = prices[0];
        for (int i = 1; i < (int)prices.size(); i++) {
            ans = max(ans, prices[i] - minPrice);
            minPrice = min(minPrice, prices[i]);
        }

        return ans;
    }
};
