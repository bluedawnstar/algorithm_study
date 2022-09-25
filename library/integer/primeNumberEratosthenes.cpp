#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "primeNumberEratosthenes.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#define NN  100000

static bool check(const vector<bool>& L, int offsetL, vector<bool>& R, int offsetR, int N) {
    for (int i = 0; i < N; i++)
        if (L[offsetL++] != R[offsetR++])
            return false;
    return true;
}

void testPrimeNumberEratosthenes() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- test eratosthenes() ---" << endl;
    {
        auto p = eratosthenes(65536);
        cout << "prime count within 65536 : " << count(p.begin(), p.end(), true) << endl;
    }
    {
        auto v = eratosthenes(100);
        cout << "(";
        for (int i = 0; i < int(v.size()); i++) {
            if (v[i])
                cout << i << ", ";
        }
        cout << ")" << endl;

        v = eratosthenes(NN);

        int L = int(sqrt(NN)) + 10, R = NN;
        auto v2 = eratosthenes(L, R);
        assert(check(v, L, v2, 0, R - L + 1));

        L = int(sqrt(NN)) - 10, R = NN;
        v2 = eratosthenes(L, R);
        assert(check(v, L, v2, 0, R - L + 1));

        L = 0, R = NN;
        v2 = eratosthenes(L, R);
        assert(check(v, L, v2, 0, R - L + 1));
    }
    cout << "OK!" << endl;
}
