#include <tuple>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#include "circulationProblemWithVertexDemand.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testCirculationProblemWithVertexDemand() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Circulation Problem with Vertex Demand ---------------" << endl;
    {
        CirculationProblemWithVertexDemand<int> cp;
        cp.init(vector<int>{ -4, -9, 1, 5, 7 });
        cp.addEdge(0, 2);
        cp.addEdge(0, 3);
        cp.addEdge(1, 0);
        cp.addEdge(1, 2);
        cp.addEdge(1, 4);
        cp.addEdge(2, 3);
        cp.addEdge(2, 4);
        bool ans = cp.isPossible();
        if (ans != true)
            cout << "Invalid answer!" << endl;
        assert(ans == true);
    }
    cout << "OK!" << endl;
}
