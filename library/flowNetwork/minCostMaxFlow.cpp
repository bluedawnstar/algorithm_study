#include <queue>
#include <algorithm>
#include <vector>

using namespace std;

#include "minCostMaxFlow.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

// https://www.hackerrank.com/contests/w32/challenges/balls-and-boxes

#define NODE_BALL_S 0
#define NODE_BOX_S  100
#define NODE_S      200
#define NODE_T      201

void build(MinCostMaxFlow<int>& network, const vector<int>& A, const vector<int>& C, const vector<vector<int>>& B) {
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

void testMinCostMaxFlow() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Min-Cost Max-Flow ---------" << endl;
    // https://www.hackerrank.com/contests/w32/challenges/balls-and-boxes

    MinCostMaxFlow<int> network(300);

    build(network,
        vector<int>{ 1, 4, 8 },
        vector<int>{ 2, 5, 1, 1, 6, 1 },
        vector<vector<int>>{
            { 633, 587, 100, 887, 979, 892 },
            { 708, 871, 132, 28, 255, 825 },
            { 536, 139, 672, 280, 968, 417 }
        }
    );
    int minCost = -network.doMinCostMaxFlow(NODE_S, NODE_T).second;
    cout << "min cost = " << minCost << endl;
    assert(minCost == 6649);

    cout << "OK!" << endl;
}
