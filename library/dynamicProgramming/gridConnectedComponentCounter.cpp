#include <algorithm>

using namespace std;

#include "gridConnectedComponentCounter.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testGridConnectedComponentCounter() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Grid Connected Component Counter -----------" << endl;
    {
        assert(GridConnectedComponentCounter2xN<>::count(2) == 34);
        assert(GridConnectedComponentCounter2xN<>::count(3) == 176);
        assert(GridConnectedComponentCounter2xN<>::count(4) == 864);
        assert(GridConnectedComponentCounter2xN<>::count(5) == 4096);
    }

    cout << "OK!" << endl;
}
