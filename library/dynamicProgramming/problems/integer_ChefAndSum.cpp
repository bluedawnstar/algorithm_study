#include <functional>
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "integer_ChefAndSum.h"
#include "integer_ChefAndSum2.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../../common/iostreamhelper.h"
#include "../../common/profile.h"

void testChefAndSum() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- ChefAndSum problem ------------------------" << endl;
    {
        ChefAndSumSolver<> solver;
        ChefAndSumSolver2<> solver2;

        vector<int> A{ 2, 3, 4 };
        long long K = 2, M = 1, X = 1;

        int ans1 = solver.solve(10, A, K, M, X);
        int ans2 = solver2.solve(10, A, K, M, X);
        if (ans1 != 2414 || ans2 != 2414)
            cout << "Mismatched : " << ans1 << ", " << ans2 << endl;
        assert(ans1 == 2414 && ans2 == 2414);
    }

    cout << "OK!" << endl;
}
