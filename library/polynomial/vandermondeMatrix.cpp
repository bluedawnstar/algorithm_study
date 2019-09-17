#include <vector>
#include <algorithm>

using namespace std;

#include "vandermondeMatrix.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "../integer/fastModOp.h"

#define MOD     1000000007

// https://www.hackerearth.com/challenges/competitive/july-circuits-19/algorithm/something-additive-for-you-4e9342a9/
// https://www.hackerearth.com/problem/algorithm/something-additive-for-you-4e9342a9/
// best solution : https://www.hackerearth.com/challenges/competitive/july-circuits-19/algorithm/something-additive-for-you-4e9342a9/submission/28663751/

// answer = { return[1], return[2], return[3], ..., return[M] }
static vector<int> solver(vector<int> A, int N, int M, int K) {
    reverse(A.begin(), A.end());
    vector<int> C(N);

    FastModOp op(N + K, MOD);
    for (int i = 0; i < N; i++)
        C[i] = op.comb(K + i - 1, i); // H(K + i - 1, i)

    return VandermondeMatrixMultiplierMod<MOD>::multiply(C, A, M);
}

void testVandermondeMatrix() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Vandermonde Matrix ----------------" << endl;
    {
        auto ans = solver(vector<int>{ 1, 2, 3 }, 3, 3, 2);
        if (ans[1] != 10 || ans[2] != 20 || ans[3] != 46)
            cout << "Invalid answer : " << ans << endl;
        assert(ans[1] == 10);
        assert(ans[2] == 20);
        assert(ans[3] == 46);
    }
    cout << "OK!" << endl;
}
