#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "catalanNumber.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

const int MOD = 1000000007;

void testCatalan() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Catalan Number --------------------------------------" << endl;
    {
        Catalan<int> C;
        C.build(37);

        vector<int> gt{     1,     1,      2,      5,      14,      42,      132,       429,      1430,       4862,
                        16796, 58786, 208012, 742900, 2674440, 9694845, 35357670, 129644790, 477638700, 1767263190 };
        for (int i = 0; i < int(gt.size()); i++) {
            if (C[i] != gt[i])
                cout << "Mismatched : " << C[i] << ", " << gt[i] << endl;
            assert(C[i] == gt[i]);
        }
    }
    {
        CatalanMod<int> C;
        C.build(37);

        vector<int> gt{     1,     1,      2,      5,      14,      42,      132,       429,      1430,       4862,
                        16796, 58786, 208012, 742900, 2674440, 9694845, 35357670, 129644790, 477638700, 1767263190 % MOD };
        for (int i = 0; i < int(gt.size()); i++) {
            if (C[i] != gt[i])
                cout << "Mismatched : " << C[i] << ", " << gt[i] << endl;
            assert(C[i] == gt[i]);
        }
    }

    cout << "OK!" << endl;
}
