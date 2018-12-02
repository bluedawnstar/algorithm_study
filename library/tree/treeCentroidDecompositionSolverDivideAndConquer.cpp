#include <memory.h>
#include <functional>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "treeCentroidDecompositionSolverDivideAndConquer.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <tuple>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#define MAXN    100
#define MAXC    100

static TreeCentroidDecompositionSolverDivideAndConquer<int> solver;

static long long gAns[MAXN + 1];
static int gColor[MAXC + 1];
static int gColorStk[MAXC + 1];

static void dfsGetC(vector<int>& C, int u, int parent) {
    C.push_back(solver.values[u]);
    for (int v : solver.edges[u]) {
        if (v != parent && !solver.ctMarked[v])
            dfsGetC(C, v, u);
    }
}

static void dfsC(long long& sum, int u, int parent) {
    solver.treeSize[u] = 1;

    int c = solver.values[u];

    ++gColorStk[c];
    for (int v : solver.edges[u]) {
        if (v != parent && !solver.ctMarked[v]) {
            dfsC(sum, v, u);
            solver.treeSize[u] += solver.treeSize[v];
        }
    }
    --gColorStk[c];

    if (!gColorStk[c]) {
        gColor[c] += solver.treeSize[u];
        sum += solver.treeSize[u];
    }
}

static void addC(int sz, long long currVal, int u, int parent) {
    int c = solver.values[u];

    if (!gColorStk[c])
        currVal += sz - gColor[c];

    gAns[u] += currVal;

    ++gColorStk[c];
    for (int v : solver.edges[u]) {
        if (v != parent && !solver.ctMarked[v])
            addC(sz, currVal, v, u);
    }
    --gColorStk[c];
}

void subC(int sz, long long currVal, int u, int parent) {
    int c = solver.values[u];

    if (!gColorStk[c])
        currVal += sz - gColor[c];

    gAns[u] -= currVal;

    ++gColorStk[c];
    for (int v : solver.edges[u]) {
        if (v != parent && !solver.ctMarked[v])
            subC(sz, currVal, v, u);
    }
    --gColorStk[c];
}


// https://www.hackerrank.com/challenges/unique-colors/problem
// https://www.hackerrank.com/contests/may-world-codesprint/challenges/unique-colors
static vector<long long> solve(const vector<int>& A, const vector<pair<int, int>>& edges) {
    //step1: initialize
    solver.init(A);

    //step2: add edges
    for (auto it : edges)
        solver.addEdge(it.first, it.second);

    //step3: solve
    solver.solve(
        []() {
            // no action
        },
        [](int u) {
            // get colors of all descendants
            vector<int> C;
            dfsGetC(C, u, -1);
            sort(C.begin(), C.end());
            C.erase(unique(C.begin(), C.end()), C.end());

            // initialize
            for (auto c : C)
                gColor[c] = gColorStk[c] = 0;

            long long uVal = 0;
            dfsC(uVal, u, -1);

            // initialize
            for (auto c : C)
                gColorStk[c] = 0;

            addC(solver.treeSize[u], uVal, u, -1);
        },
        [](int u, int parent) {
            // get colors of all descendants
            vector<int> C;
            dfsGetC(C, u, parent);
            C.push_back(solver.values[parent]);    // a parent color
            sort(C.begin(), C.end());
            C.erase(unique(C.begin(), C.end()), C.end());

            // initialize
            for (auto c : C)
                gColor[c] = gColorStk[c] = 0;

            long long uVal = 0;
            dfsC(uVal, u, parent);

            uVal += solver.treeSize[u] - gColor[solver.values[parent]];
            gColor[solver.values[parent]] = solver.treeSize[u];

            // initialize
            for (auto c : C)
                gColorStk[c] = 0;

            subC(solver.treeSize[u], uVal, u, parent);
        }
    );

    vector<long long> res;
    for (int i = 0; i < solver.N; i++)
        res.push_back(gAns[i]);

    return res;
}

void testCentroidDecompositionSolverDiviceAndConquer() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Centroid Decomposition Solver by Divide and Conquer -------------------------" << endl;
    {
        vector<int> A = { 1, 2, 3, 2, 3 };
        vector<pair<int, int>> edges = { { 0, 1 },{ 1, 2 },{ 1, 3 },{ 0, 4 } };
        auto ans = solve(A, edges);
        vector<long long> gt = { 10, 9, 11, 9, 12 };
        assert(ans == gt);
    }
    cout << "OK!" << endl;
}
