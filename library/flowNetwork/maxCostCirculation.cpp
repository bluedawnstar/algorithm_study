#include <queue>
#include <algorithm>
#include <vector>

using namespace std;

#include "maxCostCirculation.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testMaxCostCirculation() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Max-Cost Circulation ---------------" << endl;
    {
        MaxCostCirculation<int> mcc(3);

        mcc.addEdge(0, 1, 2, 1);
        mcc.addEdge(1, 2, 3, -5);
        mcc.addEdge(2, 0, 2, 3);

        auto ans = mcc.solve();
        auto gt = 0;            // no positive cycle
        if (ans != gt) {
            cout << "Mismatched : cost = " << ans << ", " << gt << endl;
        }
        assert(ans == gt);
    }
    {
        MaxCostCirculation<int> mcc(4);

        mcc.addEdge(0, 1, 2, 1);
        mcc.addEdge(1, 2, 3, -5);
        mcc.addEdge(2, 0, 2, 3);
        mcc.addEdge(3, 1, 2, -7);
        mcc.addEdge(2, 3, 3, 2);

        auto ans = mcc.solve();
        auto gt = 0;            // no positive cycle
        if (ans != gt) {
            cout << "Mismatched : cost = " << ans << ", " << gt << endl;
        }
        assert(ans == gt);
    }
    {
        MaxCostCirculation<int> mcc(3);

        mcc.addEdge(0, 1, 2, 1);
        mcc.addEdge(1, 2, 3, -2);
        mcc.addEdge(2, 0, 2, 3);

        auto ans = mcc.solve();
        auto gt = 4;            // 0->1->2->0 : flow(2) * cost(2)
        if (ans != gt)
            cout << "Mismatched : cost = " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        MaxCostCirculation<int> mcc(4);

        mcc.addEdge(0, 1, 2, 1);
        mcc.addEdge(1, 2, 3, -2);
        mcc.addEdge(2, 0, 2, 3);
        mcc.addEdge(3, 1, 2, -3);
        mcc.addEdge(2, 3, 3, 2);

        auto ans = mcc.solve();
        auto gt = 4;            // 3->1->2->3 : flow(2) * cost(2)
        if (ans != gt)
            cout << "Mismatched : cost = " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        MaxCostCirculation<int> mcc(4);

        mcc.addEdge(0, 1, 2, 1);
        mcc.addEdge(1, 2, 3, -2);
        mcc.addEdge(2, 0, 2, 3);
        mcc.addEdge(3, 1, 2, 3);
        mcc.addEdge(2, 3, 3, 2);

        auto ans = mcc.solve();
        auto gt = 8;            // 3->1->2->3 : flow(3) * cost(2), 0->1->2->0 : flow(1) * cost(2)
        if (ans != gt)
            cout << "Mismatched : cost = " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    cout << "OK!" << endl;
}
