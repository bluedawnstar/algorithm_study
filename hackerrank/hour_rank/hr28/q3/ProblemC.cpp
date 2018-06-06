#include <memory.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cctype>
#include <cstring>
#include <climits>
#include <cmath>
#include <vector>
#include <string>
#include <memory>
#include <numeric>
#include <limits>
#include <functional>
#include <tuple>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iostream>

using namespace std;

#define PROFILE_START(i)    clock_t start##i = clock()
#define PROFILE_STOP(i)     fprintf(stderr, "elapsed time (" #i ") = %f\n", double(clock() - start##i) / CLOCKS_PER_SEC)

typedef long long           ll;
typedef unsigned long long  ull;

// with sqrt decomposition
struct BitSetRangeQuery {
    typedef unsigned long long ValueT;
    typedef long long SignedValueT;

    static const int BIT_SIZE = sizeof(unsigned long long) * 8;
    static const unsigned long long BIT_ALL = 0xffffffffffffffffull;
    static const unsigned long long BIT_ONE = 1ull;

    static const int INDEX_MASK = 0x3F;
    static const int INDEX_SHIFT = 6;

    int N;
    vector<ValueT> values;

    int sqrtN;
    vector<bool> blockInv;
    vector<int> blockOne;

    void init(int n) {
        sqrtN = (int)sqrt(n);
        N = ((n + sqrtN - 1) / sqrtN) * sqrtN;

        values = vector<ValueT>((N + BIT_SIZE - 1) / BIT_SIZE);

        blockInv = vector<bool>(N / sqrtN);
        blockOne = vector<int>(N / sqrtN);
    }

    template <typename T>
    void update(const vector<T>& v, T bitMask) {
        for (int i = 0, idx = 0; i < N; i += BIT_SIZE) {
            ValueT x = 0;
            for (int j = 0; j < BIT_SIZE && idx < (int)v.size(); j++, idx++) {
                if (v[idx] & bitMask)
                    x |= BIT_ONE << j;
            }
            values[i >> INDEX_SHIFT] = x;
        }

        for (int i = 0; i < (int)blockOne.size(); i++)
            blockOne[i] = doCount(i * sqrtN, (i + 1) * sqrtN - 1);
    }

    bool test(int pos) const {
        bool res = (values[pos >> INDEX_SHIFT] & (BIT_ONE << (pos & INDEX_MASK))) != 0;
        if (blockInv[pos / sqrtN])
            res = !res;
        return res;
    }

    int count(int left, int right) {
        int lIdx = left / sqrtN;
        int rIdx = right / sqrtN;

        int res = 0;
        if (lIdx == rIdx) {
            if (left % sqrtN == 0 && (right + 1) % sqrtN == 0)
                res += blockInv[lIdx] ? (sqrtN - blockOne[lIdx]) : blockOne[lIdx];
            else {
                apply(lIdx);
                res += doCount(left, right);
            }
        } else {
            if (left % sqrtN == 0)
                res += blockInv[lIdx] ? (sqrtN - blockOne[lIdx]) : blockOne[lIdx];
            else {
                apply(lIdx);
                res += doCount(left, (lIdx + 1) * sqrtN - 1);
            }
            if ((right + 1) % sqrtN == 0)
                res += blockInv[rIdx] ? (sqrtN - blockOne[rIdx]) : blockOne[rIdx];
            else {
                apply(rIdx);
                res += doCount(rIdx * sqrtN, right);
            }
            for (int i = lIdx + 1; i < rIdx; i++) {
                res += blockInv[i] ? (sqrtN - blockOne[i]) : blockOne[i];
            }
        }
        return res;
    }

    void flip(int left, int right) {
        int lIdx = left / sqrtN;
        int rIdx = right / sqrtN;

        if (lIdx == rIdx) {
            if (left % sqrtN == 0 && (right + 1) % sqrtN == 0)
                blockInv[lIdx] = !blockInv[lIdx];
            else
                doFlip(left, right, lIdx);
        } else {
            if (left % sqrtN == 0)
                blockInv[lIdx] = !blockInv[lIdx];
            else
                doFlip(left, (lIdx + 1) * sqrtN - 1, lIdx);
            if ((right + 1) % sqrtN == 0)
                blockInv[rIdx] = !blockInv[rIdx];
            else
                doFlip(rIdx * sqrtN, right, rIdx);
            for (int i = lIdx + 1; i < rIdx; i++)
                blockInv[i] = !blockInv[i];
        }
    }

