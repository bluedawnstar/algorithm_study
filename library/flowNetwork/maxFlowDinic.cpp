#include <queue>
#include <algorithm>
#include <vector>

using namespace std;

#include "maxFlowDinic.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testMaxFlowDinic() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Max Flow - Dinic ---------" << endl;

    MaxFlowDinic<int> maxFlow(6);

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

    maxFlow.addEdge(0, 1, 16, 0);
    maxFlow.addEdge(0, 2, 13, 0);
    maxFlow.addEdge(1, 2, 10, 4);
    maxFlow.addEdge(1, 3, 12, 0);
    maxFlow.addEdge(2, 3, 0, 9);
    maxFlow.addEdge(2, 4, 14, 0);
    maxFlow.addEdge(3, 4, 0, 7);
    maxFlow.addEdge(3, 5, 20, 0);
    maxFlow.addEdge(4, 5, 4, 0);

    auto flow = maxFlow.calcMaxFlow(0, 5);
    cout << "Dinic : " << flow << endl;
    assert(flow == 23);

    cout << "OK!" << endl;
}
