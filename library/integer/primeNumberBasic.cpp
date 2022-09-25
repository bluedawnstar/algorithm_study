#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "primeNumberBasic.h"
#include "primeFactor.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#define NN  1000000

void testPrimeNumberBasic() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- test getPrimeFactors() -----------" << endl;
    {
        auto ans0 = getPrimeFactors(10001);
        auto primes = findPrimeNumbers(100);
        auto ans1 = getPrimeFactorsFast(10001, primes);
        assert(ans0 == ans1);
    }
    {
        int N = 100000007;
        auto primes = findPrimeNumbers(N);

        PROFILE_START(0);
        auto ans0 = getPrimeFactors(N);
        PROFILE_STOP(0);
        PROFILE_START(1);
        PROFILE_STOP(1);
        PROFILE_START(2);
        auto ans1 = getPrimeFactorsFast(N, primes);
        PROFILE_STOP(2);

        if (ans0.empty() || ans1.empty())
            cerr << "It mustn't be shown!" << endl;
    }
    cout << "OK!" << endl;

    clock_t start;

    cout << "--- test isPrimeNumber() from 0 to 100 ---" << endl;
    {
        for (int i = 0; i <= 100; i++) {
            auto b = isPrimeNumber(i);
            if (b)
                cout << i << ", ";
        }
        cout << endl;

        PROFILE_START(0);
        int n = 0;
        for (int i = 0; i <= 1000000; i++) {
            n += isPrimeNumber(i);
        }
        PROFILE_STOP(0);
        if (n <= 0)
            cerr << "What?" << endl;

        //PROFILE_START(1);
        //n = 0;
        //for (int i = 0; i <= 1000000; i++) {
        //    n += isPrimeNumberWithMiller(i, 5);
        //}
        //PROFILE_STOP(1);
        //if (n <= 0)
        //    cerr << "What?" << endl;
    }

    cout << "--- test getPrimeFactors() from 0 to 100 ---" << endl;
    for (int i = 0; i <= 100; i++)
        cout << i << ": " << getPrimeFactors(i) << endl;

    cout << "--- test getPrimeFactors2() from 0 to 100 ---" << endl;
    for (int i = 0; i <= 100; i++) {
        PrimeFactors pf(i);
        cout << i << ": " << pf.primeFactors << endl;
    }

    cout << "--- test findPrimeNumbers() from 0 to 100 ---" << endl;
    cout << findPrimeNumbers(100) << endl;;

    cout << "--- test getPrimeFactors(n,primes,factors) from 0 to 100 ---" << endl;
    {
        vector<int> primes;
        vector<PrimeFactors> primeFactors;
        buildAll(100, primes, primeFactors);
        for (int i = 0; i < 100; i++)
            cout << primeFactors[i].primeFactors << endl;
    }

    cout << "--- performance test about prime factorization functions ---" << endl;

    start = clock();
    for (int i = 0; i <= NN; i++)
        getPrimeFactors(i);
    cout << "getPrimeFactor()'s elapsed time from 1 to " << NN << " = " << double(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;

    start = clock();
    for (int i = 0; i <= NN; i++) {
        PrimeFactors pf(i);;
    }
    cout << "getPrimeFactor2()'s elapsed time from 1 to " << NN << " = " << double(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;

    start = clock();
    findPrimeNumbers(NN);
    cout << "findPrimeNumbers()'s elapsed time from 1 to " << NN << " = " << double(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;

    start = clock();
    {
        vector<int> primes;
        vector<PrimeFactors> primeFactors;
        buildAll(NN, primes, primeFactors);
    }
    cout << "getPrimeFactors(n,primes,factors)'s elapsed time from 1 to " << NN << " = " << double(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;

    cout << "OK!" << endl;
}
