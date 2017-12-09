#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "primeNumberBasic.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <iostream>
#include "../common/iostreamhelper.h"

#define NN  1000000

void testPrimeNumberBasic() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    clock_t start;

    cout << "--- test isPrimeNumber() from 0 to 100 ---" << endl;
    for (int i = 0; i <= 100; i++) {
        if (isPrimeNumber(i))
            cout << i << ", ";
    }
    cout << endl;

    cout << "--- test getPrimeFactors() from 0 to 100 ---" << endl;
    for (int i = 0; i <= 100; i++)
        cout << i << ": " << getPrimeFactors(i) << endl;

    cout << "--- test getPrimeFactors2() from 0 to 100 ---" << endl;
    for (int i = 0; i <= 100; i++)
        cout << i << ": " << getPrimeFactors2(i) << endl;

    cout << "--- test findPrimeNumbers() from 0 to 100 ---" << endl;
    cout << findPrimeNumbers(100) << endl;;

    cout << "--- test getPrimeFactors(n,primes,factors) from 0 to 100 ---" << endl;
    {
        vector<int> primes;
        vector<vector<pair<int, int>>> primeFactors;
        getPrimeFactors(100, primes, primeFactors);
        cout << primeFactors << endl;
    }

    cout << "--- performance test about prime factorization functions ---" << endl;

    start = clock();
    for (int i = 0; i <= NN; i++)
        getPrimeFactors(i);
    cout << "getPrimeFactor()'s elapsed time from 1 to " << NN << " = " << double(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;

    start = clock();
    for (int i = 0; i <= NN; i++)
        getPrimeFactors2(i);
    cout << "getPrimeFactor2()'s elapsed time from 1 to " << NN << " = " << double(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;

    start = clock();
    findPrimeNumbers(NN);
    cout << "findPrimeNumbers()'s elapsed time from 1 to " << NN << " = " << double(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;

    start = clock();
    {
        vector<int> primes;
        vector<vector<pair<int, int>>> primeFactors;
        getPrimeFactors(NN, primes, primeFactors);
    }
    cout << "getPrimeFactors(n,primes,factors)'s elapsed time from 1 to " << NN << " = " << double(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
}
