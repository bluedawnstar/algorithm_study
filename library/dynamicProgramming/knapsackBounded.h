#pragma once

///////////////////////////////////////////////////////////////////////////////
// Bounded Knapsack

// https://www.acmicpc.net/problem/12920
struct BoundedKnapsack {
    // O(n*W*logC)
    static int knapsack(const vector<int>& weight, const vector<int>& value, const vector<int>& maxCount, int maxWeight) {
        if (maxWeight <= 0)
            return 0;

        int N = int(weight.size());

        vector<int> w2, v2;
        for (int i = 0; i < N; i++) {
            int cnt = maxCount[i];
            for (int j = 1; cnt > 0; j <<= 1) {
                int t = min(j, cnt);
                w2.emplace_back(weight[i] * t);
                v2.emplace_back(value[i] * t);
                cnt -= t;
            }
        }

        N = int(w2.size());
        vector<int> dp(maxWeight + 1);
        for (int i = 0; i < N; i++) {
            for (int j = maxWeight; j >= w2[i]; j--) {
                dp[j] = max(dp[j], dp[j - w2[i]] + v2[i]);
            }
        }

        return dp[maxWeight];
    }

    static int knapsack2(const vector<int>& w, const vector<int>& v, const vector<int>& c, int K) {
        if (K <= 0)
            return 0;

        int n = int(w.size());

        vector<int> w2, v2;
        for (int i = 0; i < n; i++) {
            int cnt = c[i];
            for (int j = 1; cnt > 0; j <<= 1) {
                int t = min(j, cnt);
                w2.emplace_back(w[i] * t);
                v2.emplace_back(v[i] * t);
                cnt -= t;
            }
        }

        n = int(w2.size());
        vector<int> dp(K + 1);
        for (int i = 0; i < n; i++) {
            for (int j = K; j >= w2[i]; j--) {
                if (j == w2[i] || dp[j - w2[i]] > 0)
                    dp[j] = max(dp[j], dp[j - w2[i]] + v2[i]);
            }
        }

        return *max_element(dp.begin(), dp.end());
    }
};
