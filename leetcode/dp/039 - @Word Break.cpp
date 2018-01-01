class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> S;
        for (auto& w : wordDict)
            S.insert(w);

        int n = (int)s.length();
        vector<bool> dp(n + 1);
        dp[0] = true;
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < i; j++) {
                if (S.find(s.substr(j, i - j)) != S.end() && dp[j]) {
                    dp[i] = true;
                    break;
                }
            }
        }
        return dp.back();
    }
};