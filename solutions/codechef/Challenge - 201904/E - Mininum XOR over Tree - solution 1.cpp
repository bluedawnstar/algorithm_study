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

struct BitVectorRank {
    static const int BIT_SIZE = sizeof(unsigned int) * 8;
    static const unsigned int BIT_ALL = (unsigned int)-1;

    static const int INDEX_MASK = 0x1F;
    static const int INDEX_SHIFT = 5;

    int                  N;
    int                  bitCount;
    vector<unsigned int> values;
    vector<int>          rank;

    BitVectorRank() {
        //init(0);
    }

    explicit BitVectorRank(int size) {
        init(size);
    }

    void init(int size) {
        N = size;
        bitCount = 0;
        values = vector<unsigned int>((size + BIT_SIZE - 1) / BIT_SIZE + 1);
        rank = vector<int>(values.size() + 1);
    }


    int size() const {
        return N;
    }

    void set() {
        int n = N >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            values[i] = BIT_ALL;

        int r = N & INDEX_MASK;
        if (r != 0)
            values[n] = (1u << r) - 1u;
    }

    void set(int pos) {
        values[pos >> INDEX_SHIFT] |= 1u << (pos & INDEX_MASK);
    }

    void reset() {
        fill(values.begin(), values.end(), 0u);
    }

    void reset(int pos) {
        values[pos >> INDEX_SHIFT] &= ~(1u << (pos & INDEX_MASK));
    }

    void set(int pos, bool val) {
        if (val)
            set(pos);
        else
            reset(pos);
    }

    unsigned int get(int pos) const {
        return values[pos >> INDEX_SHIFT] & (1u << (pos & INDEX_MASK));
    }

    bool test(int pos) const {
        return (values[pos >> INDEX_SHIFT] & (1u << (pos & INDEX_MASK))) != 0;
    }

    //--- after set ---

    void buildRank() {
        bitCount = 0;
        if (N == 0)
            return;
        for (int i = 0; i < int(values.size()); i++) {
            rank[i] = bitCount;
            bitCount += popcount(values[i]);
        }
        rank[values.size()] = bitCount;
    }

    int count() const {
        return bitCount;
    }

    // inclusive [0, pos]
    int rank1(int pos) const {
        if (pos < 0)
            return 0;
        else if (pos >= N - 1)
            return bitCount;
        int idx = ++pos >> INDEX_SHIFT;
        int off = pos & INDEX_MASK;
        return rank[idx] + popcount(values[idx] & ((1u << off) - 1u));
    }

    // inclusive [left, right]
    int rank1(int left, int right) const {
        return rank1(right) - rank1(left - 1);
    }

    // inclusive [0, pos]
    int rank0(int pos) const {
        if (pos < 0)
            return 0;
        return pos + 1 - rank1(pos);
    }

    // inclusive [left, right]
    int rank0(int left, int right) const {
        return rank0(right) - rank0(left - 1);
    }

    static int popcount(unsigned int x) {
#ifndef __GNUC__
        return int(__popcnt(x));
#else
        return __builtin_popcount(x);
#endif
    }
};

struct SparseTableMin {
    int N;
    vector<vector<int>> value;
    vector<int> H;

    SparseTableMin() {
    }

    SparseTableMin(const int a[], int n) {
        build(a, n);
    }

    explicit SparseTableMin(const vector<int>& a) {
        build(a);
    }


    void build(const int a[], int n) {
        this->N = n;

        H.resize(n + 1);
        H[1] = 0;
        for (int i = 2; i < int(H.size()); i++)
            H[i] = H[i >> 1] + 1;

        value.assign(H.back() + 1, vector<int>(n));
        for (int i = 0; i < n; i++)
            value[0][i] = a[i];

        for (int i = 1; i < int(value.size()); i++) {
            vector<int>& prev = value[i - 1];
            vector<int>& curr = value[i];
            for (int v = 0; v < n; v++) {
                if (v + (1 << (i - 1)) < n)
                    curr[v] = min(prev[v], prev[v + (1 << (i - 1))]);
                else
                    curr[v] = prev[v];
            }
        }
    }

    void build(const vector<int>& a) {
        build(&a[0], int(a.size()));
    }


