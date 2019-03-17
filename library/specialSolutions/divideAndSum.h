#pragma once

// https://www.codechef.com/problems/CHONQ
// Tags: Math, sqrt-decomposition
struct DivideAndSumSolver {
    int N;
    vector<long long> values;
    vector<long long> sum;

    // O(N*sqrt(max(A[i])))
    void build(const vector<int>& A) {
        N = int(A.size());
        values.assign(N, 0ll);
        sum.assign(N + 1, 0ll);

        for (int i = 0; i < N; i++) {
            int lastX = 0;

            //step1: process denominators from 1 to sqrt(i)
            //  values[i    ] += A[i] / 1;
            //  values[i - 1] += A[i] / 2;
            //  ...
            //  values[i - sqrt(i) + 1] += A[i] / sqrt(i);
            int j;
            for (j = 1; j * j <= A[i]; j++) {
                int x = A[i] / j;
                int idx = i - j + 1;
                if (idx >= 0)
                    values[idx] += x;
                lastX = x;
            }

            //step2: process last value
            for (; j <= A[i] && A[i] / j == lastX; j++) {
                int idx = i - j + 1;
                if (idx >= 0)
                    values[idx] += lastX;
            }

            if (j > A[i])
                continue;

            //step3: process left all values
            for (int k = A[i] / j; k >= 1; k--) {
                int R = i - A[i] / (k + 1) + 1;
                int L = max(0, i - A[i] / k + 1);
                if (L < R) {
                    sum[L] += k;
                    sum[R] -= k;
                }
            }
        }
    }

    // O(N)
    // return (position, value)
    pair<int, long long> findMin() {
        pair<int, long long> res(N, numeric_limits<long long>::max());

        long long s = 0;
        for (int i = 0; i < N; i++) {
            s += sum[i];
            long long x = values[i] + s;
            if (x < res.second) {
                res.first = i;
                res.second = x;
            }
        }
        return res;
    }

    // O(N)
    // return (position, value)
    pair<int, long long> findFirstLessThanEqual(int K) {
        long long s = 0;
        for (int i = 0; i < N; i++) {
            s += sum[i];
            long long x = values[i] + s;
            if (x <= K)
                return make_pair(i, x);
        }
        return make_pair(N, numeric_limits<long long>::max());
    }
};
