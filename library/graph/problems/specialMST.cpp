#include <tuple>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

#include "specialMST.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../../common/iostreamhelper.h"

void testSpecialMST() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Special MST ------------" << endl;
    {
        SpecialMST mst(4);
        mst.addEdge(0, 1, 1);
        mst.addEdge(0, 2, 1);
        mst.addEdge(0, 3, 1);
        mst.addEdge(1, 2, 2);
        mst.addEdge(2, 3, 2);

        vector<long long> gt{ 0, 5, 4, 3 };
        auto ans = mst.calcMstCostByDegreeOfRootNode();

        if (ans != gt)
            cout << "Mismatched : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    cout << "OK" << endl;
}
