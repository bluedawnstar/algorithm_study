#pragma once

#include "intMod.h"

// finds all solutions to ax = b (mod M)
template <typename T, typename U>
inline vector<T> solveModularEq(T a, T b, U M) {
    vector<T> res;

    T x, y;
    T g = extGcd(a, M, x, y);
    if (!(b % g)) {
        x = (x * (b / g) % M + M) % M;
        for (int i = 0; i < g; i++)
            res.push_back((x + i * (M / g) % M + M) % M);
    }
    sort(res.begin(), res.end());

    return res;
}

// https://e-maxx-eng.appspot.com/algebra/discrete-log.html

// find x satisfying  a^x = b (mod M), a and M are coprime
struct DiscreteLogarithm {
    // M is 64bit integer
    static long long solve64bit(long long a, long long b, long long M) {
        int n = (int)ceil(sqrt(M)) + 1;

        long long an = 1;
        for (int i = 0; i < n; i++)
            an = modMul(an, a, M);

        unordered_map<long long, long long> vals;
        long long cur = an;
        for (int i = 1; i <= n; i++) {
            if (vals.find(cur) == vals.end())
                vals[cur] = i;
            cur = modMul(cur, an, M);
        }

        cur = b;
        for (int i = 0; i <= n; i++) {
            if (vals.find(cur) != vals.end()) {
                long long ans = vals[cur] * n - i;
                if (ans < M)
                    return ans;
            }
            cur = modMul(cur, a, M);
        }

        return -1;
    }

    // M is 32bit integer
    static int solve32bit(long long a, long long b, int M) {
        int am = int(a % M);
        int bm = int(b % M);
        int n = (int)ceil(sqrt(M)) + 1;

        long long an = 1;
        for (int i = 0; i < n; i++)
            an = (an * am) % M;

        unordered_map<long long, int> vals;
        long long cur = an;
        for (int i = 1; i <= n; i++) {
            if (vals.find(cur) == vals.end())
                vals[cur] = i;
            cur = (cur * an) % M;
        }

        cur = bm;
        for (int i = 0; i <= n; i++) {
            if (vals.find(cur) != vals.end()) {
                int ans = vals[cur] * n - i;
                if (ans < M)
                    return ans;
            }
            cur = (cur * am) % M;
        }

        return -1;
    }
};

