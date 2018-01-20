class Solution {
public:
    int minimumTotal(vector<vector<int>>& triangle) {
        if (triangle.empty() || triangle[0].empty())
            return 0;

        vector<int> dp(triangle.back().size(), 0);
        dp[0] = triangle[0][0];
        for (int i = 1; i < triangle.size(); i++) {
            dp[i] = dp[i - 1] + triangle[i][i];
            for (int j = i - 1; j > 0; j--)
                dp[j] = min(dp[j - 1], dp[j]) + triangle[i][j];
            dp[0] = dp[0] + triangle[i][0];
        }

        return *min_element(dp.begin(), dp.end());
    }
};