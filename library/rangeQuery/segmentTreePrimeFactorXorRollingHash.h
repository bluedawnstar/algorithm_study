#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

struct PrimeFactorXorRollingHash {
    static const int mod = 1'000'000'007;

    static const int XOR_HASH_PRIME_1 = 1000003;
    static const int XOR_HASH_PRIME_2 = 1000033;

    vector<int> primes;
    vector<int> minPrimeFactors;

    vector<array<int, 2>> rollingHash;

    void build(int maxX) {
        primes.clear();
        minPrimeFactors = vector<int>(maxX + 1);

        primes.push_back(2);
        minPrimeFactors[2] = 2;

        for (int i = 3; i <= maxX; i++) {
            int root = int(sqrt(i));
            for (int j = 0; primes[j] <= root; j++) {
                if (i % primes[j] == 0) {
                    minPrimeFactors[i] = primes[j];
                    break;
                }
            }
            if (!minPrimeFactors[i]) {
                primes.push_back(i);
                minPrimeFactors[i] = i;
            }
        }

        rollingHash.resize(maxX + 1);
        rollingHash[0][0] = 1;
        rollingHash[0][1] = 1;
        for (int i = 1; i <= maxX; i++) {
            rollingHash[i][0] = int(1ll * rollingHash[i - 1][0] * XOR_HASH_PRIME_1 % mod);
            rollingHash[i][1] = int(1ll * rollingHash[i - 1][1] * XOR_HASH_PRIME_2 % mod);
        }
    }

    unsigned long long getHash(int x, int hashIndex, int k) {
        unsigned long long hash = 0;

        while (x > 1) {
            int cnt = 0;
            int pf = minPrimeFactors[x];
            while (x % pf == 0) {
                cnt++;
                x /= pf;
            }
            hash = xorAdd(hash, xorMul(rollingHash[pf][hashIndex], cnt, k), k);
        }
        return hash;
    }

    //---

    static unsigned long long xorAdd(unsigned long long a, unsigned long long b, int k) {
        if (k == 1)
            return 0;

        unsigned long long c = 0;
        unsigned long long kk = 1;
        while (a > 0 || b > 0) {
            unsigned long long r1 = a % k;
            unsigned long long r2 = b % k;
            unsigned long long r = (r1 + r2) % k;
            c += r * kk;
            kk *= k;
            a /= k, b /= k;
        }
        return c;
    }

    static unsigned long long xorMul(unsigned long long a, unsigned long long b, int k) {
        if (k == 1)
            return 0;

        unsigned long long c = 0;
        b %= k;
        unsigned long long kk = 1;
        while (a > 0) {
            unsigned long long r = a % k;
            r = (1ll * r * b) % k;
            c += r * kk;
            kk *= k;
            a /= k;
        }
        return c;
    }
};

// https://www.hackerearth.com/problem/algorithm/a-kth-root-subarray-09f931a5/description/
/*
    0. precondition
        1 <= A[i] <= maxX

    1. update #1 --> update()
           A[L .. R] = x^y
      (or) A[L .. R] = x

    2. update #2 --> add()
           A[L .. R] *= x^y
      (or) A[L .. R] *= x

    3. query
           A[L] * A[L+1] * ... * A[R-1] * A[R] = a^k       (a is any integer)
*/
struct SegmentTreePrimeFactorXorRollingHash {
    enum LazyT {
        lzNone,
        lzSet,
        lzAdd
    };

    PrimeFactorXorRollingHash hash;

    int N;
    vector<array<unsigned long long, 2>> values;
    vector<array<unsigned long long, 2>> lazy;
    vector<int> lazyType;

    int K;

    SegmentTreePrimeFactorXorRollingHash(int maxX) : N(0) {
        hash.build(maxX);
    }

    void init(int n, int k) {
        N = n;
        K = k;

#ifndef __GNUC__
        int size = 1 << (32 - int(_lzcnt_u32(n - 1)));
#else
        int size = 1 << (32 - __builtin_clz(n - 1));
#endif

        values.resize(size * 2);
        lazyType.resize(size);
        lazy.resize(size);
    }

    void build(int value, int n, int k) {
        init(n, k);
        dfsBuild(1, 0, n - 1, value);
    }

    void build(const int A[], int n, int k) {
        init(n, k);
        dfsBuild(1, 0, n - 1, A);
    }

    void build(const vector<int>& A, int k) {
        int n = int(A.size());
        init(n, k);
        dfsBuild(1, 0, n - 1, A.data());
    }

