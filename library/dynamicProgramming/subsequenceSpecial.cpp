#include <vector>
#include <algorithm>

using namespace std;

#include "subsequenceSpecial.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testSubsequenceSpecial() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Special Subsequence Problems ------------------------" << endl;
    {
        assert(SpecialSetCounterMod<>::count(3) == 5);
    }

    cout << "OK!" << endl;
}
