#include <queue>
#include <algorithm>
#include <vector>

using namespace std;

#include "minCutMaxFlow.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testMinCutMaxFlow() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Min Cut - Max Flow ---------" << endl;

    MinCutMaxFlow<int> network(6);

    /*
    int in[][6] = {
    { 0, 16, 13, 0, 0, 0 },
    { 0, 0, 10, 12, 0, 0 },
    { 0, 4, 0, 0, 14, 0 },
    { 0, 0, 9, 0, 0, 20 },
    { 0, 0, 0, 7, 0, 4 },
    { 0, 0, 0, 0, 0, 0 }
    };
    */

    network.addEdge(0, 1, 16);
    network.addEdge(0, 2, 13);
    network.addEdge(1, 2, 10, 4);
    network.addEdge(1, 3, 12);
    network.addEdge(2, 3, 0, 9);
    network.addEdge(2, 4, 14);
    network.addEdge(3, 4, 0, 7);
    network.addEdge(3, 5, 20);
    network.addEdge(4, 5, 4);

    vector<pair<int, int>> minCutEdges;
    auto flow = network.findMinCut(0, 5, minCutEdges);
    cout << "Min-Cut : " << flow << endl;
    assert(flow == 23);

    cout << minCutEdges << endl;
    auto gtMinCutEdges = vector<pair<int, int>>{ { 1, 3 }, { 4, 3 }, { 4, 5 } };
    assert(minCutEdges == gtMinCutEdges);

    cout << "OK!" << endl;
}
