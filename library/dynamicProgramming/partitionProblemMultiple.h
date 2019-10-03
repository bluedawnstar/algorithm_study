#pragma once

// 1. the number of partitions of an array such that the sum of elements in each partition is in [minSum, maxSum]
// https://www.hackerearth.com/practice/algorithms/searching/binary-search/practice-problems/algorithm/partitions-5fd40ffc/description/
struct MultiplePartitionProblem {
    /*
        minSum <= sum[R] - sum[L - 1] <= maxSum
        minSum - sum[R] <= -sum[L - 1] <= maxSum - sum[R]
        sum[R] - minSum >= sum[L - 1] >= sum[R] - maxSum
    */
    // O(n*logn)
    template <typename T>
    static long long countPartitions(const T v[], int N, long long minSum, long long maxSum) {
        vector<long long> sum;
        vector<long long> sumDP(N + 1);

        sum.reserve(N + 1);

        long long sumR = 0, cnt = 0;
        for(int i = 0; i < N; i++){
            sumR += v[i];

            auto start = lower_bound(sum.begin(), sum.end(), sumR - maxSum) - sum.begin();
            auto end = upper_bound(sum.begin(), sum.end(), sumR - minSum) - sum.begin();

            cnt = sumDP[end] - sumDP[start];
            if (minSum <= sumR && sumR <= maxSum)
                cnt++;

            sumDP[i + 1] = sumDP[i] + cnt;
            sum.push_back(sumR);
        }

        return cnt;
    }

    template <typename T>
    static long long countPartitions(const vector<T>& v, long long minSum, long long maxSum) {
        return countPartitions(v.data(), int(v.size()), minSum, maxSum);
    }

    // O(n*logn)
    template <typename T>
    static long long countPartitionsMod(const T v[], int N, long long minSum, long long maxSum, T MOD) {
        vector<long long> sum;
        vector<long long> sumDP(N + 1);

        sum.reserve(N + 1);

        long long sumR = 0, cnt = 0;
        for(int i = 0; i < N; i++){
            sumR += v[i];

            auto start = lower_bound(sum.begin(), sum.end(), sumR - maxSum) - sum.begin();
            auto end = upper_bound(sum.begin(), sum.end(), sumR - minSum) - sum.begin();

            cnt = sumDP[end] - sumDP[start];
            if (minSum <= sumR && sumR <= maxSum)
                cnt++;

            cnt %= MOD;

            sumDP[i + 1] = sumDP[i] + cnt;
            sum.push_back(sumR);
        }

        return cnt;
    }

    template <typename T>
    static long long countPartitionsMod(const vector<T>& v, long long minSum, long long maxSum, T MOD) {
        return countPartitionsMod(v.data(), int(v.size()), minSum, maxSum, MOD);
    }
};

// 2. the number of partitions of an array such that
//   1) the sum of each partition must be in non-decreasing format
//   2) the size of each partition must be in non-decreasing format
// https://www.hackerearth.com/practice/algorithms/dynamic-programming/2-dimensional/practice-problems/algorithm/partitioning-binary-strings-857646e9/description/
struct MultiplePartitionProblem2 {
    // O(N^2)
    template <typename T>
    static long long countPartitions(const vector<T>& A) {
        int N = int(A.size());

        vector<T> S(N + 1);
        for (int i = 0; i < N; i++)
            S[i + 1] =  S[i] + A[i];

        /*
                                      [i]
            |1...|1...|1...|1...|1...|1...|1...|
                 [L]       [cur]        [R]

            dp[i][j] = the number of partitions
                i = right index, j = the size of a right half (R - cur)
        */
        vector<vector<long long>> dp(N, vector<long long>(N + 1));

        for (int j = 1; j <= N; j++)
            dp[0][j] = dp[0][j - 1] + 1;

        for (int i = 1; i < N; i++) {
            int R = i + 1;
            int L = i - 1;
            for (int cur = i; cur > 0; cur--) {
                for (; L >= 0; --L) {
                    if ((cur - L > R - cur) || (S[cur] - S[L] > S[R] - S[cur]))
                        break;
                }
                dp[i][R - cur] = dp[i][R - cur - 1] + dp[cur - 1][cur - L - 1];
            }
            dp[i][i + 1] = dp[i][i] + 1;
        }

        return dp[N - 1][N];
    }

    // O(N^2)
    template <typename T>
    static long long countPartitionsMod(const vector<T>& A, T mod) {
        int N = int(A.size());

        vector<T> S(N + 1);
        for (int i = 0; i < N; i++)
            S[i + 1] =  S[i] + A[i];

        /*
                                      [i]
            |1...|1...|1...|1...|1...|1...|1...|
                 [L]       [cur]        [R]

            dp[i][j] = the number of partitions
                i = right index, j = the size of a right half (R - cur)
        */
        vector<vector<long long>> dp(N, vector<long long>(N + 1));

        for (int j = 1; j <= N; j++)
            dp[0][j] = dp[0][j - 1] + 1;

        for (int i = 1; i < N; i++) {
            int R = i + 1;
            int L = i - 1;
            for (int cur = i; cur > 0; cur--) {
                for (; L >= 0; --L) {
                    if ((cur - L > R - cur) || (S[cur] - S[L] > S[R] - S[cur]))
                        break;
                }
                dp[i][R - cur] = (dp[i][R - cur - 1] + dp[cur - 1][cur - L - 1]) % mod;
            }
            dp[i][i + 1] = (dp[i][i] + 1) % mod;
        }

        return dp[N - 1][N];
    }
};
