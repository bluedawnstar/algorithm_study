// O(N^2)
class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        int rows = matrix.size();
        if (rows == 0)
            return 0;
        int cols = matrix[0].size();

        vector<int> height(cols + 1, 0);    // heights

        int res = 0;
        for (int i = 0; i < rows; i++) {
            stack<pair<int, int>> stk;      // (height, index)
            stk.emplace(-1, -1);

            for (int j = 0; j <= cols; j++) {
                if (j >= cols || matrix[i][j] == '0')
                    height[j] = 0;
                else
                    height[j]++;

                int R = j - 1;
                while (stk.top().first >= height[j]) {
                    int h = stk.top().first;
                    stk.pop();
                    res = max(res, (R - stk.top().second) * h);
                }
                stk.emplace(height[j], j);
            }
        }

        return res;
    }
};

// O(N^3)
class Solution_DP {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        int n = (int)matrix.size();
        if (!n)
            return 0;
        int m = (int)matrix[0].size();
        if (!m)
            return 0;

        vector<vector<int>> dp(n, vector<int>(m));
        for (int i = 0; i < n; i++) {
            dp[i][0] = (matrix[i][0] == '1');
            for (int j = 1; j < m; j++)
                dp[i][j] = (matrix[i][j] == '1') ? dp[i][j - 1] + 1 : 0;
        }

        int ans = 0;
        for (int i = 0; i < m; i++) {
            for (int j = i; j < m; j++) {
                int sum = 0, len = j - i + 1;
                for (int k = 0; k < n; k++) {
                    if (dp[k][j] >= len) {
                        sum += len;
                        ans = max(ans, sum);
                    } else {
                        sum = 0;
                    }
                }
            }
        }

        return ans;
    }
};
