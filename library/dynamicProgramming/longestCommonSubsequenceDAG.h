#pragma once

struct LcsDAG {
    struct NodeT {
        vector<int> next;
        int i, j;               // s1[i], s2[j]
        char ch;
        long long count;
    };


    int lcsLength;
    vector<NodeT> nodes;    // root is 0

    LcsDAG() {
    }

    LcsDAG(const string& s1, const string& s2) {
        build(s1, s2);
    }

    // O(N^2)
    void build(const char s1[], int n, const char s2[], int m) {
        nodes.clear();
        nodes.reserve(min(n, m) + 1);
        nodes.push_back(NodeT{ vector<int>{}, -1, -1, 0, 0ll });

        vector<vector<int>> dp(n + 1, vector<int>(m + 1));
        vector<vector<pair<int, int>>> match(min(n, m) + 1);    // match[i][j] = (index in nodes, previous index in match[i - 1])
        match[0].emplace_back(0, -1);
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (s1[i - 1] == s2[j - 1]) {
                    int k = dp[i - 1][j - 1] + 1;
                    dp[i][j] = k;
                    match[k].emplace_back(int(nodes.size()), int(match[k - 1].size()) - 1);
                    nodes.push_back(NodeT{ vector<int>{}, i - 1, j - 1, s1[i - 1], 0ll });
                } else {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        lcsLength = dp[n][m];
        if (lcsLength <= 0)
            return;

        // build DAG

        for (auto& it : match[lcsLength])
            nodes[it.first].count = 1;

        for (int i = lcsLength; i > 0; i--) {
            for (int j = int(match[i].size()) - 1; j >= 0; j--) {
                int b = match[i][j].first;
                if (nodes[b].count <= 0)
                    continue;

                for (int k = match[i][j].second; k >= 0; k--) {
                    int a = match[i - 1][k].first;
                    if (nodes[a].i >= nodes[b].i || nodes[a].j >= nodes[b].j)
                        continue;
                    nodes[a].count += nodes[b].count;
                    nodes[a].next.push_back(b);
                }
            }
        }
    }

    void build(const string& s1, const string& s2) {
        int n = int(s1.length());
        int m = int(s2.length());
        build(s1.data(), n, s2.data(), m);
    }

    //---

    int getLength() const {
        return lcsLength;
    }

    long long getTotalCount() const {
        return nodes[0].count;
    }

    // kth >= 0, O(N)
    string getKth(long long kth) const {
        string res;
        if (0 <= kth && kth < nodes[0].count)
            getKth(res, kth + 1, 0);
        return res;
    }

    template <typename FuncT = function<void(int, const string&)>>
    void iterateAll(const FuncT& f) const {
        string s;
        dfsIterate(0, s, f);
    }

    vector<string> findAll() const {
        vector<string> res;
        iterateAll([&res](int u, const string& s) {
            res.push_back(s);
        });
        return res;
    }

    unordered_map<string, int> findAllDistinct() const {
        unordered_map<string, int> res;
        iterateAll([&res](int u, const string& s) {
            ++res[s];
        });
        return res;
    }

private:
    // kth >= 1
    void getKth(string& res, long long kth, int u) const {
        for (int v : nodes[u].next) {
            if (nodes[v].count < kth) {
                kth -= nodes[v].count;
            } else {
                res.push_back(nodes[v].ch);
                getKth(res, kth, v);
                break;
            }
        }
    }

    template <typename FuncT = function<void(int, const string&)>>
    void dfsIterate(int u, string& s, const FuncT& f) const {
        if (nodes[u].next.empty()) {
            f(u, s);
            return;
        }
        for (auto v : nodes[u].next) {
            s.push_back(nodes[v].ch);
            dfsIterate(v, s, f);
            s.pop_back();
        }
    }
};
