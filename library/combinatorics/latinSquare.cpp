#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "latinSquare.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testLatinSquare() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Latin Square -----------------------------------" << endl;
    {
        vector<vector<int>> out;
        assert(LatinSquare::findAnyWithTrace(out, 3, 6) == true);
        assert(LatinSquare::findAnyWithTrace(out, 2, 3) == false);
    }

    cout << "OK!" << endl;
}
