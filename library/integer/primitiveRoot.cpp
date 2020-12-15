#include <cassert>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

#include "primitiveRoot.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testPrimitiveRoot() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Primitive Root -----------------------------" << endl;
    {
        int pr = PrimitiveRootFinder::find(239);
        cout << "primitive root of 239 = " << pr << endl;
        assert(pr == 7);
    }
    {
        int pr = PrimitiveRootFinder::find(998244353);
        cout << "primitive root of 998244353 = " << pr << endl;
        assert(pr == 3);

        pr = PrimitiveRootFinder::find(761);
        cout << "primitive root of 761 = " << pr << endl;
        assert(pr == 6);

        cout << "primitive root of " << 1000000007 << " = " << PrimitiveRootFinder::find(1000000007) << endl;
        cout << "primitive root of " << 1000000009 << " = " << PrimitiveRootFinder::find(1000000009) << endl;

        //for (int i = 0; i <= 1000000009; i++) {
        //    int r = PrimitiveRootFinder::find(i);
        //    if (r > 0)
        //        cout << "primitive root of " << i << " = " << r << endl;
        //}
    }
    cout << "OK!" << endl;
}
