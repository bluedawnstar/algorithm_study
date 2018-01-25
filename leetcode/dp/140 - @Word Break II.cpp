// DP & DFS
class Solution {
public:
    void dfs(const string& s, const vector<vector<int>>& dp, vector<string>& res, vector<int>& t, int pos) {
        if (pos <= 0) {
            string temp;
            for (int i = (int)t.size() - 1, j = 0; i >= 0; i--) {
                if (!temp.empty())
                    temp += ' ';
                temp += s.substr(j, t[i]);
                j += t[i];
            }
            res.push_back(temp);
            return;
        }

        for (auto len : dp[pos]) {
            if (pos - len < 0)
                continue;

            t.push_back(len);
            dfs(s, dp, res, t, pos - len);
            t.pop_back();
        }
    }

    vector<string> wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> words;
        for (auto& w : wordDict)
            words.insert(w);

        int n = (int)s.length();
        vector<vector<int>> dp(n + 1);
        dp[0].push_back(0);
        for (int i = 0; i < n; i++) {
            if (dp[i].empty())
                continue;

            string t;
            for (int j = i; j < n; j++) {
                t += s[j];
                if (words.find(t) != words.end())
                    dp[j + 1].push_back(j - i + 1);
            }
        }

        vector<string> res;
        vector<int> t;
        dfs(s, dp, res, t, n);
        return res;
    }
};

// Memoization
class Solution {
public:
    vector<string> dfs(const string& s, const unordered_set<string>& words, unordered_map<string, vector<string>>& M) {
        vector<string> res;

        if (s.empty()) {
            res.push_back("");
            return res;
        }

        if (M.find(s) != M.end())
            return M[s];

        string prefix;
        for (int i = 0; i < (int)s.length(); i++) {
            prefix += s[i];
            if (words.find(prefix) != words.end()) {
                auto suffixes = dfs(s.substr(i + 1, (int)s.length() - i - 1), words, M);
                for (auto& t : suffixes) {
                    if (t.empty())
                        res.push_back(prefix);
                    else
                        res.push_back(prefix + " " + t);
                }
            }
        }

        return M[s] = res;
    }

    vector<string> wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> words;
        for (auto& w : wordDict)
            words.insert(w);

        unordered_map<string, vector<string>> M;
        return dfs(s, words, M);
    }
};