    //-----------------------------------------------------

    int doCount(int left, int right) {
        int lIndex = left >> INDEX_SHIFT;
        int rIndex = right >> INDEX_SHIFT;
        int lOffset = left & INDEX_MASK;
        int rOffset = (right + 1) & INDEX_MASK;
        ValueT lMask = ~((BIT_ONE << lOffset) - BIT_ONE);
        ValueT rMask = rOffset ? (BIT_ONE << rOffset) - BIT_ONE : BIT_ALL;

        if (lIndex == rIndex)
            return popcount(values[lIndex] & lMask & rMask);

        int res = popcount(values[lIndex] & lMask);
        res += popcount(values[rIndex] & rMask);
        for (int i = lIndex + 1; i < rIndex; i++)
            res += popcount(values[i]);
        return res;
    }

    void doFlip(int left, int right, int blockId) {
        int lIndex = left >> INDEX_SHIFT;
        int rIndex = right >> INDEX_SHIFT;
        int lOffset = left & INDEX_MASK;
        int rOffset = (right + 1) & INDEX_MASK;
        ValueT lMask = ~((BIT_ONE << lOffset) - BIT_ONE);
        ValueT rMask = rOffset ? (BIT_ONE << rOffset) - BIT_ONE : BIT_ALL;

        if (lIndex == rIndex) {
            lMask &= rMask;
            blockOne[blockId] += popcount(~values[lIndex] & lMask) - popcount(values[lIndex] & lMask);
            values[lIndex] ^= lMask;
        } else {
            blockOne[blockId] += popcount(~values[lIndex] & lMask) - popcount(values[lIndex] & lMask);
            values[lIndex] ^= lMask;

            blockOne[blockId] += popcount(~values[rIndex] & rMask) - popcount(values[rIndex] & rMask);
            values[rIndex] ^= rMask;
            for (int i = lIndex + 1; i < rIndex; i++) {
                blockOne[blockId] += popcount(~values[i]) - popcount(values[i]);
                values[i] = ~values[i];
            }
        }
    }

    void apply(int blockId) {
        if (blockInv[blockId]) {
            doFlip(blockId * sqrtN, (blockId + 1) * sqrtN - 1, blockId);
            blockInv[blockId] = false;
        }
    }

    static int popcount(unsigned x) {
#ifndef __GNUC__
        return (int)__popcnt(x);
#else
        return __builtin_popcount(x);
#endif
    }

    static int popcount(unsigned long long x) {
#if defined(_M_X64)
        return (int)__popcnt64(x);
#elif defined(__GNUC__)
        return __builtin_popcountll(x);
#else
        return (int)__popcnt(unsigned(x)) + (int)__popcnt(unsigned(x >> 32));
#endif
    }
};

// Complete the xorQueries function below.
vector<long long> xorQueries(vector<int> a, int m, int p) {
    // Return an array consisting of the answers of all type-2 queries.
    int N = (int)a.size();

    vector<int> X(N);
    int t = 0;
    for (int i = 0; i < p; i++)
        t ^= a[i];

    X[0] = t;
    for (int i = 1, j = i + p - 1; i < N; i++, j++) {
        t ^= a[i - 1];
        if (j < N)
            t ^= a[j];
        X[i] = t;
    }

#define BITS    20

    BitSetRangeQuery bs[BITS];
    for (int i = 0; i < BITS; i++) {
        bs[i].init(N);
        bs[i].update(X, 1 << i);
    }

    vector<long long> res;
    for (int i = 0; i < m; i++) {
        int type;
        cin >> type;
        if (type == 1) {
            int idx, val;
            cin >> idx >> val;
            idx--;

            if (val == 0)
                continue;

            int l = max(0, idx - p + 1);
            int r = min(idx, N - p);
            if (l > r)
                continue;

            for (int j = 0; j < BITS; j++) {
                if (val & (1 << j))
                    bs[j].flip(l, r);
            }
        } else {
            int l, r;
            cin >> l >> r;
            l--, r--;

            if (r > N - p)
                r = N - p;

            long long y = 0;
            if (l <= r) {
                for (int j = 0; j < BITS; j++)
                    y += (long long)bs[j].count(l, r) << j;
            }
            res.push_back(y);
        }
    }

    return res;
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, P;
    cin >> N >> M >> P;

    vector<int> a(N);
    for (int i = 0; i < N; i++)
        cin >> a[i];

    auto res = xorQueries(a, M, P);
    for (auto i : res)
        cout << i << endl;

    return 0;
}
