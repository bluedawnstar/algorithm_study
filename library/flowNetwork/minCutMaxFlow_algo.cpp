#include <queue>
#include <algorithm>
#include <vector>

using namespace std;

#include "minCutMaxFlow_algo_signAssignmentProblem.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <assert.h>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testMinCutMaxFlowAlgo() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Applications of Min Cut - Max Flow ---------" << endl;
    {
        SignAssignmentProblem<int> solver;

        /*
          +-----------+-----------+
          |           |           |
          | p=3, h=1  | p=1, h=4  |
          |    (0)    |    (1)    |
          +-----------+-----------+
          |           |           |
          |      h=-1 | p=2, h=-2 |
          |           |    (2)    |
          +-----------+-----------+
        */

        vector<int> h{ 1, 4, -2 };
        vector<int> p{ 3, 1,  2 };

        solver.init(3, h, p);
        solver.addEdge(0, 1);
        solver.addEdge(1, 2);

        int ans = solver.solve();
        int gt = 12;
        if (ans != gt)
            cout << "Mismatched : " << ans << ", " << gt << endl;
        assert(ans == 12);

        vector<int> gtSign{ 1, 1, -1 };
        if (solver.sign != gtSign)
            cout << "Mismatched : " << solver.sign << ", " << gtSign << endl;
        assert(solver.sign == gtSign);
    }
    cout << "OK!" << endl;
}
