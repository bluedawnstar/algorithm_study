#pragma once

/*
    https://www.codechef.com/problems/CHGORAM2
    https://discuss.codechef.com/t/chgoram2-editorial/53535

    Find the number of triples of distinct nodes such that all three nodes are marked and
      all three pairs of nodes within the triple have the same distance.

    1. property

                u
              /   \
            ...   ...
            /       \
          leg        v
                    /  \
                  ...  ...
                  /      \
                fork    fork

        a = u-leg, b = u-v, d = v-fork

            d = a + b
        <=> a = d - b

    2. solution
        dp1[u][i] = the number of (i == a)
        dp2[u][i] = the number of (i == d - b)


                u
              /   \
            ...    v
            /     /  \ 
          leg   ...  ...    
                /      \
              fork    fork

        For each direct child v of node u,
         
        1) update v's dp tables
          - increase depth of dp1 by 1
          - decrease depth of dp2 by 1, because 'b' is increased by 1

            dp1[v][i + 1] = dp1[v][i]
            dp2[v][i - 1] = dp2[v][i]

        2) update answer

            ans += SUM (dp1[u][i] * dp2[v][i] + dp2[u][i] * dp1[v][i])
                   i=1

        3) merge v's dp tables to u's
          - make new forks
            dp2[u][i] += dp1[u][i] * dp1[v][i]

          - directly merge v's tables to u's
            dp2[u][i] += dp2[v][i]
            dp1[u][i] += dp1[v][i]
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
    vector<vector<long long>> dp1;  // leg, reversed
    vector<vector<long long>> dp2;  // fork, reversed

    long long dfsSolve(int u, int parent) {
        long long res = 0;

        dp1[u] = { marks[u] };      // leg
        dp2[u] = { 0 };             // fork
        for (int v : edges[u]) {
            if (v == parent)
                continue;

            res += dfsSolve(v, u);

            dp1[v].push_back(0);    // increase 'a' by 1
            dp2[v].pop_back();      // decrease 'd - b' by 1, because 'b' is increased by 1

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

            //--- update answer

            // leg from v, fork from u
            for (int i = 1; i <= int(dp1[v].size()); i++)
                res += dp1[v][dp1[v].size() - i] * dp2[u][dp2[u].size() - i];
            // fork from v, leg from u
            for (int i = 1; i <= int(dp2[v].size()); i++)
                res += dp2[v][dp2[v].size() - i] * dp1[u][dp1[u].size() - i];

            //--- combine dp tables

            // new forks
            for (int i = 1; i <= int(dp1[v].size()); i++)
                dp2[u][dp2[u].size() - i] += dp1[u][dp1[u].size() - i] * dp1[v][dp1[v].size() - i];
            // add v's tables to u's
            for (int i = 1; i <= int(dp1[v].size()); i++)
                dp1[u][dp1[u].size() - i] += dp1[v][dp1[v].size() - i];
            for (int i = 1; i <= int(dp2[v].size()); i++)
                dp2[u][dp2[u].size() - i] += dp2[v][dp2[v].size() - i];
        }
        return res;
    }
};
