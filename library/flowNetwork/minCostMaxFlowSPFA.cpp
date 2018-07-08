#include <queue>
#include <algorithm>
#include <vector>

using namespace std;

#include "minCostMaxFlowSPFA.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

// https://www.hackerrank.com/contests/w32/challenges/balls-and-boxes
// https://www.hackerrank.com/contests/w38/challenges/cargo-delivery/problem

#define NODE_BALL_S 0
#define NODE_BOX_S  100
#define NODE_S      200
#define NODE_T      201

void build(MinCostMaxFlowSPFA<int>& network, const vector<int>& A, const vector<int>& C, const vector<vector<int>>& B) {
    int N = (int)A.size();
    int M = (int)C.size();

    for (int i = 0; i < N; i++)
        network.addEdge(NODE_S, NODE_BALL_S + i, A[i], 0);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            network.addEdge(NODE_BALL_S + i, NODE_BOX_S + j, 1, -B[i][j]);
        }
    }

    for (int i = 0; i < M; i++) {
        network.addEdge(NODE_BOX_S + i, NODE_T, C[i], 0);
        for (int j = C[i]; j < N; j++)
            network.addEdge(NODE_BOX_S + i, NODE_T, 1, 2 * (j - C[i]) + 1);
    }
}

void testMinCostMaxFlowSPFA() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Min-Cost Max-Flow - SPFA ---------" << endl;
    // https://www.hackerrank.com/contests/w32/challenges/balls-and-boxes

    MinCostMaxFlowSPFA<int> network(300);

    build(network,
        vector<int>{ 1, 4, 8 },
        vector<int>{ 2, 5, 1, 1, 6, 1 },
        vector<vector<int>>{
            { 633, 587, 100, 887, 979, 892 },
            { 708, 871, 132, 28, 255, 825 },
            { 536, 139, 672, 280, 968, 417 }
    }
    );

    PROFILE_START(0);
    auto ans = network.calcMinCostMaxFlow(NODE_S, NODE_T);
    PROFILE_STOP(0);

    int minCost = -ans.second;
    int maxFlow = ans.first;
    cout << "min cost = " << minCost << ", max flow = " << maxFlow << endl;
    assert(minCost == 6649);
    assert(maxFlow == 11);

    cout << "OK!" << endl;
}
