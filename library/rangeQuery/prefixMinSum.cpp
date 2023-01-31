#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

#include "prefixMinSum.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static long long prefixMinSumSlow(const vector<int>& A, int L, int R) {
    long long res = 0;

    int mx = A[L];
    for (int i = L; i <= R; i++) {
        mx = min(mx, A[i]);
        res += mx;
    }
    return res;
}

void testPrefixMinSum() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Prefix Min Sum ------------------------" << endl;
    {
        const int T = 100;
        const int N = 100;
        const int MAXX = 100'000'000;
        for (int tt = 0; tt < T; tt++) {
            vector<int> A(N);
            for (int i = 0; i < N; i++)
                A[i] = RandInt32::get() % MAXX + 1;

            PrefixMinSum pms(A);
            for (int i = 0; i < T; i++) {
                int L = RandInt32::get() % N;
                int R = RandInt32::get() % N;
                if (L > R)
                    swap(L, R);
                
                auto gt = prefixMinSumSlow(A, L, R);
                auto ans = pms.query(L, R);
                if (ans != gt) {
                    cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
                }
                assert(ans == gt);
            }
        }
    }

    cout << "OK!" << endl;
}
