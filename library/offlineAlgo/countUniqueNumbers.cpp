#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "../rangeQuery/fenwickTree.h"
#include "countUniqueNumbers.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <unordered_set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static vector<int> countNaive(const vector<int>& A, const vector<pair<int, int>>& rangeQuery) {
    vector<int> ans(rangeQuery.size());
    for (int i = 0; i < int(rangeQuery.size()); i++) {
        int L = rangeQuery[i].first;
        int R = rangeQuery[i].second;

        unordered_map<int, int> M;
        for (; L <= R; L++) {
            int n = ++M[A[L]];
            if (n == 1)
                ++ans[i];
            else if (n == 2)
                --ans[i];
        }
    }

    return ans;
}

void testCountUniqueNumbers() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Count Unique Numbers with Ranges -------------" << endl;

    int N = 1000;
    vector<int> in(N);
    for (int i = 0; i < N; i++)
        in[i] = RandInt32::get() % 100;

    vector<pair<int, int>> query;
    int T = 100;
    for (int i = 0; i < T; i++) {
        int L = RandInt32::get() % N;
        int R = RandInt32::get() % N;
        if (L > R)
            swap(L, R);
        query.emplace_back(L, R);
    }

    auto gt = countNaive(in, query);
    auto ans = countUniqueNumbers(in, query);
    assert(gt == ans);

    cout << "OK!" << endl;
}
