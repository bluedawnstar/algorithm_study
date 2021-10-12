#include <vector>
#include <algorithm>

using namespace std;

#include "subarrayMexCounter.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// O(N)
static int calcMex(const int A[], int N) {
    vector<int> cnt(N + 1);

    int mex = 0;
    for (int i = 0; i < N; i++) {
        if (A[i] >= N)
            continue;

        cnt[A[i]]++;
        while (cnt[mex] > 0)
            mex++;
    }

    return mex;
}

// 0 <= kth < N*(N+1)/2
static int kthMexValue(const int A[], int N, long long kth) {
    vector<long long> cnt(N + 1);
    for (int L = 0; L < N; L++) {
        for (int R = L; R < N; R++) {
            ++cnt[calcMex(A + L, R - L + 1)];
        }
    }

    for (int i = 0; ; i++) {
        if (kth < cnt[i])
            return i;
        kth -= cnt[i];
    }
    return N;
}


void testSubarrayMexCounter() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Subarray Mex Counter --------------------------" << endl;
    {
        const int T = 100;
        const int N = 100;
        
        for (int tt = 0; tt < T; tt++) {
            vector<int> A(N);
            iota(A.begin(), A.end(), 0);
            random_shuffle(A.begin(), A.end());

            for (int i = 0; i < 10; i++)
                A[RandInt32::get() % N] = RandInt32::get() % N;

            long long K = RandInt64::get() % (1ll * N * (N + 1) / 2);
            auto ans = SubarrayMexCounter::kthMexValueOfAllSubarrays(A, K);
            auto gt = kthMexValue(A.data(), int(A.size()), K);

            if (ans != gt) {
                cout << "Mismatched : " << ans << ", " << gt << endl;
            }
            assert(ans == gt);
        }
    }

    cout << "OK!" << endl;
}
