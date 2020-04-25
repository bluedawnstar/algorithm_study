#include <vector>
#include <queue>
#include <list>
#include <unordered_set>
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

// https://www.codechef.com/problems/EGGFREE

// result = { (u->v direction?), ... }
static vector<bool> solveEggFree(const vector<pair<int, int>>& edges, int N) {
    ChordalGraph graph(N);

    for (int i = 0; i < int(edges.size()); i++)
        graph.addEdge(edges[i].first, edges[i].second);

    auto order = graph.findPerfectEliminationOrdering();
    //auto order = graph.findPerfectEliminationOrderingWithLexBFS();
    if (order.empty())
        return vector<bool>();

    vector<int> rev(N);
    for (int i = 0; i < N; i++)
        rev[order[i]] = i;

    vector<bool> res(edges.size());
    for (int i = 0; i < int(edges.size()); i++)
        res[i] = (rev[edges[i].first] < rev[edges[i].second]);

    return res;
}


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
