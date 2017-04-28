#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "primeNumber.h"

// check if x is a prime number
bool isPrimeNumber(int x) {
    if (x < 2)
        return false;
    else if (x == 2)
        return true;
    else if ((x & 1) == 0)
        return false;

    int root = int(sqrt(x));
    for (int i = 3; i <= root; i += 2) {
        if (x % i == 0)
            return false;
    }

    return true;
}

// return all prime numbers from 1 to n
vector<int> findPrimeNumbers(int n) {
    vector<int> res;
    if (n < 2)
        return res;

    if (n >= 2)
        res.push_back(2);

    for (int i = 3; i <= n; i += 2) {
        bool isPrime = true;
        int root = int(sqrt(i));
        for (int j = 0; res[j] <= root; j++) {
            if (i % res[j] == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime)
            res.push_back(i);
    }

    return res;
}


// return prime factors of x
vector<int> getPrimeFactors(int x) {
    vector<int> res;

    int root = int(sqrt(x));
    if (root >= 2) {
        while (x % 2 == 0) {
            x /= 2;
            res.push_back(2);
        }
    }

    for (int i = 3; i <= root; i += 2) {
        while (x % i == 0) {
            x /= i;
            res.push_back(i);
        }
    }

    if (x > 1)
        res.push_back(x);

    return res;
}

// return prime factor and power of x
vector<pair<int, int>> getPrimeFactors2(int x) {
    vector<pair<int, int>> res;

    int root = int(sqrt(x));
    if (root >= 2) {
        int cnt = 0;
        while (x % 2 == 0) {
            x /= 2;
            cnt++;
        }
        if (cnt > 0)
            res.push_back(make_pair(2, cnt));
    }

    for (int i = 3; i <= root; i += 2) {
        int cnt = 0;
        while (x % i == 0) {
            x /= i;
            cnt++;
        }
        if (cnt > 0)
            res.push_back(make_pair(i, cnt));
    }

    if (x > 1)
        res.push_back(make_pair(x, 1));

    return res;
}

// return all prime numbers and prime factors from 1 to n
void getPrimeFactors(int n, vector<int>& primes,
                     vector<vector<pair<int, int>>>& primeFactors) {
    primes.clear();
    primeFactors = vector<vector<pair<int, int>>>(n + 1);
    if (n < 2)
        return;

    if (n >= 2) {
        primes.push_back(2);
        primeFactors[2].push_back(make_pair(2, 1));
    }

    for (int i = 3; i <= n; i++) {
        bool isPrime = true;
        int root = int(sqrt(i));
        for (int j = 0; primes[j] <= root; j++) {
            if (i % primes[j] == 0) {
                primeFactors[i] = primeFactors[i / primes[j]];
                if (primeFactors[i][0].first == primes[j])
                    primeFactors[i][0].second++;
                else
                    primeFactors[i].insert(primeFactors[i].begin(),
                                           make_pair(primes[j], 1));
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            primes.push_back(i);
            primeFactors[i].push_back(make_pair(i, 1));
        }
    }
}


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
