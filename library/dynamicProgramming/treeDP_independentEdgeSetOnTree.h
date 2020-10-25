#pragma once

// independent edge set (matching) on a tree
template <int mod = 1'000'000'007>
struct IndependentEdgeSetOnTree {
    int N;
    vector<vector<pair<int, int>>> tree;    // (v, edge-index)
    vector<pair<int, int>> edges;           // { (u, v), ... }

    IndependentEdgeSetOnTree() : N(0) {
    }

    explicit IndependentEdgeSetOnTree(int n) {
        init(n);
    }

    void init(int n) {
        this->N = n;
        tree = vector<vector<pair<int, int>>>(n);
        edges = vector<pair<int, int>>();
        edges.reserve(n - 1);
    }

    void add(int u, int v) {
        int edgeIndex = int(edges.size());
        tree[u].emplace_back(v, edgeIndex);
        tree[v].emplace_back(u, edgeIndex);
        edges.emplace_back(u, v);
    }

    //---

    // [edge-index] = the number of independent edge set include edges[edge-index] 
    vector<int> countIndependentEdgeSetByEachEdgeUsed() {
        dp = vector<pair<int, int>>(N);
        answer = vector<int>(N - 1);

        dfs(0, -1);
        dfs2(0, -1, 0, 0);

        return answer;
    }

private:
    vector<pair<int, int>> dp;          // [...] = (including u, excluding u)
    vector<int> answer;

    void dfs(int u, int parent) {
        vector<pair<int, int>> cnt;     // [v] = count

        for (auto& it : tree[u]) {
            if (it.first == parent)
                continue;
            dfs(it.first, u);
            cnt.emplace_back(it.first, dp[it.first].second + 1);
        }

        vector<int> prefix(cnt.size() + 1);
        vector<int> suffix(cnt.size() + 1);
        prefix[0] = 1;
        suffix[cnt.size()] = 1;
        for (int i = 1, j = int(cnt.size() - 1); i <= int(cnt.size()); i++, j--) {
            prefix[i] = int(1ll * prefix[i - 1] * cnt[i - 1].second % mod);
            suffix[j] = int(1ll * suffix[j + 1] * cnt[j].second % mod);
        }
        for (int i = 0; i < int(cnt.size()); i++)
            dp[u].second = int((dp[u].second + 1ll * prefix[i] * suffix[i + 1] % mod * dp[cnt[i].first].first) % mod);

        dp[u].second += prefix.back() - 1;
        if (dp[u].second < 0)
            dp[u].second += mod;
        else if (dp[u].second >= mod)
            dp[u].second -= mod;

        dp[u].first = prefix.back();
    }

    void dfs2(int u, int parent, int prevUsed, int prevUnused) {
        vector<int> cnt;
        for (auto& it : tree[u]) {
            if (it.first == parent)
                continue;
            cnt.emplace_back(dp[it.first].second + 1);
        }

        vector<int> prefix(cnt.size() + 1);
        vector<int> suffix(cnt.size() + 1);
        prefix[0] = prevUnused + 1;
        suffix[cnt.size()] = 1;
        for (int i = 1, j = int(cnt.size() - 1); i <= int(cnt.size()); i++, j--) {
            prefix[i] = int(1ll * prefix[i - 1] * cnt[i - 1] % mod);
            suffix[j] = int(1ll * suffix[j + 1] * cnt[j] % mod);
        }

        int i = 0;
        int totalNextUnused = int(1ll * prevUsed * suffix[0] % mod);
        for (auto& it : tree[u]) {
            if (it.first == parent)
                continue;
            int v = it.first;
            int eidx = it.second;

            answer[eidx] = int(dp[v].first * (1ll * prefix[i] * suffix[i + 1] % mod) % mod);

            totalNextUnused += answer[eidx];
            if (totalNextUnused >= mod)
                totalNextUnused -= mod;

            i++;
        }

        i = 0;
        for (auto& it : tree[u]) {
            if (it.first == parent)
                continue;
            int v = it.first;
            int eidx = it.second;

            int nextUsed = int(1ll * prefix[i] * suffix[i + 1] % mod);

            // (totalNextUnused - prefix.back() / (dp[v].second + 1) * dp[v].first) / (dp[v].second + 1) + prefix.back() / (dp[v].second + 1) - 1
            int nextUnused = int(totalNextUnused - 1ll * nextUsed * dp[v].first % mod);
            if (nextUnused < 0)
                nextUnused += mod;

            nextUnused = int(1ll * nextUnused * modInv(dp[v].second + 1) % mod + nextUsed - 1);
            if (nextUnused < 0)
                nextUnused += mod;
            else if (nextUnused >= mod)
                nextUnused -= mod;

            dfs2(v, u, nextUsed, nextUnused);
            i++;
        }
    }

    static int modPow(int x, int n) {
        if (n == 0)
            return 1;

        long long t = x % mod;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % mod;
            t = t * t % mod;
        }
        return int(res);
    }

    static int modInv(int x) {
        return modPow(x, mod - 2);
    }
};
