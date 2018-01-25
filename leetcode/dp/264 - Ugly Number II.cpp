class Solution {
public:
    int nthUglyNumber(int n) {
        vector<int> dp;
        dp.reserve(n);

        dp.push_back(1);

        int idx2 = 0;
        int idx3 = 0;
        int idx5 = 0;
        for (int i = 1; i < n; i++) {
            int minNum = min(dp[idx2] * 2, min(dp[idx3] * 3, dp[idx5] * 5));
            if (minNum == dp[idx2] * 2)
                idx2++;
            if (minNum == dp[idx3] * 3)
                idx3++;
            if (minNum == dp[idx5] * 5)
                idx5++;
            dp.push_back(minNum);
        }

        return dp.back();
    }
};