    // inclusive
    int query(int left, int right) const {
        right++;
        if (right <= left)
            return INT_MAX;

        int k = H[right - left];
        const vector<int>& mink = value[k];
        return min(mink[left], mink[right - (1 << k)]);
    }
};

template <typename T>
struct WaveletMatrixArrayIndirect {
    static const T NaN = numeric_limits<T>::min();

    int                 N;
    int                 H;
    T                   maxVal;
    vector<vector<int>> indexes;    // MSB bit first
    vector<T>           values;

    WaveletMatrixArrayIndirect() {
    }

    vector<int> build(const vector<T>& in) {
        return build(&in[0], int(in.size()), in.empty() ? 0 : *max_element(in.begin(), in.end()));
    }

    vector<int> build(const vector<T>& in, int maxVal) {
        return build(&in[0], int(in.size()), maxVal);
    }

    vector<int> build(const T* first, int n) {
        return build(first, n, (n == 0) ? 0 : *max_element(first, first + n));
    }

    // return reordered indexes of values
    // reordered_values[i] = values[return_value[i]]
    vector<int> build(const T* first, int n, int maxVal) {
        this->N = n;
        this->maxVal = maxVal;

        H = 1;
        while (maxVal >= (T(1) << H))
            ++H;

        indexes = vector<vector<int>>(H, vector<int>(N + 1));

        vector<int> cur(N);
        vector<T> next(N);

        iota(cur.begin(), cur.end(), 0);
        for (int i = 0; i < H; i++) {
            T mask = T(1) << (H - i - 1);

            vector<int>& v = indexes[i];

            int zeroN = 0;
            for (int j = 0; j < N; j++) {
                v[j] = zeroN;
                zeroN += ((first[cur[j]] & mask) == 0);
            }
            v[N] = zeroN;

            int zeroPos = 0, onePos = zeroN;
            for (int j = 0; j < N; j++) {
                if (first[cur[j]] & mask) {
                    next[onePos++] = cur[j];
                } else {
                    next[zeroPos++] = cur[j];
                }
            }
            next.swap(cur);
        }

        values.resize(N);
        for (int i = 0; i < n; i++)
            values[i] = first[cur[i]];

        return cur;
    }


    int size() const {
        return N;
    }

    // (0 <= pos < N)
    // return (value, index)
    pair<T,int> get(int pos) const {
        T val = 0;
        for (int i = 0; i < H; i++) {
            const vector<int>& v = indexes[i];

            if (v[pos] == v[pos + 1]) {
                val = (val << 1) | 1;
                pos = v.back() + (pos - v[pos]);
            } else {
                val = val << 1;
                pos = v[pos];
            }
        }
        return make_pair(val, pos);
    }

    // inclusive (0 <= left <= right < N, 0 <= k)
    pair<T,pair<int,int>> kth(int left, int right, int k) const {
        if (k < 0 || right - left < k) {
            return make_pair(NaN, make_pair(-1, -1));
        }

        T val = 0;
        for (int i = 0; i < H; i++) {
            const vector<int>& v = indexes[i];

            int count = v[right + 1] - v[left];
            if (k >= count) {
                val = (val << 1) | 1;
                left = v.back() + (left - v[left]);
                right = v.back() + (right - v[right + 1]);
                k -= count;
            } else {
                val = val << 1;
                left = v[left];
                right = v[right + 1] - 1;
            }
        }
        return make_pair(val, make_pair(left, right));
    }

    // inclusive (0 <= left <= right < N)
    int countLessThanOrEqual(int left, int right, T val) const {
        auto t = countEx(left, right, val);
        return ::get<0>(t) + ::get<1>(t);
    }

    // inclusive (0 <= left <= right < N)
    int count(int left, int right, T val) const {
        return ::get<0>(countEx(left, right, val));
    }

    // inclusive (0 <= left <= right < N)
    int count(int left, int right, T valLow, T valHigh) const {
        auto tL = countEx(left, right, valLow);
        auto tH = countEx(left, right, valHigh);
        return ::get<0>(tH) + ::get<1>(tH) - ::get<1>(tL);
    }

