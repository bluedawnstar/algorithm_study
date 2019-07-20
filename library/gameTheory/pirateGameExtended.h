#pragma once

// https://www.codechef.com/problems/LVMFFN

// Solution 1: online solution
// Solution 2: offline solution (at line 110)
/*
    1) K = 2
             N
      ---------------------------------------
             1: M
             2: -1
             3: M
             4: M - 2
             5: M - 3
             6: M - 3
             7: M - 4
             8: M - 4
             ...
        base-1: 0
          base: 0         (base = M * K)
        base+1: 0
        base+2: x
        base+3: 0
        base+4: x
        base+5: x
        base+6: x
        base+7: 0
        ...

    2) K > 2
        - when N / K <= M
                M - N / K

        - when N / K > M
            if it is possible at N=i, then next possible position is 

                j = K * M + K * (i + 1) / (K - 1)
                if (j % K == 0)
                    j = j - 1;
*/

struct ExtendedPirateGame {
    long long maxN;
    int K;
    vector<long long> possibleNs;   // possible Ns when N / K > M

    // O(KlogN)
    void build(long long maxN, int K) {
        this->maxN = maxN;
        this->K = K;

#if 0
        for (long long i = K + 1; i <= maxN; ) {
            possibleNs.push_back(i);

            long long j = K * (i + 1) / (K - 1);
            if (j % K == 0)
                j--;

            i = j;
        }
#else
        for (long long i = K + 1; i <= maxN; ) {
            possibleNs.push_back(i);

            long long iq = (i + 1) / (K - 1);
            long long ir = (i + 1) % (K - 1);
            long long j = K * iq + K * ir / (K - 1);
            if (j % K == 0)
                j--;

            i = j;
        }
#endif
        possibleNs.push_back(maxN + 1); // sentinel
    }

    // O(logK + loglogN)
    long long solve(long long N, long long M) {
        if (K == 2)
            return solve2(N, M);

        if (N / K <= M)
            return M - N / K;

        N -= K * M;
        auto it = lower_bound(possibleNs.begin(), possibleNs.end(), N);
        if (*it == N)
            return 0;

        return -1ll;
    }

private:
    // O(1)
    long long solve2(long long N, long long M) {
        if (N == 1)
            return M;
        else if (N == 2)
            return -1ll;
        else if (N <= M * K) {
            if (N == 3)
                return M;
            else
                return max(0ll, M - (N + 1) / 2);
        }

        long long base = M * K;
        long long index = N - base;

        if ((index & (index + 1)) == 0)
            return 0;
        else
            return -1ll;
    }
};

struct ExtendedPirateGameOffline {
    // input = { (N, M), ... }, O(QlogQ + KlogN)
    static vector<long long> solve(vector<pair<long long, long long>>& queries, int K) {
        vector<long long> res(queries.size());

        if (K == 2) {
            for (int i = 0; i < int(queries.size()); i++)
                res[i] = solve2(queries[i].first, queries[i].second);
        } else {
            vector<pair<long long, int>> Q; // (N, index)

            for (int i = 0; i < int(queries.size()); i++) {
                long long N, M;
                tie(N, M) = queries[i];

                if (N / K <= M)
                    res[i] = M - N / K;
                else
                    Q.emplace_back(N - K * M, i);
            }
            sort(Q.begin(), Q.end());

            int qidx = 0;
            for (long long i = K + 1; qidx < int(Q.size()); ) {
                while (qidx < int(Q.size()) && Q[qidx].first < i)
                    res[Q[qidx++].second] = -1ll;

                while (qidx < int(Q.size()) && Q[qidx].first == i)
                    res[Q[qidx++].second] = 0ll;

#if 0
                long long j = K * (i + 1) / (K - 1);
#else
                long long iq = (i + 1) / (K - 1);
                long long ir = (i + 1) % (K - 1);
                long long j = K * iq + K * ir / (K - 1);
#endif
                if (j % K == 0)
                    j--;

                i = j;
            }
        }

        return res;
    }

private:
    // O(1)
    static long long solve2(long long N, long long M) {
        if (N == 1)
            return M;
        else if (N == 2)
            return -1ll;
        else if (N <= M * 2) {
            if (N == 3)
                return M;
            else
                return max(0ll, M - (N + 1) / 2);
        }

        long long base = M * 2;
        long long index = N - base;

        if ((index & (index + 1)) == 0)
            return 0;
        else
            return -1ll;
    }
};
