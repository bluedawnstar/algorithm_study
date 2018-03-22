#pragma once

// A = { (x1, y1), (x2, y2), ..., (xn, yn) }
// distF = [](const pair<int, int>& a, const pair<int, int>& b) {
//      return sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second));
// }
template <typename T, typename U>
U bitonicTourCost(vector<pair<T, T>>& A, const function<U(const pair<T,T>&, const pair<T,T>&)>& distF) {
    sort(A.begin(), A.end());

    int N = (int)A.size();
    vector<vector<U>> dp(N, vector<U>(N, -1));

    dp[0][0] = 0;
    for (int i = 1; i < N; i++)
        dp[0][i] = dp[0][i - 1] + distF(A[i - 1], A[i]);

    for (int i = 1; i < N; i++) {
        for (int j = i; j < N; j++) {
            if (i == j || i == j - 1) {
                U minD = numeric_limits<U>::max();
                for (int k = 0; k < i; k++)
                    minD = min(minD, dp[k][i] + distF(A[k], A[j]));
                dp[i][j] = minD;
            } else {
                dp[i][j] = dp[i][j - 1] + distF(A[j - 1], A[j]);
            }
        }
    }

    return dp[N - 1][N - 1];
}
