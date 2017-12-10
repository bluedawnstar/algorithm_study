#pragma once

#include "intMod.h"
#include "primeNumberBasic.h"
#include "primeFactor.h"

struct PrimitiveRootFinder {
    static int find(int n) {
        unordered_set<int> s;

        if (isPrimeNumber(n) == false)
            return -1;

        int phi = n - 1;
        findPrimefactors(s, phi);

        for (int r = 2; r <= phi; r++) {
            bool flag = false;
            for (auto it = s.begin(); it != s.end(); it++) {
                if (modPow(r, phi / (*it), n) == 1) {
                    flag = true;
                    break;
                }
            }

            if (flag == false)
                return r;
        }

        return -1;
    }

private:
    static void findPrimefactors(unordered_set<int> &s, int n) {
        while (n % 2 == 0) {
            s.insert(2);
            n = n / 2;
        }

        int root = int(sqrt(n));
        for (int i = 3; i <= root; i += 2) {
            while (n % i == 0) {
                s.insert(i);
                n = n / i;
            }
        }

        if (n > 2)
            s.insert(n);
    }
};
