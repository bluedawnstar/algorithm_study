// O(n^3)
class Solution {
public:
    int maxCoins(vector<int>& nums) {
        if (nums.empty())
            return 0;

        if (nums.size() == 1u)
            return nums[0];

        nums.insert(nums.begin(), 1);
        nums.push_back(1);

        int n = (int)nums.size();
        vector<vector<int>> dp(n, vector<int>(n));
        for (int d = 3; d <= n; d++) {
            for (int l = 0; l <= n - d; l++) {
                int r = l + d - 1;

                int lxr = nums[l] * nums[r];
                for (int m = l + 1; m < r; m++) {
                    dp[l][r] = max(dp[l][r], dp[l][m] + dp[m][r] + nums[m] * lxr);
                }
            }
        }

        return dp[0][n - 1];
    }
};
