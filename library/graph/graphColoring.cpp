#include <climits>
#include <numeric>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#include "graphColoring.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testGraphColoringGreedy() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Graph Coloring ----------------" << endl;
    {
        GraphColoring gc(5);
        gc.addEdge(0, 1);
        gc.addEdge(0, 2);
        gc.addEdge(1, 2);
        gc.addEdge(1, 3);
        gc.addEdge(2, 3);
        gc.addEdge(3, 4);
        cout << gc.doVertexColoringGreedy() << endl;

        auto ans = gc.doVertexColoring();
        cout << ans.first << ", " << ans.second << endl;
    }
    {
        GraphColoring gc(5);
        gc.addEdge(0, 1);
        gc.addEdge(0, 2);
        gc.addEdge(1, 2);
        gc.addEdge(1, 4);
        gc.addEdge(2, 4);
        gc.addEdge(4, 3);
        cout << gc.doVertexColoringGreedy() << endl;

        auto ans = gc.doVertexColoring();
        cout << ans.first << ", " << ans.second << endl;
    }

    cout << "OK" << endl;
}
