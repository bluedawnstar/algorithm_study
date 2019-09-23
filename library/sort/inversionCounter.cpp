#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

#include "inversionCounter.h"
#include "inversionCounter2.h"

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

void testInversionCounter() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Inversion Counter ---------------------------------" << endl;
    {
        vector<int> A{ 1, 3, 2, 0, 5, 4 };
        int gt = 5;
        int ans1 = int(InversionCounter<int>::count(A));
        int ans2 = int(InversionCounter2::count(A));
        if (ans1 != gt || ans2 != gt)
            cout << "ERROR at " << __LINE__ << endl;
        assert(ans1 == gt && ans2 == gt);
    }
    {
        vector<int> A{ 3, 3, 2, 3, 2, 4 };
        int gt = 5;
        int ans1 = int(InversionCounter<int>::count(A));
        int ans2 = int(InversionCounter2::count(A));
        if (ans1 != gt || ans2 != gt)
            cout << "ERROR at " << __LINE__ << endl;
        assert(ans1 == gt && ans2 == gt);
    }
    {
        int N = 1000;

        vector<int> A(N);
        for (int i = 0; i < N; i++)
            A[i] = RandInt32::get() % N;

        auto gt = countSlow(A);
        auto ans1 = InversionCounter<int>::count(A);
        auto ans2 = InversionCounter2::count(A);
        if (ans1 != gt || ans2 != gt)
            cout << "ERROR at " << __LINE__ << endl;
        assert(ans1 == gt && ans2 == gt);
    }

    cout << "OK!" << endl;
}
