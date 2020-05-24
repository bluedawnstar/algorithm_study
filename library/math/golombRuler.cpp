#include <vector>
#include <algorithm>

using namespace std;

#include "golombRuler.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <unordered_set>
#include <iostream>
#include "../common/iostreamhelper.h"

static bool check(const vector<int>& v) {
    unordered_set<int> S;
    for (int i = 0; i < int(v.size()); i++) {
        for (int j = i + 1; j < int(v.size()); j++) {
            int d = v[j] - v[i];
            if (S.find(d) != S.end())
                return false;
            S.insert(d);
        }
    }
    return true;
}

void testGolombRuler() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Golomb ruler -----------------------" << endl;
    {
        for (int n = 3; n <= 10; n++) {
            bool ok = check(GolombRuler::build(n));
            if (!ok)
                cout << "invalid sequences : N = " << n << endl;
            assert(ok);
        }
    }

    cout << "OK!" << endl;
}
