#pragma once

// https://www.hackerrank.com/contests/hourrank-31/challenges/save-the-queen/problem

// N : cover range
// A[i] : i's cover time
struct CoverTime {
    // O(K * logX)
    static double solve(int N, const vector<int>& A) {
        int K = int(A.size());
        if (N > K)
            return 0.0;

        double lo = 0.0, hi = 1e15;

        while (hi - lo > 1e-6) {
            double mid = (lo + hi) / 2;

            int inv = N;
            double req = 0.0;
            for (int j = 0; j < K; j++) {
                if (A[j] >= mid)
                    inv--;
                else
                    req += A[j];
            }

            if (inv <= 0 || (req / inv) >= mid)
                lo = mid;
            else
                hi = mid;
        }

        return (lo + hi) / 2;
    }

    // O(K*logK)
    static double solve2(int N, vector<int> A) {
        int K = int(A.size());
        if (N > K)
            return 0.0;

        sort(A.begin(), A.end(), greater<int>());
        if (N == K)
            return A.back();

        long long left = accumulate(A.begin() + N, A.end(), 0ll);
        A.resize(N);

        int cnt = 1;
        while (A.size() > 1) {
            long long gap = 1ll * (A[A.size() - 2] - A.back()) * cnt;
            if (left <= gap)
                break;

            left -= gap;
            A.pop_back();
            cnt++;
        }

        return A.back() + (1.0 * left / cnt);
    }
};
