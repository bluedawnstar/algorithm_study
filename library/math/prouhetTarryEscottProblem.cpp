#include <vector>
#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

using namespace std;

#include "prouhetTarryEscottProblem.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

long long intPow(int x, int p) {
    long long res = 1;
    for (int i = 0; i < p; i++)
        res *= x;
    return res;
}

void testProuhetTarryEscottProblem() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Prouhet-Tarry-Escott problem ------------------------------" << endl;
    {
        auto seq = ProuhetTarryEscottProblem::generateThueMorseSequence(3);
        vector<bool> gt{ false, true, true, false, true, false, false, true, true, false, false, true, false, true, true, false };
        if (seq != gt)
            cout << "Mismatched : " << seq << ", " << gt << endl;
        assert(seq == gt);
    }
    {
        const int X = 11;
        const int SCALE = 7;
        for (int k = 1; k <= 5; k++) {
            int N = (1 << (k + 1)) * 7;
            auto group = ProuhetTarryEscottProblem::partition(N, k);

            long long sumA = 0, sumB = 0;
            for (int i = 0; i < N; i++) {
                if (group[i])
                    sumB += intPow(X + i, k);
                else
                    sumA += intPow(X + i, k);
            }

            if (sumA != sumB) {
                cout << "Mismatched at K = " << k << " : " << sumA << ", " << sumB << endl;
            }
            assert(sumA == sumB);
        }
    }

    cout << "OK!" << endl;
}
