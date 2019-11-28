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
    {
        CatalanMod<int> C;
        C.buildFast(37);

        vector<int> gt{     1,     1,      2,      5,      14,      42,      132,       429,      1430,       4862,
            16796, 58786, 208012, 742900, 2674440, 9694845, 35357670, 129644790, 477638700, 1767263190 % MOD };
        for (int i = 0; i < int(gt.size()); i++) {
            if (C[i] != gt[i])
                cout << "Mismatched : " << C[i] << ", " << gt[i] << endl;
            assert(C[i] == gt[i]);
        }
    }
    {
        CatalanMod<int> C1, C2;

        int N = 30000;
#ifdef _DEBUG
        N = 1000;
#endif

        PROFILE_START(0);
        C1.build(N);
        PROFILE_STOP(0);

        PROFILE_START(1);
        C2.buildFast(N);
        PROFILE_STOP(1);

        for (int i = 0; i <= N; i++) {
            if (C1[i] != C2[i])
                cout << "Mismatched : " << C1[i] << ", " << C2[i] << endl;
        }
    }

    cout << "OK!" << endl;
}
