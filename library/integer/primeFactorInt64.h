#pragma once

#include "primeFactor.h"

struct PrimeFactorizerInt64 {
    int N;
    MinFactors minFactors;

    void init(int n) {
        N = n;
        minFactors.build(N);
    }

    vector<pair<long long, int>> factorize(long long n) {
        vector<pair<long long, int>> ans;

        if (n <= 1)
            return ans;

        vector<long long> temp;
        while (n % 2 == 0) {
            temp.push_back(2);
            n >>= 1;
        }

        int m = 0;
        vector<long long> s(70);

        pollardRho(n, m, s);
        for (int i = 0; i < m; i++) {
            temp.push_back(s[i]);
        }
        sort(temp.begin(), temp.end());

        for (int i = 0; i < int(temp.size()); ++i) {
            int j = i, e = 0;
            while (j < int(temp.size()) && temp[j] == temp[i]) {
                e += 1;
                j += 1;
            }
            ans.push_back({ temp[i], e });
            i = j - 1;
        }
        return ans;
    }

protected:
    void pollardRho(long long n, int& m, vector<long long>& s) {
        long long x;
        if (n == 1)
            return;

        if (n <= N) {
            while (n != 1) {
                int p = minFactors.minFactors[int(n)];
                while (n % p == 0) {
                    n /= p;
                    s[m++] = p;
                }
            }
            return;
        }

        while (!millerRabin(n)) {
            int c;
            for (c = 1, x = n; x == n; c = 1 + randInt() % (n - 1)) {
                x = go(n, c);
            }
            if (x < 0)
                break;
            n /= x;
            pollardRho(x, m, s);
        }
        if (n > 1)
            s[m++] = n;
    }

    bool millerRabin(long long n) {
        if (n <= N)
            return minFactors.minFactors[int(n)] == n;
        return !witness(28087, n);
    }

    static long long func(long long x, long long n, int c) {
        long long res = mulMod(x, x, n) + c;
        return (res >= n ? res % n : res);
    }

    static long long go(long long n, int c) {
        long long x, y, d = 1;
        x = y = rand() & 0x7fff;
        if (x >= n) {
            x %= n;
            y %= n;
        }
        while (d == 1) {
            x = func(x, n, c);
            y = func(func(y, n, c), n, c);
            d = gcd(abs(y - x), n);
        }
        if (d != n)
            return d;
        return d;
    }

private:
    static long long randLL() {
        return (rand() & 0x7fff) * (1ll << 48)
             + (rand() & 0x7fff) * (1ll << 32)
             + (rand() & 0x7fff) * (1ll << 16)
             + (rand() & 0x7fff);
    }

    static int randInt() {
        return (rand() & 0x7fff) * (rand() & 0x7fff);
    }

    static long long gcd(long long p, long long q) {
        return q == 0 ? p : gcd(q, p % q);
    }

    static long long mulMod(long long a, long long b, long long M) {
        long long x = 0, y = a % M;
        while (b > 0) {
            if (b & 1)
                x = (x + y) % M;

            y = (y << 1) % M;
            b >>= 1;
        }
        return x % M;
    }

    static long long power(long long a, long long x, long long M) {
        long long res = 1;

        a = a % M;
        while (x > 0) {
            if (x & 1)
                res = static_cast<long long>(mulMod(res, a, M));

            x >>= 1;
            a = static_cast<long long>(mulMod(a, a, M));
        }

        return res;
    }

    static bool witness(long long a, long long n) {
        long long x, y, u = n - 1, t = 0;
        while (u % 2 == 0) {
            u >>= 1;
            t += 1;
        }
        x = power(a, u, n);
        while (t--) {
            y = x;
            x = power(x, 2, n);
            if (x == 1 && y != 1 && y != n - 1)
                return 1;
        }
        return x != 1;
    }
};
