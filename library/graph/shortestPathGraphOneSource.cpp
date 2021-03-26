#include <tuple>
#include <queue>
#include <climits>
#include <algorithm>
#include <vector>

using namespace std;

#include "shortestPathGraphOneSource.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

const int INF = 0x3f3f3f3f;

void testShortestPathGraph() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Shortest Path Graph ---------" << endl;
    {
        // https://www.hackerearth.com/challenges/competitive/march-circuits-21/algorithm/allsomenone-78013449/
        const int N = 6;
        vector<tuple<int, int, int>> edges{ // (u, v, w)
            { 1, 2,  7 },
            { 3, 0, 20 },
            { 4, 0,  5 },
            { 5, 4, 18 },
            { 2, 5, 10 },
            { 1, 3,  8 },
            { 4, 3,  7 },
            { 4, 1,  5 },
        };
        vector<int> gt{ 2, 0, 0, 0, 2, 2 };

        ShortestPathGraph<int, INF> solver(N);
        int u, v, w;
        for (auto& it : edges) {
            tie(u, v, w) = it;
            solver.addEdge(u, v, w);
        }

        // step #1 : find shortest path
        solver.dijkstra(0);
        // step #2 : build undirected graph from shortest path DAG
        auto graph = solver.buildUndirectedGraphFromShortestPaths(N - 1);

        // step #3 : solve problems
        vector<int> ans(N);
        for (int u : graph.originalVertex)
            ans[u] = 1;
        ans[0] = ans[N - 1] = 2;

        auto cut = graph.findCutVertex();
        for (int i = 0; i < graph.N; i++) {
            if (cut[i])
                ans[graph.originalVertex[i]] = 2;
        }

        if (ans != gt)
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }

    cout << "OK" << endl;
}
