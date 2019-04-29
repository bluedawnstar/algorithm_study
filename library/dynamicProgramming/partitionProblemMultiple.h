#pragma once

// https://www.hackerearth.com/practice/algorithms/searching/binary-search/practice-problems/algorithm/partitions-5fd40ffc/description/

// the number of partitions of an array such that the sum of elements in each partition is in [minSum, maxSum]

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
