#include <memory>
#include <vector>
#include <unordered_map>

using namespace std;

#include "matrixPowerSum.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

const int MOD = 1'000'000'007;

static MatrixMod<MOD> powerSum(const MatrixMod<MOD>& A, int N) {
    int K = A.N;
    MatrixMod<MOD> res(K), curr(K);
    res.identity();
    curr.identity();

    for (int i = 1; i <= N; i++) {
        curr *= A;
        res += curr;
    }

    return res;
}

void testMatrixPowerSum() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Matrix Power Sum ------------------------------" << endl;
    {
        int K = 3;
        MatrixMod<MOD> A(K);

        A.mat[0][0] = 9; A.mat[0][1] = 2; A.mat[0][2] = 5;
        A.mat[1][0] = 1; A.mat[1][1] = 4; A.mat[1][2] = 7;
        A.mat[2][0] = 8; A.mat[2][1] = 3; A.mat[2][2] = 6;

        MatrixPowerSumMod<MOD> mps;
        mps.prepare(A, 1, 10000);

        for (int i = 0; i < 20; i += 2) {
            auto gt = powerSum(A, i);
            auto ans = mps.powerSum(i);
            auto ans2 = MatrixPowerSumMod<MOD>::powerSum(A, i);

            if (ans != gt || ans2 != gt) {
                cout << "Mismatched at " << __LINE__ << endl;
            }
            assert(ans == gt && ans2 == gt);
        }
        for (int i = 101; i < 120; i += 2) {
            auto gt = powerSum(A, i);
            auto ans = mps.powerSum(i);
            auto ans2 = MatrixPowerSumMod<MOD>::powerSum(A, i);

            if (ans != gt || ans2 != gt) {
                cout << "Mismatched at " << __LINE__ << endl;
            }
            assert(ans == gt && ans2 == gt);
        }
    }
    {
        int K = 3;
        int P = 3;
        MatrixMod<MOD> A(K);

        A.mat[0][0] = 9; A.mat[0][1] = 2; A.mat[0][2] = 5;
        A.mat[1][0] = 1; A.mat[1][1] = 4; A.mat[1][2] = 7;
        A.mat[2][0] = 8; A.mat[2][1] = 3; A.mat[2][2] = 6;

        MatrixPowerSumMod<MOD> mps;
        mps.prepare(A, P, 10000);

        auto AK = MatrixMod<MOD>::pow(A, P);
        for (int i = 0; i < 20; i += 2) {
            auto gt = powerSum(AK, i);
            auto ans = mps.powerSum(i);
            auto ans2 = MatrixPowerSumMod<MOD>::powerSum(AK, i);

            if (ans != gt || ans2 != gt) {
                cout << "Mismatched at " << __LINE__ << endl;
            }
            assert(ans == gt && ans2 == gt);
        }
        for (int i = 101; i < 120; i += 2) {
            auto gt = powerSum(AK, i);
            auto ans = mps.powerSum(i);
            auto ans2 = MatrixPowerSumMod<MOD>::powerSum(AK, i);

            if (ans != gt || ans2 != gt) {
                cout << "Mismatched at " << __LINE__ << endl;
            }
            assert(ans == gt && ans2 == gt);
        }
    }

    cout << "OK!" << endl;
}