    //---

    // A[left...right] = x^y
    void update(int left, int right, int x, int y) {
        unsigned long long hash1 = hash.xorMul(hash.getHash(x, 0, K), y, K);
        unsigned long long hash2 = hash.xorMul(hash.getHash(x, 1, K), y, K);
        dfsUpdate(1, 0, N - 1, left, right, hash1, hash2);
    }

    // A[left...right] = x
    void update(int left, int right, int x) {
        unsigned long long hash1 = hash.getHash(x, 0, K);
        unsigned long long hash2 = hash.getHash(x, 1, K);
        dfsUpdate(1, 0, N - 1, left, right, hash1, hash2);
    }

    // A[left...right] *= x^y
    void add(int left, int right, int x, int y) {
        unsigned long long hash1 = hash.xorMul(hash.getHash(x, 0, K), y, K);
        unsigned long long hash2 = hash.xorMul(hash.getHash(x, 1, K), y, K);
        dfsAdd(1, 0, N - 1, left, right, hash1, hash2);
    }

    // A[left...right] *= x
    void add(int left, int right, int x) {
        unsigned long long hash1 = hash.getHash(x, 0, K);
        unsigned long long hash2 = hash.getHash(x, 1, K);
        dfsAdd(1, 0, N - 1, left, right, hash1, hash2);
    }

    // if A[L] * A[L+1] * ... * A[R-1] * A[R] = a^K, then return 0
    pair<unsigned long long, unsigned long long> query(int left, int right) {
        return dfsQuery(1, 0, N - 1, left, right);
    }

private:
    void dfsBuild(int node, int nodeLeft, int nodeRight, int value) {
        if (nodeLeft == nodeRight) {
            values[node][0] = hash.getHash(value, 0, K);
            values[node][1] = hash.getHash(value, 1, K);
            return;
        }

        int mid = (nodeLeft + nodeRight) >> 1;
        int L = node << 1;
        int R = L + 1;

        dfsBuild(L, nodeLeft, mid, value);
        dfsBuild(R, mid + 1, nodeRight, value);
        values[node][0] = hash.xorAdd(values[L][0], values[R][0], K);
        values[node][1] = hash.xorAdd(values[L][1], values[R][1], K);
    }

    void dfsBuild(int node, int nodeLeft, int nodeRight, const int A[]) {
        if (nodeLeft == nodeRight) {
            values[node][0] = hash.getHash(A[nodeLeft], 0, K);
            values[node][1] = hash.getHash(A[nodeLeft], 1, K);
            return;
        }

        int mid = (nodeLeft + nodeRight) >> 1;
        int L = node << 1;
        int R = L + 1;

        dfsBuild(L, nodeLeft, mid, A);
        dfsBuild(R, mid + 1, nodeRight, A);
        values[node][0] = hash.xorAdd(values[L][0], values[R][0], K);
        values[node][1] = hash.xorAdd(values[L][1], values[R][1], K);
    }

    void pushDown(int node, int nodeLeft, int nodeRight, int parentNode) {
        if (lazyType[parentNode] == lzSet) {
            if (nodeLeft == nodeRight) {
                values[node] = lazy[parentNode];
            } else {
                lazyType[node] = lzSet;
                lazy[node] = lazy[parentNode];
                values[node][0] = hash.xorMul(lazy[parentNode][0], nodeRight - nodeLeft + 1, K);
                values[node][1] = hash.xorMul(lazy[parentNode][1], nodeRight - nodeLeft + 1, K);
            }
        } else {
            if (nodeLeft == nodeRight) {
                values[node][0] = hash.xorAdd(values[node][0], lazy[parentNode][0], K);
                values[node][1] = hash.xorAdd(values[node][1], lazy[parentNode][1], K);
            } else {
                if (lazyType[node] == lzNone) {
                    lazyType[node] = lzAdd;
                    lazy[node] = lazy[parentNode];
                } else {
                    lazy[node][0] = hash.xorAdd(lazy[node][0], lazy[parentNode][0], K);
                    lazy[node][1] = hash.xorAdd(lazy[node][1], lazy[parentNode][1], K);
                }
                values[node][0] = hash.xorAdd(values[node][0],
                    hash.xorMul(lazy[parentNode][0], nodeRight - nodeLeft + 1, K), K);
                values[node][1] = hash.xorAdd(values[node][1],
                    hash.xorMul(lazy[parentNode][1], nodeRight - nodeLeft + 1, K), K);
            }
        }
    }

