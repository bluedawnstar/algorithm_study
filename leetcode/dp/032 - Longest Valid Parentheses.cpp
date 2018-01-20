// DP + stack
class Solution {
public:
    int longestValidParentheses(string s) {
        int n = (int)s.length();

        int ans = 0;
        vector<int> dp(n + 1);

        stack<int> st;
        for (int i = 0; i < n; i++) {
            if (s[i] == '(')
                st.push(i);
            else if (!st.empty()) {
                int len = i - st.top() + 1;
                st.pop();

                dp[i + 1] = len + dp[i + 1 - len];
                ans = max(ans, dp[i + 1]);
            }
        }

        return ans;
    }
};

// DP
class Solution {
public:
    int longestValidParentheses(string s) {
        int n = (int)s.length();

        int ans = 0;
        vector<int> dp(n);

        for (int i = 1; i < n; i++) {
            if (s[i] == ')') {
                if (s[i - 1] == '(') {  // "...()"
                    dp[i] = 2 + (i >= 2 ? dp[i - 2] : 0);
                    ans = max(ans, dp[i]);
                } else if (dp[i - 1] > 0) {
                    int j = i - dp[i - 1] - 1;
                    if (j >= 0 && s[j] == '(') {    // "...((...))"
                        dp[i] = dp[i - 1] + 2 + (j > 0 ? dp[j - 1] : 0);
                        ans = max(ans, dp[i]);
                    }
                }
            }
        }

        return ans;
    }
};
