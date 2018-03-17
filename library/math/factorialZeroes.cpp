#include <vector>
#include <algorithm>

using namespace std;

#include "factorialZeroes.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static long long f(int x) {
    long long res = 1;
    for (int i = 2; i <= x; i++)
        res *= i;
    return res;
}

static long long f(int x1, int x2) {
    long long res = 1;
    for (int i = x1; i <= x2; i++)
        res *= i;
    return res;
}

static int countTrailingZeroes(long long x) {
    int res = 0;
    while (x && x % 10 == 0) {
        res++;
        x /= 10;
    }
    return res;
}

void testFactorialZeroes() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Factorial Zeroes ------------------------------" << endl;

    for (int i = 7; i <= 10; i++) {
        assert(countTrailingZeroes(f(i)) == countTrailingZeroesOfFactorial(i));
        for (int j = 1; j <= i; j++) {
            assert(countTrailingZeroes(f(j, i)) == countTrailingZeroesOfFactorial(j, i));
        }
    }

    for (int i = 0; i < 1000000; i++) {
        long long L = RandInt64::get();
        long long R = RandInt64::get();
        if (L > R)
            swap(L, R);
        assert(countTrailingZeroesOfFactorial(0, L) == countTrailingZeroesOfFactorial(L));
        assert(countTrailingZeroesOfFactorial(0, R) == countTrailingZeroesOfFactorial(R));
    }

    cout << "OK!" << endl;
}
