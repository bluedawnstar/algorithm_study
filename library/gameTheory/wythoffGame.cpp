#include <vector>
#include <algorithm>

using namespace std;

#include "wythoffGame.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testWythoffGame() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Wythoff's game --------------------------" << endl;
    {
        vector<pair<int, int>> gt{
            { 0, 0 }, { 1, 2 }, { 3, 5 }, { 4, 7 }, { 6, 10 },
            { 8, 13 }, { 9, 15 }, { 11, 18 }, { 12, 20 }
        };
        for (int i = 0; i < int(gt.size()); i++) {
            assert(WythoffGame::find(i) == gt[i]);
        }
    }

    cout << "OK!" << endl;
}
