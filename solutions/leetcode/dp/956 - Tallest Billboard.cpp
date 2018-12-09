// method #1 : O(3^n)
class Solution {
public:
    int tallestBillboard(vector<int>& rods) {
        int N = int(rods.size());
        if (N < 2)
            return 0;

        int res = 0;

        int size = 1 << N;
        vector<int> tbl(size);

        int j = -1, mask = 0;
        for (int i = 1; i < size; i++) {
            if ((i & (i - 1)) == 0)
                mask = (1 << ++j) - 1;
            tbl[i] = tbl[i & mask] + rods[j];

            int msk = ~i & mask;
            if (tbl[msk] < tbl[i] || tbl[i] <= res)
                continue;

            for (int s = msk; s; s = (s - 1) & msk) {
                if (tbl[i] == tbl[s]) {
                    res = max(res, tbl[i]);
                    break;
                }
            }
        }

        return res;
    }
};

// method #2 : O(n*sum)
class Solution {
public:
    int tallestBillboard(vector<int>& rods) {
        int N = int(rods.size());
        if (N < 2)
            return 0;

        int res = 0;

        vector<vector<int>> dp(N, vector<int>(10001, -1));
        dp[0][5000 + rods[0]] = rods[0];
        dp[0][5000 - rods[0]] = 0;
        dp[0][5000] = 0;
        for (int i = 1; i < N; i++) {
            for (int j = 0; j <= 10000; j++) {
                if (dp[i - 1][j] < 0)
                    continue;

                dp[i][j] = max(dp[i][j], dp[i - 1][j]);
                dp[i][j + rods[i]] = max(dp[i][j + rods[i]], dp[i - 1][j] + rods[i]);
                dp[i][j - rods[i]] = max(dp[i][j - rods[i]], dp[i - 1][j]);
            }
            res = max(res, dp[i][5000]);
        }

        return res;
    }
};