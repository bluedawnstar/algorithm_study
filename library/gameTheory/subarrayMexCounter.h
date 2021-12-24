#pragma once

// https://www.codechef.com/problems/MEXPROB

struct SubarrayMexCounter {
    // return the number of subarrays whose MEX is greater than or equal to x (MEX >= x)
    // O(N), sliding window
    static long long countSubarraysMexGreaterThanOrEqualTo(const int A[], int N, int minMex) {
        vector<long long> cnt(N + 1);

        // move right
        int mex = 0, R = 0;
        for (; R < N && mex < minMex; R++) {
            cnt[A[R]]++;
            while (cnt[mex])
                mex++;
        }
        if (mex < minMex)
            return 0ll;

        long long res = (N - R + 1);
        for (int L = 0; L + 1 < N; L++) {
            // move left
            --cnt[A[L]];
            if (A[L] < minMex && cnt[A[L]] <= 0) {
                // move right
                for (; R < N && cnt[A[L]] <= 0; R++)
                    cnt[A[R]]++;
            }
            if (A[L] < minMex && cnt[A[L]] <= 0)
                break;

            res += (N - R + 1);
        }

        return res;
    }

    static long long countSubarraysMexGreaterThanOrEqualTo(const vector<int>& A, int minMex) {
        return countSubarraysMexGreaterThanOrEqualTo(A.data(), int(A.size()), minMex);
    }


    //---

    // O(N*logN), 0 <= kth < N*(N+1)/2
    static int kthMexValueOfAllSubarrays(const int A[], int N, long long kth) {
        long long totalSubarrayCount = 1ll * N * (N + 1) / 2;

        int lo = 1, hi = N;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (countSubarraysMexGreaterThanOrEqualTo(A, N, mid) >= (totalSubarrayCount - kth))
                lo = mid + 1;
            else
                hi = mid - 1;
        }

        return hi;
    }

    static int kthMexValueOfAllSubarrays(const vector<int>& A, long long kth) {
        return kthMexValueOfAllSubarrays(A.data(), int(A.size()), kth);
    }
};
