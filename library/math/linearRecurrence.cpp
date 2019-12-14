#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

#include "linearRecurrence.h"
#include "detMod.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

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

                auto ans = rec.getNth(K * 2 + i);
                if (X2.back() != ans)
                    cout << "Mismatched : " << ans << ", " << X2.back() << endl;
                assert(X2.back() == ans);
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

    cout << "OK!" << endl;
}
