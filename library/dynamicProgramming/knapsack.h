#pragma once

// see also "powerSumOfAllCombinations_NTT.h

///////////////////////////////////////////////////////////////////////////////
// 0-1 Knapsack Problem

// returns the maximum value that can be put in a knapsack of capacity maxWeight, O(n*W)
inline int knapsackNaive(const vector<int>& weight, const vector<int>& value, int maxWeight) {
    int N = int(weight.size());

    vector<vector<int>> dp(N + 1, vector<int>(maxWeight + 1));
    for (int i = 1; i <= N; i++) {
        int v = value[i - 1];
        int w = weight[i - 1];
        for (int j = 1; j <= maxWeight; j++) {
            if (j >= w)
                dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - w] + v);
            else
                dp[i][j] = dp[i - 1][j];
        }
    }

    return dp[N][maxWeight];
}

inline int knapsack(const vector<int>& weight, const vector<int>& value, int maxWeight) {
    if (maxWeight <= 0)
        return 0;

    int N = int(weight.size());

    vector<int> dp(maxWeight + 1);
    for (int i = 0; i < N; i++) {
        int v = value[i];
        int w = weight[i];
        for (int j = maxWeight; j >= w;  j--) {
            dp[j] = max(dp[j], dp[j - w] + v);
        }
    }

    return dp[maxWeight];
}

// maxValue <<< maxWeight
// O(n^2*maxValue)
inline int knapsack(const vector<int>& weight, const vector<int>& value, int maxWeight, int maxValue) {
    if (maxWeight <= 0)
        return 0;

    const int INF = 0x3f3f3f3f;
    int N = int(weight.size());

    vector<int> dp(N * maxValue + 1, INF);
    dp[0] = 0;
    for (int i = 0; i < N; i++) {
        int v = value[i];
        int w = weight[i];
        for (int j = N * maxValue; j >= v; j--) {
            dp[j] = min(dp[j], dp[j - v] + w);
        }
    }

    for (int i = N * maxValue; i > 0; i--) {
        if (dp[i] <= maxWeight)
            return i;
    }

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
// Unbounded Knapsack

inline int unboundedKnapsack(const vector<int>& weight, const vector<int>& value, int maxWeight) {
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
