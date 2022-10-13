#pragma once

/*
    https://www.hackerearth.com/problem/algorithm/collect-maximum-coins-57aa70cd/

    - move to an adjacent node with 1 cost
    - pay cost[i] to get gain[i]
    
*/
template <typename T>
struct TreeTraversalKnapsack {
    int N;
    vector<vector<int>> edges;

    TreeTraversalKnapsack() : N(0) {
    }

    explicit TreeTraversalKnapsack(int n) {
        init(n);
    }

    void init(int n) {
        N = n;
        edges = vector<vector<int>>(n);
    }

    void add(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }


    // start in node 0, O(N*K^2)
    int knapsack(const vector<int>& cost, const vector<T>& gain, int maxCost) {
        this->cost = cost;
        this->gain = gain;
        dp = vector<vector<array<T, 2>>>(N, vector<array<T, 2>>(maxCost + 1));

        dfs(maxCost, 0, -1);

        int ans = 0;
        for (int i = 0; i <= maxCost; i++) {
            ans = max(ans, max(dp[0][i][kOneWay], dp[0][i][kRoundTrip]));
        }

        return ans;
    }

private:
    vector<int> cost;
    vector<T> gain;

    enum {
        kOneWay,   // if we start traversing from V, traverse the subtree of V and come back to V at the end of traversal
        kRoundTrip // if we start traversing from V, traverse the subtree of V and don't have to come back to V at the end of traversal
    };
    vector<vector<array<T, 2>>> dp; // dp[starting_node][cost_sum][kOneWay|kRoundTrip] = max_gain_sum

    void updateMax(T& y, T x) {
        y = max(y, x);
    }

    void dfs(int maxCost, int u, int parent) {
        for (auto v : edges[u]) {
            if (v == parent)
                continue;

            dfs(maxCost, v, u);
            for (int i = maxCost; i >= 0; i--) {
                for (int j = maxCost - i - 1; j >= 0; j--) {
                    if (i + j + 2 <= maxCost) {
                        // U_round_trip = U_round_trip + V_round_trip
                        updateMax(dp[u][i + j + 2][kRoundTrip], dp[u][i][kRoundTrip] + dp[v][j][kRoundTrip]);
                        // U_one_way = U_one_way + V_round_trip
                        updateMax(dp[u][i + j + 2][kOneWay], dp[u][i][kOneWay] + dp[v][j][kRoundTrip]);
                    }
                    if (i + j + 1 <= maxCost) {
                        // U_one_way = U_round_trip + V_one_way
                        updateMax(dp[u][i + j + 1][kOneWay], dp[u][i][kRoundTrip] + dp[v][j][kOneWay]);
                    }
                }
            }
        }
        // add cost & gain of the node u
        for (int i = maxCost - cost[u]; i >= 0; i--) {
            updateMax(dp[u][i + cost[u]][kOneWay], dp[u][i][kOneWay] + gain[u]);
            updateMax(dp[u][i + cost[u]][kRoundTrip], dp[u][i][kRoundTrip] + gain[u]);
        }
    }
};
