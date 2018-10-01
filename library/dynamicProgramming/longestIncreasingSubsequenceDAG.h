#pragma once

template <typename T>
struct LisDAG {
    struct NodeT {
        vector<int> next;
        int index;          // index in original data
        long long count;    // the number of LIS in sub tree
    };

    int N;
    vector<T> value;

    int root;
    int length;
    vector<NodeT> tree;

    LisDAG() {
    }

    LisDAG(const T v[], int n) {
        build(v, n);
    }

    explicit LisDAG(const vector<T>& v) {
        build(v);
    }

    // worst case O(N^2), but normally O(NlogN)
    void build(const T v[], int n) {
        value.assign(v, v + n);

        vector<vector<pair<int, int>>> dp;  // dp[i][j] = (index of character in v, previous index in dp[i - 1])
        dp.reserve(n);

        vector<T> lis;
        lis.reserve(n);

        if (n > 0) {
            dp.push_back(vector<pair<int, int>>{ { 0, -1 } });
            lis.push_back(v[0]);

            for (int i = 1; i < n; i++) {
                if (lis.back() < v[i]) {
                    dp.push_back(vector<pair<int, int>>{ { i, int(dp.back().size()) - 1 } });
                    lis.push_back(v[i]);
                } else {
                    int it = int(lower_bound(lis.begin(), lis.end(), v[i]) - lis.begin());
                    dp[it].emplace_back(i, (it > 0) ? int(dp[it - 1].size()) - 1 : -1);
                    lis[it] = v[i];
                }
            }
        }

        // build tree

        root = 0;
        length = int(lis.size());
        tree = vector<NodeT>(n + 1);
        for (int i = 0; i <= n; i++) {
            tree[i].index = i - 1;
            tree[i].count = 0;
        }

        for (auto& it : dp.back())
            tree[it.first + 1].count = 1;

        for (int i = int(lis.size()) - 1; i >= 0; i--) {
            for (int j = int(dp[i].size()) - 1; j >= 0; j--) {
                if (i == 0) {
                    int b = dp[i][j].first;
                    if (tree[b + 1].count > 0) {
                        tree[0].count += tree[b + 1].count;
                        tree[0].next.push_back(b + 1);
                    }
                } else {
                    for (int k = dp[i][j].second; k >= 0; k--) {
                        int a = dp[i - 1][k].first;
                        int b = dp[i][j].first;
                        if (v[a] >= v[b])
                            break;
                        if (tree[b + 1].count > 0) {
                            tree[a + 1].count += tree[b + 1].count;
                            tree[a + 1].next.push_back(b + 1);
                        }
                    }
                }
            }
        }
    }

    void build(const vector<T>& v) {
        build(v.data(), int(v.size()));
    }

    //---

    int getLength() const {
        return length;
    }

    long long getTotalCount() const {
        return tree[0].count;
    }

    // kth >= 0, O(N)
    vector<T> getKth(long long kth) const {
        vector<T> res;
        if (0 <= kth && kth < tree[0].count) {
            getKth(res, kth + 1, 0);
            reverse(res.begin(), res.end());
        }
        return res;
    }

    template <typename FuncT = function<void(int, const vector<T>&)>>
    void iterateAll(const FuncT& f) const {
        vector<T> vec;
        dfsIterate(0, vec, f);
    }

    vector<vector<T>> findAll() const {
        vector<vector<T>> res;
        iterateAll([&res](int u, const vector<T>& vec) {
            res.push_back(vec);
        });
        return res;
    }

private:
    // kth >= 1
    bool getKth(vector<T>& res, long long kth, int u) const {
        for (int v : tree[u].next) {
            if (tree[v].count < kth) {
                kth -= tree[v].count;
                continue;
            }

            if (getKth(res, kth, v)) {
                res.push_back(value[v - 1]);
                break;
            }
        }
        return true;
    }

    template <typename FuncT = function<void(int, const vector<T>&)>>
    void dfsIterate(int u, vector<T>& vec, const FuncT& f) const {
        if (tree[u].next.empty()) {
            f(u, vec);
            return;
        }
        for (auto v : tree[u].next) {
            vec.push_back(value[v - 1]);
            dfsIterate(v, vec, f);
            vec.pop_back();
        }
    }
};
