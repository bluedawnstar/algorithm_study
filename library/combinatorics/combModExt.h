#pragma once

#include "combMod.h"
#include "../integer/intMod.h"

// https://e-maxx-eng.appspot.com/algebra/module-inverse.html
// see CombModeGeneral below if M is neither prime nor coprime

// M = P^b (p is prime)
struct CombModPPow {
    int P, b;

    long long M;
    vector<long long> fact;
    vector<long long> pow;
    vector<int> c;
    vector<long long> g;

    // nCr (n <= maxN)
    CombModPPow(int maxN, int P, int b)
        : P(P), b(b), fact(maxN + 2), pow(maxN + 2), c(maxN + 2), g(maxN + 2) {
        M = (long long)ceil(::pow(P, b));

        // make factorial, power, g and c table
        fact[0] = 1;
        pow[0] = 1;
        g[0] = 1;
        c[0] = 0;
        for (int i = 1; i <= maxN; i++) {
            fact[i] = fact[i - 1] * i % M;
            pow[i] = pow[i - 1] * P % M;
            c[i] = c[i - 1];
            if (i % P) {
                g[i] = g[i - 1] * i % M;
            } else {
                int k = i;
                while (k % P == 0) {
                    ++c[i];
                    k /= P;
                }
                g[i] = g[i - 1] * k % M;
            }
        }
    }

    // O(logM), it can be reduced to O(1) if precompute all inverses
    long long comb(int n, int r) {
        if (n < r)
            return 0;
        if (n == 0 || r == 0 || n == r)
            return 1;

        int cn = c[n] - c[r] - c[n - r];
        if (cn >= b)
            return 0;

        return g[n] * modInvIter(g[r], M) % M * modInvIter(g[n - r], M) % M * pow[cn] % M;
    }
};

// M is normal integer (neither prime number nor coprime number)
// M = p^a * q^b * r^c ... (p, q, r are prime numbers)
struct CombModGeneral {
    long long M;
    int PN;                             // the number of prime factors
    vector<pair<int, int>> P;     // prime factors

    vector<long long> fact;
    vector<vector<long long>> pow;
    vector<vector<int>> c;
    vector<long long> g;

    // nCr (n <= maxN), P is prime factors
    CombModGeneral(int maxN, long long M, const vector<pair<int, int>>& P)
        : M(M), PN((int)P.size()), P(P), fact(maxN + 2), pow(maxN + 2, vector<long long>(PN)),
          c(maxN + 2, vector<int>(PN)), g(maxN + 2) {

        // make factorial, power, g and c table
        fact[0] = 1;
        g[0] = 1;
        for (int j = 0; j < PN; j++) {
            pow[0][j] = 1;
            c[0][j] = 0;
        }
        for (int i = 1; i <= maxN; i++) {
            fact[i] = fact[i - 1] * i % M;
            c[i] = c[i - 1];

            int divisor = 1;
            for (int j = 0; j < (int)P.size(); j++) {
                pow[i][j] = pow[i - 1][j] * P[j].first % M;
                if (i % P[j].first == 0) {
                    for (int k = i; k % P[j].first == 0; k /= P[j].first) {
                        ++c[i][j];
                        divisor *= P[j].first;
                    }
                }
            }
            g[i] = g[i - 1] * (i / divisor) % M;
        }
    }

    // O(logM), it can be reduced to O(1) if precompute all inverses
    long long comb(int n, int r) {
        if (n < r)
            return 0;
        if (n == 0 || r == 0 || n == r)
            return 1;

        long long powP = 1ll;

        bool multiple = true;
        for (int i = 0; i < PN; i++) {
            int cn = c[n][i] - c[r][i] - c[n - r][i];
            if (cn < P[i].second)
                multiple = false;
            powP = (powP * pow[cn][i]) % M;
        }

        if (multiple)
            return 0;

        return g[n] * modInvIter(g[r], M) % M * modInvIter(g[n - r], M) % M * powP % M;
    }
};
