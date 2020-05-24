#include <tuple>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#include "shortestPathOneSourceWithWildcard.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testShortestPathWithWildcard() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Shortest Path with Wildcards ---------" << endl;
    {
        ShortestPathWithWildcard<int> graph(5, 1);
        graph.addEdge(0, 1, 2);
        graph.addEdge(0, 2, 6);
        graph.addEdge(1, 3, 6);
        graph.addEdge(1, 4, 8);
        graph.addEdge(2, 4, 4);
        graph.addEdge(3, 4, 1);

        graph.dijkstra(0);

        vector<int> ans(5);
        for (int i = 0; i < 5; i++)
            ans[i] = graph.getDistance(i);

        vector<int> gt{ 0, 0, 0, 2, 2 };
        if (ans != gt)
            cout << "Mismatched : ans = " << ans << ", gt = " << gt << endl;
        assert(ans == gt);
    }
    cout << "OK" << endl;
}