    // return (the number of val, numbers less than val, numbers greater than val)
    // inclusive (0 <= left <= right < N)
    tuple<int, int, int> countEx(int left, int right, T val) const {
        if (val > maxVal) {
            return make_tuple(0, right - left + 1, 0);
        }

        int lt = 0, gt = 0;
        for (int i = 0; i < H; i++) {
            const vector<int>& v = indexes[i];

            if ((val >> (H - i - 1)) & 1) {
                int leftN = left - v[left];
                int rightN = right + 1 - v[right + 1];

                lt += (right - left + 1) - (rightN - leftN);
                left = v.back() + leftN;
                right = v.back() + rightN - 1;
            } else {
                int leftN = v[left];
                int rightN = v[right + 1];

                gt += (right - left + 1) - (rightN - leftN);
                left = leftN;
                right = rightN - 1;
            }
        }
        return make_tuple(right - left + 1, lt, gt);
    }

    //--- extended operations

    pair<T,pair<int,int>> findMaxXor(int left, int right, T k) const {
        T val = 0;

        T bit = T(1) << (H - 1);
        for (int i = 0; i < H; i++, bit >>= 1) {
            const vector<int>& v = indexes[i];

            int rangeN = right - left + 1;
            int count = v[right + 1] - v[left]; // zero count
            if (count == 0 || (count < rangeN && !(k & bit))) {
                val |= bit;
                left = v.back() + (left - v[left]);         // total 0 count + 1 count
                right = v.back() + (right - v[right + 1]);  // total 0 count + 1 count
            } else {
                left = v[left];                             
                right = v[right + 1] - 1;
            }
        }

        return make_pair(val, make_pair(left, right));
    }
};

#define MAXN    200000
#define MAXW    (1 << 20)
#define MAXK    (1 << 20)

int gN;
int gW[MAXN + 1];
vector<int> gE[MAXN + 1];

int gCurrTime = 0;
pair<int, int> gVisit[MAXN + 1];

int tW[MAXN + 1];
int tV[MAXN + 1];

WaveletMatrixArrayIndirect<int> gWM;
SparseTableMin gRMQ;

void dfsBuild(int u, int p) {
    gVisit[u].first = gCurrTime++;
    for (auto v : gE[u]) {
        if (v == p)
            continue;
        dfsBuild(v, u);
    }
    gVisit[u].second = gCurrTime;
}

void build() {
    gCurrTime = 0;
    dfsBuild(0, -1);

    for (int u = 0; u < gN; u++) {
        int t = gVisit[u].first;
        tW[t] = gW[u];
        tV[t] = u;
    }
    auto idx = gWM.build(tW, gN);
    vector<int> v(gN);
    for (int i = 0; i < gN; i++)
        v[i] = tV[idx[i]];
    gRMQ.build(v);
}

namespace FastIO {
    char gInBuff[1 << 25];
    const char* gInPtr;

    void init() {
        gInPtr = gInBuff;
        auto size = fread(gInBuff, 1, sizeof(gInBuff) - 4, stdin);
        gInBuff[size] = 0;
    }

    int readInt() {
        int u = 0;

        while (*gInPtr && *gInPtr <= 32)
            ++gInPtr;

        while (*gInPtr >= '0' && *gInPtr <= '9')
            u = (u << 3) + (u << 1) + (*gInPtr++ - '0');

        return u;
    }
}

int main(void) {
    FastIO::init();

    int T = FastIO::readInt();

    while (T-- > 0) {
        int Q;

        gN = FastIO::readInt();
        Q = FastIO::readInt();

        for (int i = 0; i < gN; i++) {
            gE[i].clear();
            gW[i] = FastIO::readInt();
        }

        for (int i = 1; i < gN; i++) {
            int u, v;
            u = FastIO::readInt();
            v = FastIO::readInt();
            u--;
            v--;
            gE[u].push_back(v);
            gE[v].push_back(u);
        }

        build();

        int lastX = 0;
        int lastV = 0;
        while (Q-- > 0) {
            int a, b;
            a = FastIO::readInt();
            b = FastIO::readInt();

            int v = (a ^ lastV) - 1;
            int k = b ^ lastX;

            int L = gVisit[v].first;
            int R = gVisit[v].second - 1;

            auto r = gWM.findMaxXor(L, R, k);
            lastX = r.first;
            lastV = gRMQ.query(r.second.first, r.second.second) + 1;

            lastX ^= k;

            printf("%d %d\n", lastV, lastX);
        }
    }

    return 0;
}
