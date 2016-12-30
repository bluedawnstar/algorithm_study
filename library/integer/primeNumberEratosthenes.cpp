#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// get all prime numbers
vector<bool> eratosthenes(int n) {
    vector<bool> res(n + 1, true);
    res[0] = false;
    res[1] = false;

    if (n >= 4) {
        for (int j = 2 * 2; j <= n; j += 2)
            res[j] = false;
    }

    int root = (int)sqrt(n);
    for (int i = 3; i <= root; i += 2) {
        if (res[i]) {
            for (int j = i * i; j >= 0 && j <= n; j += i)
                res[j] = false;
        }
    }

    return res;
}

//--------- Min Prime Factors -------------------------------------------------

vector<int> getMinFactors(int n) {
    vector<int> res(n + 1);
    res[0] = -1;
    res[1] = -1;

    for (int i = 2; i <= n; i++)
        res[i] = i;

    if (n >= 4) {
        for (int j = 2 * 2; j <= n; j += 2)
            res[j] = 2;
    }

    int root = (int)sqrt(n);
    for (int i = 3; i <= root; i += 2) {
        if (res[i] == i) {
            for (int j = i * i; j >= 0 && j <= n; j += i) {
                if (res[j] == j)
                    res[j] = i;
            }
        }
    }

    return res;
}

bool isPrimeNumber(const vector<int>& minFactors, int x) {
    return minFactors[x] == x;
}

vector<int> getPrimeFactors(const vector<int>& minFactors, int x) {
    vector<int> res;
    while (x > 1) {
        res.push_back(minFactors[x]);
        x /= minFactors[x];
    }

    return res;
}

vector<pair<int, int>> getPrimeFactors2(const vector<int>& minFactors, int x) {
    vector<pair<int, int>> res;

    while (x > 1) {
        if (!res.empty() && res.back().first == minFactors[x])
            res.back().second++;
        else
            res.push_back(make_pair(minFactors[x], 1));
        x /= minFactors[x];
    }

    return res;
}


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <iostream>
#include "../common/iostreamhelper.h"

#define NN  1000000

void testPrimeNumberEratosthenes() {
    return; //TODO: if you want to test functions of this file, make this line to a comment.

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
