#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "primeNumberEratosthenes.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <iostream>
#include "../common/iostreamhelper.h"

#define NN  1000000

void testPrimeNumberEratosthenes() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    clock_t start;

    cout << "--- test eratosthenes() from 0 to 100 ---" << endl;
    {
        auto v = eratosthenes(100);
        cout << "(";
        for (int i = 0; i < (int)v.size(); i++) {
            if (v[i])
                cout << i << ", ";
        }
        cout << ")";
    }

    vector<int> minFactors = getMinFactors(100);

    cout << "--- test isPrimeNumber() from 0 to 100 ---" << endl;
    for (int i = 0; i <= 100; i++) {
        if (isPrimeNumber(minFactors, i))
            cout << i << ", ";
    }
    cout << endl;

    cout << "--- test getPrimeFactors() from 0 to 100 ---" << endl;
    for (int i = 0; i <= 100; i++)
        cout << i << ": " << getPrimeFactors(minFactors, i) << endl;

    cout << "--- test getPrimeFactors2() from 0 to 100 ---" << endl;
    for (int i = 0; i <= 100; i++)
        cout << i << ": " << getPrimeFactors2(minFactors, i) << endl;

    cout << "--- performance test about prime number functions ---" << endl;

    start = clock();
    eratosthenes(NN);
    cout << "eratosthenes()'s elapsed time from 1 to " << NN << " = " << double(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;

    minFactors = getMinFactors(NN);

    start = clock();
    for (int i = 0; i <= NN; i++)
        getPrimeFactors(minFactors, i);
    cout << "getPrimeFactors()'s elapsed time from 1 to " << NN << " = " << double(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;

    start = clock();
    for (int i = 0; i <= NN; i++)
        getPrimeFactors2(minFactors, i);
    cout << "getPrimeFactors2()'s elapsed time from 1 to " << NN << " = " << double(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
}
