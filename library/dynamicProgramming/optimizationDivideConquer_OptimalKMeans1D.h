#pragma once

// O(K*N*logN)
struct OptimalKMeans1D {
    const double INF = 1e18;

    int N;
    vector<vector<double>> dp;
    vector<vector<int>> groupStart;
    vector<double> S, SS;

    // PRECONDITION: A must be sorted by ascending order
    OptimalKMeans1D(const vector<double>& A) : N(int(A.size())), S(N + 1), SS(N + 1) {
        for (int i = 1; i <= N; i++) {
            S[i] = S[i - 1] + A[i - 1];
            SS[i] = SS[i - 1] + 1.0 * A[i - 1] * A[i - 1];
        }
    }

    double solve(int K) {
        dp = vector<vector<double>>(K + 1, vector<double>(N + 1, INF));
        groupStart = vector<vector<int>>(K + 1, vector<int>(N + 1, 0));

        dp[0][0] = 0.0;
        for (int i = 1; i <= K; i++)
            divideAndConquerDP(i, 1, N, 0, N);
        return dp[K][N];
    }

    vector<pair<int, int>> getGroup() const {
        vector<pair<int, int>> res;
        res.reserve(dp.size() - 1);

        int last = N;
        int i = int(dp.size()) - 1;
        while (last > 0) {
            res.emplace_back(groupStart[i][last], last - 1);
            last = groupStart[i--][last];
        }
        reverse(res.begin(), res.end());
        return res;
    }

    // 1 <= i <= K, 1 <= left <= right <= N
    double cost(int i, int left, int right) const {
        return SS[right] - SS[left - 1] - double(S[right] - S[left - 1]) * (S[right] - S[left - 1]) / (right - left + 1);
    }

private:
    void divideAndConquerDP(int i, int jlo, int jhi, int klo, int khi) {
        if (jlo > jhi)
            return;

        int jmid = jlo + (jhi - jlo) / 2;
        int minK = klo;

        dp[i][jmid] = INF;
        groupStart[i][jmid] = klo;
        for (int k = klo; k <= khi && k < jmid; k++) {
            double val = dp[i - 1][k] + cost(i, k + 1, jmid);
            if (val < dp[i][jmid]) {
                dp[i][jmid] = val;
                groupStart[i][jmid] = k;
                minK = k;
            }
        }

        divideAndConquerDP(i, jlo, jmid - 1, klo, minK);
        divideAndConquerDP(i, jmid + 1, jhi, minK, khi);
    }
};
