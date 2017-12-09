#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "factor.h"
#include "primeFactor.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

#define NN  100

#include "primeNumberBasic.h"
#include "primeNumberEratosthenes.h"

void testFactor() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- getFactors(int x) -------------------------" << endl;
    for (int i = 0; i <= 100; i++) {
        vector<int> v = getFactors(i);
        cout << i << ": " << v << endl;
    }

    cout << "--- getFactors(vector<pair<int,int>>&) -------------------------" << endl;
    for (int i = 1; i <= 100; i++) {
        PrimeFactors pf(i);
        vector<int> factors = pf.getFactors();
        {
            vector<int> v = getFactors(i);
            assert(factors == v);
        }
        cout << i << ": " << factors << endl;
    }

    cout << "--- getAllFactorCount() -------------------------" << endl;
    MinFactors minFactors(NN);
    vector<int> factorCount = minFactors.getAllFactorCount();
    for (int i = 0; i <= NN; i++) {
        cout << "(" << i << ": " << factorCount[i] << "), ";
    }
    cout << endl;

    cout << "--- getAllFactorCountSimple() -------------------------" << endl;
    vector<int> factorCount2 = getAllFactorCountSimple(NN);
    for (int i = 0; i <= NN; i++) {
        cout << "(" << i << ": " << factorCount2[i] << "), ";
    }
    cout << endl;

    assert(factorCount == factorCount2);

    cout << "--- performance test of getAllFactorCountXXX() --" << endl;
    {
        MinFactors minFactors(NN);

        clock_t start = clock();
        for (int i = 0; i < 100000; i++)
            minFactors.getAllFactorCount();
        cout << "elapsed time of getAllFactorCount() : " << double(clock() - start) / CLOCKS_PER_SEC << endl;

        start = clock();
        for (int i = 0; i < 100000; i++)
            getAllFactorCountSimple(NN);
        cout << "elapsed time of getAllFactorCountSimple() : " << double(clock() - start) / CLOCKS_PER_SEC << endl;
    }
}
