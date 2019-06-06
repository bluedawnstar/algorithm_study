#include <tuple>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#include "circulationProblemWithEdgeDemand.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <map>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testCirculationProblemWithEdgeDemand() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Circulation Problem with Edge Demand ---------------" << endl;

    // https://codeforces.com/gym/100199/attachments/download/1686/20022003-winter-petrozavodsk-camp-andrew-stankevich-contest-1-en.pdf
    // Problem B. Reactor Cooling
    //--- case #1
    {
        CirculationProblemWithEdgeDemand<int> cp;
        cp.init(4);
        cp.addEdge(0, 1, 1, 2);
        cp.addEdge(1, 2, 1, 2);
        cp.addEdge(2, 3, 1, 2);
        cp.addEdge(3, 0, 1, 2);
        cp.addEdge(0, 2, 1, 2);
        cp.addEdge(3, 1, 1, 2);
        auto ans = cp.solve();
        if (ans.first != false)
            cout << "Invalid answer!" << endl;
        assert(ans.first == false);
    }
    //--- case #2
    {
        CirculationProblemWithEdgeDemand<int> cp;
        cp.init(4);
        cp.addEdge(0, 1, 1, 3, 0);
        cp.addEdge(1, 2, 1, 3, 1);
        cp.addEdge(2, 3, 1, 3, 2);
        cp.addEdge(3, 0, 1, 3, 3);
        cp.addEdge(0, 2, 1, 3, 4);
        cp.addEdge(3, 1, 1, 3, 5);

        auto ans = cp.solve();
        if (ans.first != true)
            cout << "Invalid answer!" << endl;
        assert(ans.first == true);

        vector<int> edgeLowerBounds{ 1, 1, 1, 1, 1, 1 };
        vector<int> edgeFlow(6);
        for (int u = 0; u < 4; u++) {
            for (auto& it : cp.mf.edges[u + 2]) {
                if (it.edgeIndex >= 0)
                    edgeFlow[it.edgeIndex] = it.flow + edgeLowerBounds[it.edgeIndex];   // add the lower bound to the flow of this edge
            }
        }

        vector<int> edgeFlowGt{ 1, 2, 3, 2, 1, 1 };
        assert(edgeFlow == edgeFlowGt);
    }

    // http://codeforces.com/problemset/problem/704/D
    // D. Captain America
    {
        //TODO: implement!
    }

    cout << "OK!" << endl;
}
