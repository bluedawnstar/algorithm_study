#pragma once

// https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iv/
// https://www.geeksforgeeks.org/maximum-profit-by-buying-and-selling-a-share-at-most-k-times/
/*
    No multiple transactions at the same time

    - dp[hold + sell][sell]

                         buy
    --> [hold=0, sell=0] --> [hold=1, sell=0]
                                    |sell
                                    V         buy
                             [hold=0, sell=1] --> [hold=1, sell=1]
                                                         |sell
                                                         V
                                                  [hold=0, sell=2] --> ...
*/
// result[i] = max profit with at most (i + 1) transactions, O(KN)
template <typename T>
vector<T> maxProfit(const vector<T>& prices, int maxK) {
    int N = int(prices.size());
    if (maxK <= 0 || N < 2)
        return vector<T>{};

    vector<T> res;

    vector<T> dp(N);
    for (int i = 0; i < maxK; i++) {
        T bestBuy = -prices[0];
        for (int j = 1; j < N; j++) {
            T sellProfit = bestBuy + prices[j];
            bestBuy = max(bestBuy, dp[j] - prices[j]);
            dp[j] = max(dp[j - 1], sellProfit);
        }
        res.push_back(dp.back());
    }

    return res;
}

// No multiple transactions at the same time
// result[i] = max profit with at most (i + 1) transactions, O((N + K)*logN)
template <typename T>
vector<T> maxProfitFast(const vector<T>& prices, int maxK) {
    int N = int(prices.size());
    if (maxK <= 0 || N < 2)
        return vector<T>{};

    priority_queue<T> profits;
    stack<pair<int, int>> vps;  // (valley, peak)
    for (T v, p = 0; p < N; ) {
        // find next valley/peak pair
        for (v = p; v < N - 1 && prices[v] >= prices[v + 1]; v++)
            ;
        for (p = v + 1; p < N && prices[p] >= prices[p - 1]; p++)
            ;
        // save profit of 1 transaction at last v/p pair, if current v is lower than last v
        while (!vps.empty() && prices[v] < prices[vps.top().first]) {
            profits.push(prices[vps.top().second-1] - prices[vps.top().first]);
            vps.pop();
        }
        // save profit difference between 1 transaction (last v and current p) and 2 transactions (last v/p + current v/p),
        // if current v is higher than last v and current p is higher than last p
        while (!vps.empty() && prices[p - 1] >= prices[vps.top().second - 1]) {
            profits.push(prices[vps.top().second - 1] - prices[v]);
            v = vps.top().first;
            vps.pop();
        }
        vps.push(pair<int, int>(v, p));
    }

    // save profits of the rest v/p pairs
    while (!vps.empty()) {
        profits.push(prices[vps.top().second-1] - prices[vps.top().first]);
        vps.pop();
    }

    vector<T> res;
    res.reserve(maxK);

    T ans = 0;
    for (int i = 0; i < maxK; i++) {
        if (!profits.empty()) {
            ans += profits.top();
            profits.pop();
        }
        res.push_back(ans);
    }

    return res;
}


// https://www.codechef.com/problems/PBOXES
/*
    With multiple transactions at the same time

    - dp[hold + sell][sell]

                         buy                  buy
    --> [hold=0, sell=0] --> [hold=1, sell=0] --> [hold=2, sell=0] --> ...
                                    |sell                |sell
                                    V         buy        V
                             [hold=0, sell=1] --> [hold=1, sell=1] --> ...
                                                         |sell
                                                         V
                                                  [hold=0, sell=2] --> ...
*/
// result[i] = max profit with at most (i + 1) transactions, O(N*K^2)
template <typename T>
vector<T> maxProfitWithMultipleTransactions(const vector<T>& prices, int maxK) {
    int N = int(prices.size());
    if (maxK <= 0 || N < 2)
        return vector<T>{};

    const T INF = numeric_limits<T>::max() / 2;

    // dp[i][i] = max profit with exact i transactions
    vector<vector<T>> dp(maxK + 1, vector<T>(maxK + 1, -INF));    //  [hold + sell][sell]
    dp[0][0] = 0;
    for (int i = 0; i < N; i++) {
        T x = prices[i];
        for (int j = maxK; j > 0; j--) {
            if (dp[j - 1][0] == -INF)
                continue;

            for (int k = j; k > 0; k--) {
                dp[j][k] = max(dp[j][k], dp[j][k - 1] + x);
                dp[j][k] = max(dp[j][k], dp[j - 1][k] - x);
            }
            dp[j][0] = max(dp[j][0], dp[j - 1][0] - x);
        }
    }

    vector<T> res;

    T ans = 0;
    for (int i = 1; i <= maxK; i++) {
        ans = max(ans, dp[i][i]);
        res.push_back(ans);
    }

    return res;
}