    void dfsUpdate(int node, int nodeLeft, int nodeRight, int indexL, int indexR, unsigned long long val1, unsigned long long val2) {
        if (indexR < nodeLeft || nodeRight < indexL)
            return;

        if (nodeLeft == nodeRight) {
            values[node][0] = val1;
            values[node][1] = val2;
            return;
        }

        if (indexL <= nodeLeft && nodeRight <= indexR) {
            lazyType[node] = lzSet;
            lazy[node][0] = val1;
            lazy[node][1] = val2;
            values[node][0] = hash.xorMul(val1, nodeRight - nodeLeft + 1, K);
            values[node][1] = hash.xorMul(val2, nodeRight - nodeLeft + 1, K);
            return;
        }

        int mid = (nodeLeft + nodeRight) >> 1;
        int L = node << 1;
        int R = L + 1;

        if (lazyType[node]) {
            pushDown(L, nodeLeft, mid, node);
            pushDown(R, mid + 1, nodeRight, node);
            lazy[node][0] = lazy[node][1] = 0;
            lazyType[node] = lzNone;
        }

        dfsUpdate(L, nodeLeft, mid, indexL, indexR, val1, val2);
        dfsUpdate(R, mid + 1, nodeRight, indexL, indexR, val1, val2);
        values[node][0] = hash.xorAdd(values[L][0], values[R][0], K);
        values[node][1] = hash.xorAdd(values[L][1], values[R][1], K);
    }

    void dfsAdd(int node, int nodeLeft, int nodeRight, int indexL, int indexR, unsigned long long val1, unsigned long long val2) {
        if (indexR < nodeLeft || nodeRight < indexL)
            return;

        if (nodeLeft == nodeRight) {
            values[node][0] = hash.xorAdd(values[node][0], val1, K);
            values[node][1] = hash.xorAdd(values[node][1], val2, K);
            return;
        }

        if (indexL <= nodeLeft && nodeRight <= indexR) {
            if (lazyType[node] == lzNone) {
                lazyType[node] = lzAdd;
                lazy[node][0] = val1;
                lazy[node][1] = val2;
            } else {
                lazy[node][0] = hash.xorAdd(lazy[node][0], val1, K);
                lazy[node][1] = hash.xorAdd(lazy[node][1], val2, K);
            }
            values[node][0] = hash.xorAdd(values[node][0],
                hash.xorMul(val1, nodeRight - nodeLeft + 1, K), K);
            values[node][1] = hash.xorAdd(values[node][1],
                hash.xorMul(val2, nodeRight - nodeLeft + 1, K), K);
            return;
        }

        int mid = (nodeLeft + nodeRight) >> 1;
        int L = node << 1;
        int R = L + 1;

        if (lazyType[node]) {
            pushDown(L, nodeLeft, mid, node);
            pushDown(R, mid + 1, nodeRight, node);
            lazyType[node] = lzNone;
            lazy[node][0] = lazy[node][1] = 0;
        }
        dfsAdd(L, nodeLeft, mid, indexL, indexR, val1, val2);
        dfsAdd(R, mid + 1, nodeRight, indexL, indexR, val1, val2);

        values[node][0] = hash.xorAdd(values[L][0], values[R][0], K);
        values[node][1] = hash.xorAdd(values[L][1], values[R][1], K);
    }

    pair<unsigned long long, unsigned long long>
        dfsQuery(int node, int nodeLeft, int nodeRight, int indexL, int indexR) {
        if (indexR < nodeLeft || nodeRight < indexL)
            return make_pair(0, 0);

        if (indexL <= nodeLeft && nodeRight <= indexR)
            return make_pair(values[node][0], values[node][1]);

        int mid = (nodeLeft + nodeRight) >> 1;
        int L = node << 1;
        int R = L + 1;

        if (lazyType[node]) {
            pushDown(L, nodeLeft, mid, node);
            pushDown(R, mid + 1, nodeRight, node);

            values[node][0] = hash.xorAdd(values[L][0], values[R][0], K);
            values[node][1] = hash.xorAdd(values[L][1], values[R][1], K);
            lazy[node][0] = lazy[node][1] = 0;
            lazyType[node] = lzNone;
        }

        auto resL = dfsQuery(L, nodeLeft, mid, indexL, indexR);
        auto resR = dfsQuery(R, mid + 1, nodeRight, indexL, indexR);
        return make_pair(hash.xorAdd(resL.first, resR.first, K),
            hash.xorAdd(resL.second, resR.second, K));
    }
};
