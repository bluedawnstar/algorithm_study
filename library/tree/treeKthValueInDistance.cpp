#include <memory.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "treeKthValueInDistance.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <tuple>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// https://www.hackerrank.com/contests/w38/challenges/neighborhood-queries/problem
static vector<int> solve(const vector<int>& A, const vector<pair<int,int>>& edges, const vector<tuple<int,int,int>>& query) {
    //step1: initialize
    KthValueInDistance<int> solver;
    solver.init(A);

    //step2: add edges
    for (auto it : edges)
        solver.addEdge(it.first, it.second);

    //step3: prepare searching
    solver.build();

    vector<int> res;
    for (auto& it : query) {
        int u, d, k;
        tie(u, d, k) = it;
        res.push_back(solver.query(u, d, k));
    }

    return res;
}


void testKthValueInDistance() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Kth Value in Distance -----------------------------" << endl;
    {
        vector<int> A = { 3, 10, 11, 1, 1 };
        vector<pair<int,int>> edges = { { 0, 1 }, { 0, 2 }, { 2, 3 }, { 2, 4 } };
        vector<tuple<int, int, int>> query = { { 3, 2, 3 }, { 4, 0, 1 }, { 4, 0, 2 }, { 2, 1, 4 }, { 2, 2, 4 } };
        auto ans = solve(A, edges, query);
        vector<int> gt = { 3, 1, -1, 11, 10 };
        assert(ans == gt);
    }
    cout << "OK!" << endl;
}
