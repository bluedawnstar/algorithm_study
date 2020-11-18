#include <queue>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

#include "generalMaxMatching.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <unordered_set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testGeneralMaxMatching() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Blossom Algorithm for General Matching -------------" << endl;
    {
        GeneralMaxMatching graph(4);
        graph.addEdge(0, 1);
        graph.addEdge(1, 2);
        graph.addEdge(2, 3);
        graph.addEdge(0, 3);

        int ans = graph.calcMaxMatching();
        cout << "general max matching : " << ans << endl;
        cout << graph.getLastMaxMatchingEdges() << endl;
        assert(ans == 2);
    }
    {
        GeneralMaxMatching graph(10);
        graph.addEdge(0, 1);
        graph.addEdge(1, 2);
        graph.addEdge(2, 3);
        graph.addEdge(3, 5);
        graph.addEdge(1, 9);
        graph.addEdge(9, 8);
        graph.addEdge(8, 4);
        graph.addEdge(4, 5);
        graph.addEdge(8, 7);
        graph.addEdge(4, 6);
        graph.addEdge(5, 6);
        graph.addEdge(7, 6);

        int ans = graph.calcMaxMatching();
        cout << "general max matching : " << ans << endl;
        cout << graph.getLastMaxMatchingEdges() << endl;
        assert(ans == 5);
    }

    cout << "OK" << endl;
}
