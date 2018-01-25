class Solution {
public:
    int integerBreak(int n) {
        if (n == 2)
            return 1;
        else if (n == 3)
            return 2;
        else if (n == 4)
            return 4;

        vector<int> dp(max(n, 4) + 1);
        dp[2] = 1;
        dp[3] = 2;
        dp[4] = 4;
        for (int i = 5; i <= n; i++) {
            for (int j = 2; j <= i / 2; j++) {
                int a = j;
                int b = i - j;
                dp[i] = max(dp[i], max(a, dp[a]) * max(b, dp[b]));
            }
        }
        return dp[n];
    }
};

class Solution {
public:
    int integerBreak(int n) {
        if (n == 2)
            return 1;
        else if (n == 3)
            return 2;
        else if (n == 4)
            return 4;

        int res = 1;
        while (n > 4) {
            res *= 3;
            n -= 3;
        }
        return res * n;
    }
};