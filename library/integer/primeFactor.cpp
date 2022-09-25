#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "primeFactor.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <iostream>
#include "../common/iostreamhelper.h"

#define NN  1000000

void testPrimeFactor() {
    return; //TODO: if you want to test, make this line a comment.

    clock_t start;

    SmallestPrimeFactors spf;
    spf.build(100);

    cout << "--- test isPrimeNumber() from 0 to 100 ---" << endl;
    for (int i = 0; i <= 100; i++) {
        if (spf.isPrimeNumber(i))
            cout << i << ", ";
    }
    cout << endl;

    cout << "--- test getPrimeFactors() from 0 to 100 ---" << endl;
    for (int i = 0; i <= 100; i++)
        cout << i << ": " << spf.getPrimeFactors(i) << endl;

    PrimeFactors pf;
    cout << "--- test getPrimeFactors2() from 0 to 100 ---" << endl;
    for (int i = 0; i <= 100; i++) {
        pf.build(spf, i);
        cout << i << ": " << pf.primeFactors << endl;
    }

    cout << "--- performance test about prime number functions ---" << endl;

    spf.build(NN);

    start = clock();
    for (int i = 0; i <= NN; i++)
        spf.getPrimeFactors(i);
    cout << "getPrimeFactors()'s elapsed time from 1 to " << NN << " = " << double(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;

    start = clock();
    for (int i = 0; i <= NN; i++) {
        PrimeFactors pf(spf, i);
    }
    cout << "getPrimeFactors2()'s elapsed time from 1 to " << NN << " = " << double(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
}
