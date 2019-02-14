#include <memory.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "treeKthDistance.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <tuple>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// https://www.codechef.com/FEB19A/problems/TRDST
static vector<int> solve(const vector<int>& K, const vector<pair<int, int>>& edges) {
    int N = int(K.size());

    KthDistance solver;
    solver.init(N);

    for (auto& e : edges)
        solver.addEdge(e.first, e.second);
    solver.build();

    vector<int> res(N);
    for (int u = 0; u < N; u++) {
        int kth = N - K[u];
        res[u] = solver.findKthDistance(u, kth) - 1;
    }

    return res;
}

void testKthDistance() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Kth Distance -----------------------------" << endl;
    {
        vector<int> K = { 1, 2, 3, 6, 6, 4, 5, 2 };
        vector<pair<int, int>> edges = {
            { 0, 1 }, { 1, 2 }, { 1, 4 }, { 1, 5 }, { 3, 4 }, { 4, 6 }, { 4, 7 }
        };
        auto ans = solve(K, edges);
        vector<int> gt = { 2, 1, 2, 1, 0, 1, 1, 2 };
        assert(ans == gt);
    }
    cout << "OK!" << endl;
}
