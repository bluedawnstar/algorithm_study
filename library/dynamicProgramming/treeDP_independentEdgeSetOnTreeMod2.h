#pragma once

// https://www.hackerearth.com/problem/algorithm/two-papers-i-833f0ea7/

// odd or even
struct IndependentEdgeSetOnTreeMod2 {
    int N;
    vector<vector<pair<int, int>>> tree;    // (v, edge-index)
    vector<pair<int, int>> edges;           // { (u, v), ... }

    IndependentEdgeSetOnTreeMod2() : N(0) {
    }

    explicit IndependentEdgeSetOnTreeMod2(int n) {
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
    // x in { 0, 1, 2 }
    struct Int2 {
        int pow2;
        bool zero;

        Int2() {
            pow2 = 0;
            zero = true;
        }

        explicit Int2(int x) {
            operator =(x);
        }

        Int2(const Int2& rhs) {
            pow2 = rhs.pow2;
            zero = rhs.zero;
        }

        Int2& operator =(int x) {
            if (x == 2) {
                pow2 = 1;
                zero = false;
            } else if (x == 1) {
                pow2 = 0;
                zero = false;
            } else {
                pow2 = 0;
                zero = true;
            }

            return *this;
        }

        Int2& operator =(const Int2& rhs) {
            if (this != &rhs) {
                pow2 = rhs.pow2;
                zero = rhs.zero;
            }
            return *this;
        }


        Int2& operator *=(int x) {
            if (x == 2) {
                if (!zero)
                    pow2++;
            } else if (x == 0) {
                pow2 = 0;
                zero = true;
            }
            return *this;
        }

        Int2 operator *(int x) const {
            Int2 res = *this;
            res *= x;
            return res;
        }

        Int2& operator /=(int x) {
            if (x == 2 && !zero) {
                if (pow2 > 0)
                    --pow2;
            }
            return *this;
        }

        Int2 operator /(int x) const {
            Int2 res = *this;
            res /= x;
            return res;
        }


        int mod2() const {
            if (zero || pow2 >= 1)
                return 0;
            return 1;
        }

        int mod4() const {
            if (zero || pow2 >= 2)
                return 0;
            return 1 << pow2;
        }
    };

    vector<pair<int, int>> dp;          // [...] = (including u, excluding u)
    vector<int> answer;

    void dfs(int u, int parent) {
        vector<pair<int, int>> cnt;     // [v] = count

        for (auto& it : tree[u]) {
            if (it.first == parent)
                continue;
            dfs(it.first, u);
            cnt.emplace_back(it.first, (dp[it.first].second + 1) & 1);
        }

        vector<int> prefix(cnt.size() + 1);
        vector<int> suffix(cnt.size() + 1);
        prefix[0] = 1;
        suffix[cnt.size()] = 1;
        for (int i = 1, j = int(cnt.size() - 1); i <= int(cnt.size()); i++, j--) {
            prefix[i] = (prefix[i - 1] * cnt[i - 1].second) & 1;
            suffix[j] = (suffix[j + 1] * cnt[j].second) & 1;
        }
        for (int i = 0; i < int(cnt.size()); i++)
            dp[u].second = (dp[u].second + prefix[i] * suffix[i + 1] * dp[cnt[i].first].first) & 1;

        dp[u].second = (dp[u].second + prefix.back() - 1) & 1;
        dp[u].first = prefix.back() & 1;
    }

    void dfs2(int u, int parent, int prevUsed, int prevUnused) {
        int res = 0;

        Int2 currUnused(prevUnused + 1);
        for (auto& it : tree[u]) {
            if (it.first == parent)
                continue;
            currUnused *= (dp[it.first].second + 1);
        }

        long long nextTotalUnused = (currUnused / (prevUnused + 1) * prevUsed).mod4();
        for (auto& it : tree[u]) {
            if (it.first == parent)
                continue;
            nextTotalUnused += (currUnused / (dp[it.first].second + 1) * dp[it.first].first).mod4();
        }

        for (auto& it : tree[u]) {
            if (it.first == parent)
                continue;
            int v = it.first;
            int eidx = it.second;

            int nextUsed = (currUnused / (dp[v].second + 1)).mod4();
            answer[eidx] = (dp[v].first * nextUsed) & 1;

            // (totalNextUnused - currUnused / (dp[v].second + 1) * dp[v].first + currUnused) / (dp[v].second + 1) - 1
            long long nextUnused = nextTotalUnused - nextUsed * dp[v].first + currUnused.mod4();
            dfs2(v, u, nextUsed & 1, int((nextUnused / (dp[v].second + 1) - 1) & 1));
        }
    }
};
