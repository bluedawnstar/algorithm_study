#include <numeric>
#include <vector>
#include <bitset>
#include <map>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "booleanXorExpression.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <unordered_set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// related problems
// https://www.codechef.com/MARCH19A/problems/TREASURE/

void testBooleanXorExpression() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Boolean Gaussian Elimination --------------------" << endl;
    {
        int T = 100;
        int N = 30;
        int M = 28;

        while (T-- > 0) {
            vector<int> Y(M);
            vector<vector<int>> A(M);
            vector<BoolExprBitSet> B(M);

            for (int i = 0; i < M; i++) {
                Y[i] = RandInt32::get() & 1;

                int xn = RandInt32::get() % N + 1;
                for (int j = 0; j < xn; j++)
                    A[i].push_back(RandInt32::get() % N);
                sort(A[i].begin(), A[i].end());
                A[i].erase(unique(A[i].begin(), A[i].end()), A[i].end());

                B[i].buildFrom(A[i], N);
            }

            auto ans1 = BooleanXorExpression::countSolutionMod(A, Y, N, 1000000007);
            auto ans2 = BooleanXorExpression::countSolutionMod(B, Y, N, 1000000007);

            if (ans1 != ans2) {
                cout << "Mismatched : " << ans1 << ", " << ans2 << endl;
                ans1 = BooleanXorExpression::countSolutionMod(A, Y, N, 1000000007);
                ans2 = BooleanXorExpression::countSolutionMod(B, Y, N, 1000000007);
            }
            assert(ans1 == ans2);
        }
    }
    cout << "*** Speed Test between two functions" << endl;
    {
        int N = 500;
        int M = 498;

        vector<int> Y(M);
        vector<vector<int>> A(M);
        vector<BoolExprBitSet> B(M);

        for (int i = 0; i < M; i++) {
            Y[i] = RandInt32::get() & 1;

            int xn = RandInt32::get() % N + 1;
            for (int j = 0; j < xn; j++)
                A[i].push_back(RandInt32::get() % N);
            sort(A[i].begin(), A[i].end());
            A[i].erase(unique(A[i].begin(), A[i].end()), A[i].end());

            B[i].buildFrom(A[i], N);
        }

        PROFILE_START(0);
        auto ans1 = BooleanXorExpression::countSolutionMod(A, Y, N, 1000000007);
        PROFILE_STOP(0);

        PROFILE_START(1);
        auto ans2 = BooleanXorExpression::countSolutionMod(B, Y, N, 1000000007);
        PROFILE_STOP(1);

        if (ans1 != ans2) {
            cout << "Mismatched : " << ans1 << ", " << ans2 << endl;
        }
        assert(ans1 == ans2);
    }
    cout << "*** Speed Test of bitset version" << endl;
    {
        int N = 5000;
        int M = 4998;
#ifdef _DEBUG
        N = 500;
        M = 498;
#endif

        vector<int> Y(M);
        vector<vector<int>> A(M);
        vector<BoolExprBitSet> B(M);

        for (int i = 0; i < M; i++) {
            Y[i] = RandInt32::get() & 1;

            int xn = RandInt32::get() % N + 1;
            for (int j = 0; j < xn; j++)
                A[i].push_back(RandInt32::get() % N);
            sort(A[i].begin(), A[i].end());
            A[i].erase(unique(A[i].begin(), A[i].end()), A[i].end());

            B[i].buildFrom(A[i], N);
        }

        PROFILE_START(2);
        auto ans = BooleanXorExpression::countSolutionMod(B, Y, N, 1000000007);
        PROFILE_STOP(2);

        if (ans == -2)
            cout << "ERROR!" << endl;
    }

    cout << "OK!" << endl;
}
