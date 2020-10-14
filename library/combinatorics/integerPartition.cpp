#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "integerPartition.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testIntegerPartition() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Integer Partition --------------------------------" << endl;
    {
        long long P[]{
            1, 1, 2, 3, 5, 7, 11, 15, 22, 30, 42, 56, 77,
            101, 135, 176, 231, 297, 385, 490, 627, 792,
            1002, 1255, 1575, 1958, 2436, 3010, 3718, 4565, 5604, 6842, 8349, 
            10143, 12310, 14883, 17977, 21637, 26015, 31185, 37338, 44583, 53174, 63261, 75175, 89134,
            105558, 124754, 147273, 173525
        };

        const int N = sizeof(P) / sizeof(P[0]);

        IntegerPartition intPart;
        intPart.build(N);

        for (int i = 0; i < N; i++) {
            long long ans = intPart.partition(i);
            if (P[i] != ans)
                cout << "Mismatched at p(" << i << ") : " << ans << ", " << P[i] << endl;
            assert(P[i] == ans);
        }
    }
    cout << "OK!" << endl;
}
