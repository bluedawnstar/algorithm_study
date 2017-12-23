#include <climits>
#include <numeric>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#include "graphColoringGreedy.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"


void testGraphColoringGreedy() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Graph Coloring Greedy ---------" << endl;
    {
        GraphColoringGreedy gc(5);
        gc.addEdge(0, 1);
        gc.addEdge(0, 2);
        gc.addEdge(1, 2);
        gc.addEdge(1, 3);
        gc.addEdge(2, 3);
        gc.addEdge(3, 4);
        cout << gc.doVertexColoring() << endl;
    }
    {
        GraphColoringGreedy gc(5);
        gc.addEdge(0, 1);
        gc.addEdge(0, 2);
        gc.addEdge(1, 2);
        gc.addEdge(1, 4);
        gc.addEdge(2, 4);
        gc.addEdge(4, 3);
        cout << gc.doVertexColoring() << endl;
    }

    cout << "OK" << endl;
}
