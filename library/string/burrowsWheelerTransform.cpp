#include <string>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

#include "burrowsWheelerTransform.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <algorithm>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

static string makeRandomString(int n) {
    string s;
    s.reserve(n);
    for (int i = 0; i < n; i++)
        s.push_back(RandInt32::get() % 26 + 'a');
    return s;
}

void testBurrowsWheelerTransform() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Burrows - Wheeler Transform --------------------------------" << endl;
    {
        auto in = "banana";

        auto gt = "bnn{aa|a";
        auto ans = BurrowsWheelerTransform::bwt(in);
        assert(ans == gt);

        auto ivt = BurrowsWheelerTransform::ibwt(ans);
        assert(ivt == in);
    }
    {
        int N = 100;
        int T = 1000;
        for (int i = 0; i < T; i++) {
            auto s = makeRandomString(N);
            assert(BurrowsWheelerTransform::ibwt(BurrowsWheelerTransform::bwt(s)) == s);
        }
    }

    cout << "OK!" << endl;
}
