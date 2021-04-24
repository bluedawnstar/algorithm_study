#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

#include "linearRecurrence.h"
#include "detMod.h"
#include "linearRecurrenceMatrix_CayleyHamiltonTheorem.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "fibonacci.h"

const int MOD = 998'244'353;

// https://www.codechef.com/problems/PANIC
struct PanicSolver {
    static MatrixMod<MOD> solve(const MatrixMod<MOD>& mat, int A, int D, int N) {
        int W = int(mat.N);
        int K = int(mat.N) * 2 + 1;

        vector<int> fibo(K * 2);
        for (int i = 0; i < K * 2; i++)
            fibo[i] = fibonacciModFast<int, MOD>(A + 1ll * i * D);

        vector<MatrixMod<MOD>> dp(K * 2, MatrixMod<MOD>(W));

        MatrixMod<MOD> curr(W);

        curr.identity();

        dp[0] = curr * fibo[0];
        for (int kk = 1; kk < K * 2; kk++) {
            auto next = curr * mat;
            dp[kk] = dp[kk - 1] + next * fibo[kk];
            swap(curr.mat, next.mat);
        }

        MatrixLinearRecurrence<int,MOD> rec(dp, mat);
        return rec.getNth(N);
    }
};

void testLinearRecurrence() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Linear Recurrence (Berlekamp-Massey & Kitamasa) ---------------------------" << endl;
    // Berlekamp-Massey & Kitamasa
    {
        int T = 100;
        int TEST_N = 500;
        const int MOD = 998244353;
#ifdef _DEBUG
        T = 10;
        TEST_N = 50;
#endif
        while (T-- > 0) {
            const int K = 20;
            vector<int> C(K);
            vector<int> X(K);

            for (int i = 0; i < K; i++) {
                C[i] = RandInt32::get() % MOD;
                X[i] = RandInt32::get() % MOD;
            }

            for (int i = 0; i < K; i++) {
                long long xx = 0;
                for (int j = 0, n = int(X.size()) - 1; j < K; j++)
                    xx = (xx + 1ll * X[n - j] * C[j]) % MOD;
                X.push_back(int(xx));
            }

            LinearRecurrence<int,MOD> rec;
            rec.build(X);
            if (rec.C != C)
                cout << "Mismatched : " << rec.C << ", " << C << endl;
            assert(rec.C == C);

            auto X2 = X;
            for (int i = 0; i < TEST_N; i++) {
                long long xx = 0;
                for (int j = 0, n = int(X2.size()) - 1; j < K; j++)
                    xx = (xx + 1ll * X2[n - j] * C[j]) % MOD;
                X2.push_back(int(xx));

                auto ans1 = rec.getNth(K * 2 + i);
                auto ans2 = rec.getNthFast(K * 2 + i);
                if (X2.back() != ans1 || X2.back() != ans2)
                    cout << "Mismatched : " << ans1 << ", " << ans2 << ", " << X2.back() << endl;
                assert(X2.back() == ans1 && X2.back() == ans2);
            }
        }
    }
    // Determinant
    {
        int T = 10;
        int N = 200;
        int VALUE_N = N * 2;
        const int MOD = 998244353;

#ifdef _DEBUG
        N = 50;
#endif

        while (T-- > 0) {
            vector<vector<int>> A(N, vector<int>(N));
            for (int i = 0; i < VALUE_N; i++) {
                int idx = RandInt32::get() % (N * N);
                A[idx / N][idx % N] = RandInt32::get() % MOD;
            }

            vector<tuple<int, int, int>> aa;
            for (int c = 0; c < N; c++) {
                for (int r = 0; r < N; r++)
                    aa.emplace_back(c, r, A[r][c]);
            }

            auto ans1 = DetMod<int,MOD>::det(N, A);
            auto ans2 = LinearRecurrence<int,MOD>::det(N, aa);

            if (ans1 != ans2)
                cout << "Mismatched : " << ans1 << ", " << ans2 << endl;
            assert(ans1 == ans2);
        }
    }
    // Matrix Linear Recurrence
    {
        vector<vector<int>> in{
            { 5, 7, 2 },
            { 1, 8, 9 },
            { 3, 4, 5 }
        };
        vector<vector<int>> gt{
            { 601338635, 934201293, 356700741 },
            { 960409891, 125261415, 197093893 },
            { 136328022, 287118456, 122438416 }
        };

        MatrixMod<MOD> mat(3);
        mat.mat = in;

        int A = 23, D = 45, N = 107;
        auto ans = PanicSolver::solve(mat, A, D, N);
        if (ans != gt) {
            cout << "Mismatched at " << __LINE__ << endl;;
        }
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
