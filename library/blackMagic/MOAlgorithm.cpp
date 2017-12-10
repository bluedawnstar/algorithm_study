#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

#include "MOAlgorithm.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#include <algorithm>

void testMOAlgorithm() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- MO's algorithm (sqrt-decomposition for answering queries) ------------------------" << endl;
    {
        vector<int> in{ 1, 3, 3, 4 };
        vector<pair<int, int>> qry{ { 0, 3 }, { 1, 3 }, { 2, 3 }, { 3, 3 } };
        vector<int> gt{ 3, 2, 2, 1 };

        MOAlgorithm mo;
        vector<int> ans = mo.query(4, in, qry);
        if (ans != gt) {
            cerr << "Failed: " << endl
                 << "gt = " << gt << endl
                 << "ans = " << ans << endl;
        }
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
