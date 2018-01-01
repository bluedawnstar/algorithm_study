class Solution {
public:
    // O(N^2)
    int maximalRectangle(vector<vector<char>>& matrix) {
        int rows = matrix.size();
        if (rows == 0)
            return 0;
        int cols = matrix[0].size();

        vector<int> height(cols, 0);
        vector<int> st;
        st.reserve(cols);

        int res = 0;

        for (int i = 0; i < rows; i++) {
            st.clear();
            st.push_back(-1);

            for (int j = 0; j < cols; j++) {
                if (matrix[i][j] == '0')
                    height[j] = 0;
                else
                    height[j]++;

                while (st.back() >= 0 && height[j] <= height[st.back()]) {
                    int h = height[st.back()];
                    st.pop_back();

                    res = max(res, (j - 1 - st.back()) * h);
                }
                st.push_back(j);
            }

            int r = st.back();
            while (st.back() >= 0) {
                int h = height[st.back()];
                st.pop_back();

                res = max(res, (r - st.back()) * h);
            }
        }

        return res;
    }
};

class Solution_DP {
public:
    // O(N^3)
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
