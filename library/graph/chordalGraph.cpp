#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "chordalGraph.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testChordalGraph() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Chordal Graph ----------------" << endl;
    {
        ChordalGraph graph(6);
        graph.addEdge(0, 2);
        graph.addEdge(0, 5);
        graph.addEdge(2, 3);
        graph.addEdge(1, 4);
        graph.addEdge(1, 5);
        graph.addEdge(2, 4);
        graph.addEdge(4, 5);
        
        auto ans = graph.findPerfectEliminationOrdering();
        if (!ans.empty()) {
            cout << "Invalid answer!" << endl;
            cout << "  ans = " << ans << endl;
        }
        assert(ans.empty());
    }
    {
        ChordalGraph graph(6);
        graph.addEdge(0, 2);
        graph.addEdge(0, 4);
        graph.addEdge(0, 5);
        graph.addEdge(2, 3);
        graph.addEdge(1, 4);
        graph.addEdge(1, 5);
        graph.addEdge(2, 4);
        graph.addEdge(4, 5);

        auto ans = graph.findPerfectEliminationOrdering();
        cout << ans << endl;
        if (ans.empty()) {
            cout << "Invalid answer!" << endl;
            cout << "  ans = " << ans << endl;
        }
        assert(!ans.empty());
    }

    cout << "OK" << endl;
}
