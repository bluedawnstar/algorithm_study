#pragma once

///////////////////////////////////////////////////////////////////////////////
// Unbounded Knapsack

struct UnboundedKnapsack {
    static int knapsack(const vector<int>& weight, const vector<int>& value, int maxWeight) {
        if (maxWeight <= 0)
            return 0;

        int N = int(weight.size());

        vector<int> dp(maxWeight + 1);
        for (int i = 0; i < N; i++) {
            int v = value[i];
            int w = weight[i];
            for (int j = w; j <= maxWeight; j++) {
                dp[j] = max(dp[j], dp[j - w] + v);
            }
        }

        return dp[maxWeight];
    }

    static int knapsack2(const vector<int>& weight, const vector<int>& value, int maxWeight) {
        if (maxWeight <= 0)
            return 0;

        int N = int(weight.size());

        vector<int> dp(maxWeight + 1);
        for (int i = 0; i < N; i++) {
            int v = value[i];
            int w = weight[i];
            for (int j = w; j <= maxWeight; j++) {
                if (j == w || dp[j - w] > 0)
                    dp[j] = max(dp[j], dp[j - w] + v);
            }
        }

        return *max_element(dp.begin(), dp.end());
    }
};
