#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// check if n is a prime number
bool isPrimeNumber(int n) {
    if (n < 2)
        return false;
    else if (n == 2)
        return true;
    else if ((n & 1) == 0)
        return false;

    int root = int(sqrt(n));
    for (int i = 3; i <= root; i += 2) {
        if (n % i == 0)
            return false;
    }

    return true;
}

// return prime factors of n
vector<int> getPrimeFactor(int n) {
    vector<int> res;

    int root = int(sqrt(n));
    if (root >= 2) {
        while (n % 2 == 0) {
            n /= 2;
            res.push_back(2);
        }
    }

    for (int i = 3; i <= root; i += 2) {
        while (n % i == 0) {
            n /= i;
            res.push_back(i);
        }
    }

    if (n > 1)
        res.push_back(n);

    return res;
}

// return prime factor and power of n
vector<pair<int, int>> getPrimeFactor2(int n) {
    vector<pair<int, int>> res;

    int root = int(sqrt(n));
    if (root >= 2) {
        int cnt = 0;
        while (n % 2 == 0) {
            n /= 2;
            cnt++;
        }
        if (cnt > 0)
            res.push_back(make_pair(2, cnt));
    }

    for (int i = 3; i <= root; i += 2) {
        int cnt = 0;
        while (n % i == 0) {
            n /= i;
            cnt++;
        }
        if (cnt > 0)
            res.push_back(make_pair(i, cnt));
    }

    if (n > 1)
        res.push_back(make_pair(n, 1));

    return res;
}


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <iostream>
#include "../common/iostreamhelper.h"

#define NN  1000000

void testPrimeNumberBasic() {
    return; //TODO: if you want to test functions of this file, make this line to a comment.

    clock_t start;

    cout << "--- test isPrimeNumber() from 0 to 100 ---" << endl;
    for (int i = 0; i <= 100; i++) {
        if (isPrimeNumber(i))
            cout << i << ", ";
    }
    cout << endl;

    cout << "--- test getPrimeFactor() from 0 to 100 ---" << endl;
    for (int i = 0; i <= 100; i++)
        cout << i << ": " << getPrimeFactor(i) << endl;

    cout << "--- test getPrimeFactor2() from 0 to 100 ---" << endl;
    for (int i = 0; i <= 100; i++)
        cout << i << ": " << getPrimeFactor2(i) << endl;

    cout << "--- performance test about prime factorization functions ---" << endl;

    start = clock();
    for (int i = 0; i <= NN; i++)
        getPrimeFactor(i);
    cout << "getPrimeFactor()'s elapsed time from 1 to " << NN << " = " << double(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;

    start = clock();
    for (int i = 0; i <= NN; i++)
        getPrimeFactor2(i);
    cout << "getPrimeFactor2()'s elapsed time from 1 to " << NN << " = " << double(clock() - start) / CLOCKS_PER_SEC << " sec" << endl;
}
