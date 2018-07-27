#pragma once

///////////////////////////////////////////////////////////////////////////////
// LCS (Longest Common Subsequence)

// O(N*M)
inline int lengthOfLcs(const string& s1, const string& s2) {
    int n = int(s1.length());
    int m = int(s2.length());

    vector<vector<int>> dp(n + 1, vector<int>(m + 1));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s1[i - 1] == s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }

    return dp[n][m];
}

// O(N*M)
inline string lcs(const string& s1, const string& s2) {
    int n = int(s1.length());
    int m = int(s2.length());

    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s1[i - 1] == s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }

    int pos = dp[n][m];
    string res(pos, ' ');

    int i = n, j = m;
    while (i > 0 && j > 0) {
        if (s1[i - 1] == s2[j - 1]) {
            res[--pos] = s1[i - 1];
            i--;
            j--;
        } else if (dp[i - 1][j] > dp[i][j - 1])
            i--;
        else
            j--;
    }

    return res;
}

// O(N*M)
inline int countOfAllLcs(const string& s1, const string& s2) {
    int n = int(s1.length());
    int m = int(s2.length());

    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    vector<vector<int>> cnt(n + 1, vector<int>(m + 1));

    for (int i = 0; i <= m; i++)
        cnt[0][i] = 1;

    for (int i = 1; i <= n; i++)
        cnt[i][0] = 1;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                cnt[i][j] = cnt[i - 1][j - 1];
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }

            if (dp[i][j] == dp[i][j - 1])
                cnt[i][j] += cnt[i][j - 1];

            if (dp[i][j] == dp[i - 1][j])
                cnt[i][j] += cnt[i - 1][j];

            if (dp[i][j] == dp[i - 1][j - 1])
                cnt[i][j] -= cnt[i - 1][j - 1];
        }
    }

    return cnt[n][m];
}

struct LcsFinder {
    static vector<string> allLcs(const string& s1, const string& s2) {
        int n = int(s1.length());
        int m = int(s2.length());

        vector<vector<int>> dp(n + 1, vector<int>(m + 1));
        vector<vector<pair<int, int>>> match(min(n, m) + 1);    // count -> (i, j)
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (s1[i - 1] == s2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                    match[dp[i][j]].emplace_back(i, j);
                } else {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        int len = dp[n][m];

        vector<string> res;
        string lcs(len, ' ');
        dfsAllLcs(s1, match, len, n + 1, m + 1, lcs, res);
        if (res.empty())
            res.push_back("");
        return res;
    }

    static unordered_map<string, int> allDistinctLcs(const string& s1, const string& s2) {
        int n = int(s1.length());
        int m = int(s2.length());

        vector<vector<int>> dp(n + 1, vector<int>(m + 1));
        vector<vector<pair<int, int>>> match(min(n, m) + 1);    // count -> (i, j)
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (s1[i - 1] == s2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                    match[dp[i][j]].emplace_back(i, j);
                } else {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        int len = dp[n][m];

        unordered_map<string, int> res;
        string lcs(len, ' ');
        dfsAllDistinctLcs(s1, match, len, n + 1, m + 1, lcs, res);
        if (res.empty())
            ++res[""];
        return res;
    }

private:
    static void dfsAllLcs(const string& s1, vector<vector<pair<int, int>>>& match,
        int pos, int n, int m, string& lcs, vector<string>& res) {
        if (pos <= 0) {
            res.push_back(lcs);
            return;
        }

        for (auto& it : match[pos]) {
            if (it.first >= n || it.second >= m)
                continue;
            lcs[pos - 1] = s1[it.first - 1];
            dfsAllLcs(s1, match, pos - 1, it.first, it.second, lcs, res);
        }
    }

    static void dfsAllDistinctLcs(const string& s1, vector<vector<pair<int, int>>>& match,
        int pos, int n, int m, string& lcs, unordered_map<string, int>& res) {
        if (pos <= 0) {
            ++res[lcs];
            return;
        }

        for (auto& it : match[pos]) {
            if (it.first >= n || it.second >= m)
                continue;
            lcs[pos - 1] = s1[it.first - 1];
            dfsAllDistinctLcs(s1, match, pos - 1, it.first, it.second, lcs, res);
        }
    }
};

struct LcsDAG {
    struct NodeT {
        int i, j;
        char ch;
        vector<int> next;
    };

    vector<NodeT> nodes;

    // the start node is nodes[0]
    void makeLcsDAG(const string& s1, const string& s2) {
        int n = int(s1.length());
        int m = int(s2.length());

        nodes.clear();
        nodes.push_back(NodeT{ -1, -1, 0, vector<int>{} });

        vector<vector<int>> dp(n + 1, vector<int>(m + 1));
        vector<vector<int>> match(min(n, m) + 1);    // count -> (i, j)
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (s1[i - 1] == s2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                    match[dp[i][j]].push_back(int(nodes.size()));
                    nodes.push_back(NodeT{ i - 1, j - 1, s1[i - 1], vector<int>{} });
                } else {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        int len = dp[n][m];
        if (len > 0) {
            vector<bool> visited(nodes.size());
            dfsLcsDAG(match, visited, len, n, m, -1);
        }
    }

    vector<string> allLcs() {
        vector<string> res;
        string lcs;
        dfsAllLcs(0, lcs, res);
        return res;
    }

    unordered_map<string, int> allDistinctLcs() {
        unordered_map<string, int> res;
        string lcs;
        dfsAllDistinctLcs(0, lcs, res);
        return res;
    }

private:
    void dfsLcsDAG(vector<vector<int>>& match, vector<bool>& visited, int pos, int n, int m, int parent) {
        if (pos <= 0) {
            nodes[0].next.push_back(parent);
            return;
        }

        for (auto it : match[pos]) {
            NodeT& node = nodes[it];
            if (node.i >= n || node.j >= m)
                continue;

            if (parent >= 0)
                nodes[it].next.push_back(parent);

            if (!visited[it]) {
                visited[it] = true;
                dfsLcsDAG(match, visited, pos - 1, node.i, node.j, it);
            }
        }
    }

    void dfsAllLcs(int idx, string& lcs, vector<string>& res) {
        NodeT& curr = nodes[idx];
        if (curr.next.empty()) {
            res.push_back(lcs);
            return;
        }

        for (auto it : curr.next) {
            NodeT& next = nodes[it];
            if (next.i <= curr.i || next.j <= curr.j)
                continue;
            lcs.push_back(next.ch);
            dfsAllLcs(it, lcs, res);
            lcs.pop_back();
        }
    }

    void dfsAllDistinctLcs(int idx, string& lcs, unordered_map<string, int>& res) {
        NodeT& curr = nodes[idx];
        if (curr.next.empty()) {
            ++res[lcs];
            return;
        }

        for (auto it : curr.next) {
            NodeT& next = nodes[it];
            if (next.i <= curr.i || next.j <= curr.j)
                continue;
            lcs.push_back(next.ch);
            dfsAllDistinctLcs(it, lcs, res);
            lcs.pop_back();
        }
    }
};
