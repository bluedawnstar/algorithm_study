#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

#include "inversionCounterAfterOneSwap.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static long long countSlow(const vector<int>& A) {
    long long res = 0;

    int N = int(A.size());
    for (int i = 1; i < N; i++) {
        for (int j = 0; j < i; j++)
            res += (A[j] > A[i]);
    }

    return res;
}

void testInversionCounterAfterOneSwap() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Inversion Counter after swapping at most once -----------" << endl;
    {
        const int N = 5;
        const vector<int> A{
            1, 4, 2, 3, 5
        };
        const int gt = 1;

        auto ans = InversionCounterAfterOneSwap::count(A);
        assert(ans == gt);
    }
    {
        const int N = 10;
        const vector<int> A{
            1, 8, 2, 4, 5, 10, 6, 7, 9, 3
        };
        const int gt = 6;

        auto ans = InversionCounterAfterOneSwap::count(A);
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
