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

template <typename T, typename MergeOp = function<T(T, T)>>
struct CompactSegmentTree {
    int       RealN;
    int       N;            // the size of array
    vector<T> tree;         //

    T         defaultValue;
    MergeOp   mergeOp;

    explicit CompactSegmentTree(MergeOp op, T dflt = T())
        : RealN(0), N(0), tree(), mergeOp(op), defaultValue(dflt) {
    }

    CompactSegmentTree(int size, MergeOp op, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), defaultValue(dflt) {
        init(size, alignPowerOf2);
    }

    CompactSegmentTree(T value, int n, MergeOp op, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), defaultValue(dflt) {
        build(value, n, alignPowerOf2);
    }

    CompactSegmentTree(const T arr[], int n, MergeOp op, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), defaultValue(dflt) {
        build(arr, n, alignPowerOf2);
    }

    CompactSegmentTree(const vector<T>& v, MergeOp op, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), defaultValue(dflt) {
        build(v, alignPowerOf2);
    }


    void init(int size, bool alignPowerOf2 = false) {
        RealN = size;
        if (alignPowerOf2) {
#ifndef __GNUC__
            N = 1 << (32 - (int)_lzcnt_u32(size - 1));
#else
            N = 1 << (32 - __builtin_clz(size - 1));
#endif
        } else
            N = size + (size & 1);
        tree.assign(N * 2, defaultValue);
    }

    void build(T value, int size, bool alignPowerOf2 = false) {
        init(size, alignPowerOf2);

        for (int i = 0; i < size; i++)
            tree[N + i] = value;

        for (int i = N - 1; i > 0; i--)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    void build(const T arr[], int size, bool alignPowerOf2 = false) {
        init(size, alignPowerOf2);

        for (int i = 0; i < size; i++)
            tree[N + i] = arr[i];

        for (int i = N - 1; i > 0; i--)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    void build(const vector<T>& v, bool alignPowerOf2 = false) {
        build(&v[0], (int)v.size(), alignPowerOf2);
    }

    //--- query

    T query(int index) const {
        return tree[index + N];
    }

    // inclusive
    T query(int left, int right) const {
        T resL = defaultValue;
        T resR = defaultValue;

        for (int L = left + N, R = right + N + 1; L < R; L >>= 1, R >>= 1) {
            if (L & 1)
                resL = mergeOp(resL, tree[L++]);
            if (R & 1)
                resR = mergeOp(tree[--R], resR);
        }

        return mergeOp(resL, resR);
    }

    //--- update

    void update(int index, T newValue) {
        tree[index + N] = newValue;

        for (int i = (index + N) >> 1; i > 0; i >>= 1)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    // inclusive
    void updateRange(int left, int right, T newValue) {
        for (int L = left + N, R = right + N + 1; L < R; L++)
            tree[L] = newValue;

        for (int L = (left + N) >> 1, R = (right + N) >> 1; L > 0; L >>= 1, R >>= 1) {
            for (int i = L; i <= R; i++)
                tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
        }
    }

    //--- add

    void add(int index, T value) {
        tree[index + N] += value;

        for (int i = (index + N) >> 1; i > 0; i >>= 1)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    // inclusive
    void addRange(int left, int right, T value) {
        for (int L = left + N, R = right + N + 1; L < R; L++)
            tree[L] += value;

        for (int L = (left + N) >> 1, R = (right + N) >> 1; L > 0; L >>= 1, R >>= 1) {
            for (int i = L; i <= R; i++)
                tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
        }
    }
};

template <typename T, typename MergeOp>
inline CompactSegmentTree<T, MergeOp> makeCompactSegmentTree(int size, MergeOp op, T dfltValue = T(), bool alignPowerOf2 = false) {
    return CompactSegmentTree<T, MergeOp>(size, op, dfltValue, alignPowerOf2);
}

template <typename T, typename MergeOp>
inline CompactSegmentTree<T, MergeOp> makeCompactSegmentTree(const vector<T>& v, MergeOp op, T dfltValue = T(), bool alignPowerOf2 = false) {
    return CompactSegmentTree<T, MergeOp>(v, op, dfltValue, alignPowerOf2);
}

template <typename T, typename MergeOp>
inline CompactSegmentTree<T, MergeOp> makeCompactSegmentTree(const T arr[], int size, MergeOp op, T dfltValue = T(), bool alignPowerOf2 = false) {
    return CompactSegmentTree<T, MergeOp>(arr, size, op, dfltValue, alignPowerOf2);
}

ll maximumProfit(vector<int> p) {
    int N = (int)p.size();

    int minP = *min_element(p.begin(), p.end());
    int maxP = *max_element(p.begin(), p.end());

    set<int> tree1;
    auto maxTree2 = makeCompactSegmentTree(maxP - minP + 1, [](ll a, ll b) { return max(a, b); }, numeric_limits<ll>::min());
    auto minTree2 = makeCompactSegmentTree(maxP - minP + 1, [](ll a, ll b) { return min(a, b); }, numeric_limits<ll>::max());

    ll res = numeric_limits<ll>::min();
    for (int i = 0; i < N; i++) {
        int key = p[i] - minP;
        if (key > 0) {
            ll minVal = minTree2.query(0, key - 1);
            ll maxVal = maxTree2.query(0, key - 1);
            if (minVal != numeric_limits<ll>::max())
                res = max(res, max(minVal * p[i], maxVal * p[i]));
        }
        if (key > 0 && !tree1.empty()) {
            auto itE = tree1.lower_bound(p[i]);
            if (itE != tree1.begin()) {
                ll minVal = 1ll * *tree1.begin() * p[i];
                ll maxVal = 1ll * *--itE * p[i];
                if (minVal > maxVal)
                    swap(minVal, maxVal);
                minTree2.update(key, min(minTree2.query(0, key), minVal));
                maxTree2.update(key, max(maxTree2.query(0, key), maxVal));
            }
        }
        tree1.insert(p[i]);
    }

    return res == numeric_limits<ll>::min() ? -1 : res;
}

#if 0
ll maximumProfit(vector<int> p) {
    int N = (int)p.size();

    set<int> P1;
    map<int, pair<ll, ll>> P2;

    ll res = numeric_limits<ll>::min();
    for (int i = 0; i < N; i++) {
        if (!P2.empty()) {
            auto it = P2.lower_bound(p[i]);
            if (it != P2.begin()) {
                --it;
                res = max(res, 1ll * it->second.first * p[i]);
                res = max(res, 1ll * it->second.second * p[i]);
            }
        }
        if (!P1.empty()) {
            auto it1 = P1.lower_bound(p[i]);
            if (it1 != P1.begin()) {
                --it1;
                auto t1 = 1ll * *it1 * p[i];
                auto t2 = 1ll * *P1.begin() * p[i];
                if (t1 > t2)
                    swap(t1, t2);

                auto it2 = P2.lower_bound(p[i]);
                if (it2 == P2.end() || p[i] < P2.begin()->first)
                    P2[p[i]] = make_pair(t1, t2);
                else if (it2->first == p[i]) {
                    it2->second.first = min(it2->second.first, t1);
                    it2->second.second = max(it2->second.second, t2);
                } else {
                    --it2;
                    P2[p[i]] = make_pair(min(it2->second.first, t1), max(it2->second.second, t2));
                }
            }
        }
        P1.insert(p[i]);
    }

    return res == numeric_limits<ll>::min() ? -1 : res;
}
#endif

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<int> p(N);
    for (int i = 0; i < N; i++)
        cin >> p[i];
    
    cout << maximumProfit(p) << endl;

    return 0;
}
