#pragma once

/*
    https://www.hackerearth.com/practice/algorithms/dynamic-programming/introduction-to-dynamic-programming-1/practice-problems/algorithm/prefix-minimum-query-4d47a2ee/

                   R
    query(L, R) = SUM min(A[L], A[L + 1], ..., A[i])
                  i=L

                =   min(A[L])
                  + min(A[L], A[L + 1])
                  + ...
                  + min(A[L], A[L + 1], ..., A[R])


    1) build step
        stack + sparse table + suffix sum

    2) query step
        sparse table + suffix sum
*/
struct PrefixMinSum {
    vector<int> A;              // 

    int N;
    int logN;                   // 
    vector<vector<int>> next;   // 
    vector<long long> suffixSum;

    PrefixMinSum() : N(0), logN(0) {
    }

    PrefixMinSum(const int A[], int n) {
        build(A, n);
    }

    explicit PrefixMinSum(const vector<int>& A) {
        build(A);
    }

    // O(N*logN)
    void build(const int A[], int n) {
        this->A.assign(A, A + n);
        build();
    }

    // O(N*logN)
    void build(const vector<int>& A) {
        this->A = A;
        build();
    }

    // O(logN)
    long long query(int L, int R) const {
        int last = L;
        for (int i = logN - 1; i >= 0; i--) {
            int nx = next[i][last];
            if (nx <= R)
                last = nx;
        }

        return (suffixSum[L] - suffixSum[last]) + static_cast<long long>(A[last]) * (R - last + 1);
    }

private:
    void build() {
        // initialize
        N = int(A.size());
        logN = int(log2(N)) + 1;
        next = vector<vector<int>>(logN);
        suffixSum = vector<long long>(N + 1);
        // next[0][0..N]
        {
            stack<pair<int, int>> stk;
            stk.emplace(-1, N);

            next[0].resize(N);
            for (int i = N - 1; i >= 0; i--) {
                while (A[i] <= stk.top().first)
                    stk.pop();

                next[0][i] = stk.top().second;
                suffixSum[i] = suffixSum[next[0][i]] + static_cast<long long>(A[i]) * (next[0][i] - i);

                stk.emplace(A[i], i);
            }
        }
        // build sparse table
        for (int i = 1; i < logN; i++) {
            next[i].resize(N);
            for (int j = 0; j < N; j++) {
                int nx = next[i - 1][j];
                next[i][j] = (nx >= N) ? nx : next[i - 1][nx];
            }
        }
    }
};
