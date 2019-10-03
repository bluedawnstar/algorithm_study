#pragma once

//-----------------------------------------------------------------------------
// https://www.hackerearth.com/practice/algorithms/dynamic-programming/2-dimensional/practice-problems/algorithm/goodset-d526c2cb/description/
// the number of ordered sets of integers which is said to be a special set
//   if for every element X of the set, the set does not contain the element X + 1. 
template <int mod = 1000000007>
struct SpecialSetCounterMod {
    /*
        dp[i][j] = the number of sequence
            i = the number of elements in a sequence, j = right index in a range (array[0..j])

        dp[0][j] = j + 1
        dp[i][j] = SUM dp[i - 1][k] = dp[i][j - 1] + dp[i - 1][j - 2], (j >= 2)
                 0<=k<=j-2

        ans = SUM dp[i][N - 1] * (i + 1)!
            0<=i<(N+1)/2

        O(N^2)
    */
    static long long count(int N) {
        vector<long long> factorial(N + 1);

        factorial[0] = factorial[1] = 1;
        for (int i = 2; i <= N; i++)
            factorial[i] = factorial[i - 1] * i % mod;

        int half = (N + 1) / 2;
        vector<vector<long long>> dp(half, vector<long long>(N));

        for (int j = 0; j < N; j++)
            dp[0][j] = j + 1;
        for (int i = 1; i < half; i++) {
            for (int j = 2; j < N; j++)
                dp[i][j] = (dp[i][j - 1] + dp[i - 1][j - 2]) % mod;
        }

        long long ans = 0;
        for (int i = 0; i < half; i++)
            ans = (ans + dp[i][N - 1] * factorial[i + 1]) % mod;

        return ans;
    }

};