#pragma once

// https://brilliant.org/wiki/egg-dropping/
// https://www.geeksforgeeks.org/dynamic-programming-set-11-egg-dropping-puzzle/

// recursive solution
inline int eggDroppingRecursive(int n, int height) {
    if (n == 1)
        return height;
    if (height == 0)
        return 0;
    if (height == 1)
        return 1;

    int res = numeric_limits<int>::max();
    for (int x = 1; x <= height; x++)
        res = min(res, 1 + max(eggDroppingRecursive(n, height - x), eggDroppingRecursive(n - 1, x - 1)));

    return res;
}

// DP solution, O(N*H^2)
inline int eggDroppingDP(int n, int height) {
    vector<vector<int>> dp(n + 1, vector<int>(height + 1));

    for (int i = 0; i <= height; i++)
        dp[0][i] = 0;
    for (int i = 0; i <= height; i++)
        dp[1][i] = i;
    for (int j = 0; j <= n; j++)
        dp[j][0] = 0;

    for (int i = 2; i <= n; i++) {
        for (int j = 1; j <= height; j++) {
            int minCnt = numeric_limits<int>::max();
            for (int x = 1; x <= j; x++)
                minCnt = min(minCnt, 1 + max(dp[i][j - x], dp[i - 1][x - 1]));

            dp[i][j] = minCnt;
        }
    }

    return dp[n][height];
}

// Binary search, O(N*logH)
inline int eggDroppingBinarySearch(int n, int height) {
    // binomial
    auto possible = [](int x, int n, int h) {
        long long ans = 0;
        long long C = 1;

        //   SUM { C(x, i) }
        // 1<=i<=n
        for (int i = 1; i <= n; i++) {
            C = C * x-- / i;
            ans += C;
            if (ans >= h)
                return true;
        }

        return false;
    };

    int lo = 0, hi = height;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (possible(mid, n, height))
            hi = mid - 1;
        else
            lo = mid + 1;
    }

    return lo;
}
