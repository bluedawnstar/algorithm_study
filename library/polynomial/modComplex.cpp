#include <vector>
#include <algorithm>

using namespace std;

#include "modComplex.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "modInt.h"
#include "mycomplex.h"

static const int MOD = 1'000'000'007;

template <typename CompT>
static CompT pow(CompT x, int n) {
    CompT res(1, 0);

    for (; n > 0; n >>= 1) {
        if (n & 1)
            res = res * x;
        x = x * x;
    }
    return res;
}

template <typename CompT>
static CompT pow(CompT x, long long n) {
    if (this->first == 0 && this->second == 0 && n > 0)
        return CompT(0, 0);
    return pow(x, int(n % (MOD - 1)));
}


void testModComplex() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Complex with modular operations ------------------------" << endl;
    {
        Complex<ModInt<int, MOD>> x1;
        ModComplex<int, MOD> x2;
        x1.set(723'521'023, 999'999'011);
        x2.set(723'521'023, 999'999'011);
        assert(x1.first == x2.first && x1.second == x2.second);

        Complex<ModInt<int, MOD>> mul1(319'772'121, 833'925'101);
        ModComplex<int, MOD> mul2(319'772'121, 833'925'101);
        x1 *= mul1;
        x2 *= mul2;
        assert(x1.first == x2.first && x1.second == x2.second);

        Complex<ModInt<int, MOD>> div1(913'644'007, 521'698'331);
        ModComplex<int, MOD> div2(913'644'007, 521'698'331);
        x1 /= div1;
        x2 /= div2;
        assert(x1.first == x2.first && x1.second == x2.second);

        x1 = pow(x1, 107);
        x2 = x2.pow(107);
        assert(x1.first == x2.first && x1.second == x2.second);
    }

    cout << "OK!" << endl;
}
