#pragma once

template <typename T>
struct MaxWeightedIndependentSet {
    static T getMaxWeightedSum(const vector<vector<int>>& edges, const vector<T>& vertexWeights, int root) {
        vector<vector<T>> dp(edges.size(), vector<T>(2));
        dfsMaxWeightedSum(edges, vertexWeights, dp, root, -1);
        return max(dp[root][false], dp[root][true]);
    }

private:
    static void dfsMaxWeightedSum(const vector<vector<int>>& edges, const vector<T>& vertexWeights,
                                 vector<vector<T>>& dp, int u, int parent) {
        dp[u][true] = vertexWeights[u];
        dp[u][false] = 0;
        for (int v : edges[u]) {
            if (v == parent)
                continue;

            dp[u][true] += dp[v][false];
            dp[u][false] += max(dp[v][false], dp[v][true]);
        }
    }
};

struct MaxIndependentSet {
    static int getLargestSize(const vector<vector<int>>& edges, int root) {
        vector<vector<int>> dp(edges.size(), vector<int>(2));
        dfsLargestSize(edges, dp, root, -1);
        return max(dp[root][false], dp[root][true]);

    }

private:
    static void dfsLargestSize(const vector<vector<int>>& edges, vector<vector<int>>& dp, int u, int parent) {
        dp[u][true] = 1;
        dp[u][false] = 0;
        for (int v : edges[u]) {
            if (v == parent)
                continue;

            dp[u][true] += dp[v][false];
            dp[u][false] += max(dp[v][false], dp[v][true]);
        }
    }
};
