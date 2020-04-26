#pragma once

/*
    https://www.codechef.com/problems/CHGORAM2
    https://discuss.codechef.com/t/chgoram2-editorial/53535

    Find the number of triples of distinct nodes such that all three nodes are marked and
      all three pairs of nodes within the triple have the same distance.
*/
struct SameDistanceTripleCounterOnTree {
    int N;
    vector<vector<int>> edges;
    vector<bool> marks;                     // usable marks

    SameDistanceTripleCounterOnTree() {
    }

    explicit SameDistanceTripleCounterOnTree(int n) {
        init(n);
    }

    void init(int n) {
        N = n;
        edges = vector<vector<int>>(n);
        marks = vector<bool>(n);
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    void setMark(int u) {
        marks[u] = true;
    }

    //---

    // O(N)
    long long solve() {
        dp1 = vector<vector<long long>>(N);
        dp2 = vector<vector<long long>>(N);
        return dfsSolve(0, -1) * 6ll;
    }

private:
    vector<vector<long long>> dp1;  // leg
    vector<vector<long long>> dp2;  // fork

    long long dfsSolve(int u, int parent) {
        long long res = 0;

        dp1[u] = { marks[u] };      // leg
        dp2[u] = { 0 };             // fork
        for (int v : edges[u]) {
            if (v == parent)
                continue;

            res += dfsSolve(v, u);

            // increase the depths by 1
            dp1[v].push_back(0);
            dp2[v].pop_back();
            // if v is deeper, switch the dp to make it faster
            if (dp1[v].size() > dp1[u].size()) {
                swap(dp1[u], dp1[v]);
                swap(dp2[u], dp2[v]);
            }
            if (dp2[u].size() < dp1[v].size()) {
                // pad dp2[u] with 0s
                vector<int> zeroes(dp1[v].size() - dp2[u].size(), 0);
                dp2[u].insert(dp2[u].begin(), zeroes.begin(), zeroes.end());
            }

            //--- update ans
            // leg from v, fork from u
            for (int i = 1; i <= int(dp1[v].size()); i++)
                res += dp1[v][dp1[v].size() - i] * dp2[u][dp2[u].size() - i];
            // fork from v, leg from u
            for (int i = 1; i <= int(dp2[v].size()); i++)
                res += dp2[v][dp2[v].size() - i] * dp1[u][dp1[u].size() - i];

            //--- combine the dp
            // new forks by combining 2 legs
            for (int i = 1; i <= int(dp1[v].size()); i++)
                dp2[u][dp2[u].size() - i] += dp1[u][dp1[u].size() - i] * dp1[v][dp1[v].size() - i];
            // just add the rest together
            for (int i = 1; i <= int(dp1[v].size()); i++)
                dp1[u][dp1[u].size() - i] += dp1[v][dp1[v].size() - i];
            for (int i = 1; i <= int(dp2[v].size()); i++)
                dp2[u][dp2[u].size() - i] += dp2[v][dp2[v].size() - i];
        }
        return res;
    }
};
