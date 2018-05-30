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

class UnionFind {
public:
    UnionFind() {
    }

    UnionFind(int N) : parent(N, 0), rank(N, 0) {
        for (int i = 0; i < N; i++)
            parent[i] = i;
    }

    void init(int N) {
        parent.resize(N);
        rank.assign(N, 0);
        for (int i = 0; i < N; i++)
            parent[i] = i;
    }


    int findSet(int x) {
        if (parent[x] == x)
            return x;
        return parent[x] = findSet(parent[x]);
    }

    int unionSet(int x, int y) {
        int xset = findSet(x);
        int yset = findSet(y);
        if (xset == yset)
            return xset;

        if (rank[xset] < rank[yset]) {
            parent[xset] = yset;
            return yset;
        } else {
            parent[yset] = xset;
            if (rank[xset] == rank[yset])
                rank[xset]++;
            return xset;
        }
    }

private:
    vector<int> parent;
    vector<int> rank;
};

template <typename T, typename BinOp = function<T(T, T)>>
struct CompactSegmentTree {
    int       RealN;
    int       N;            // the size of array
    vector<T> tree;         //

    T         defaultValue;
    BinOp     mergeOp;

    explicit CompactSegmentTree(BinOp op, T dflt = T())
        : RealN(0), N(0), tree(), mergeOp(op), defaultValue(dflt) {
    }

    CompactSegmentTree(int size, BinOp op, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), defaultValue(dflt) {
        init(size, alignPowerOf2);
    }

    CompactSegmentTree(T value, int n, BinOp op, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), defaultValue(dflt) {
        build(value, n, alignPowerOf2);
    }

    CompactSegmentTree(const T arr[], int n, BinOp op, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), defaultValue(dflt) {
        build(arr, n, alignPowerOf2);
    }

    CompactSegmentTree(const vector<T>& v, BinOp op, T dflt = T(), bool alignPowerOf2 = false)
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

template <typename T, typename BinOp>
CompactSegmentTree<T, BinOp> makeCompactSegmentTree(int size, BinOp op, T dfltValue = T(), bool alignPowerOf2 = false) {
    return CompactSegmentTree<T, BinOp>(size, op, dfltValue, alignPowerOf2);
}

template <typename T, typename BinOp>
CompactSegmentTree<T, BinOp> makeCompactSegmentTree(const vector<T>& v, BinOp op, T dfltValue = T(), bool alignPowerOf2 = false) {
    return CompactSegmentTree<T, BinOp>(v, op, dfltValue, alignPowerOf2);
}

template <typename T, typename BinOp>
CompactSegmentTree<T, BinOp> makeCompactSegmentTree(const T arr[], int size, BinOp op, T dfltValue = T(), bool alignPowerOf2 = false) {
    return CompactSegmentTree<T, BinOp>(arr, size, op, dfltValue, alignPowerOf2);
}

//-----------------------------------------------------------------------------

// PRECONDITION-1: tree's range operation is monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// PRECONDITION-2: N is aligned to power of 2
// find next position where f(x) is true in [start, N)
//   f(x): xxxxxxxxxxxOOOOOOOO
//         S          ^
template <typename T, typename BinOp>
inline int findNext(const CompactSegmentTree<T, BinOp>& st, int start, const function<bool(T)>& f) {
    int cur = start + st.N;

    while (true) {
        if (f(st.tree[cur])) {
            if (cur < st.N)
                cur <<= 1;
            else
                return cur - st.N;
        } else {
            ++cur;
            if ((cur & (cur - 1)) == 0)
                break;

#ifndef __GNUC__
            cur >>= (int)_tzcnt_u32(cur);
#else
            cur >>= __builtin_ctz(cur);
#endif
        }
    }

    return -1;
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;

    UnionFind uf(N);
    vector<int> groupSize(N, 1);
    auto tree = makeCompactSegmentTree(N + 1, [](int a, int b) { return a + b; }, 0, true);
    tree.add(1, N);

    while (Q-- > 0) {
        int t;
        cin >> t;
        if (t == 1) {
            int x, y;
            cin >> x >> y;
            x--, y--;

            int gx = uf.findSet(x);
            int gy = uf.findSet(y);
            if (gx != gy) {
                int sizeX = groupSize[gx];
                int sizeY = groupSize[gy];
                tree.add(sizeX, -1);
                tree.add(sizeY, -1);
                tree.add(sizeX + sizeY, 1);
                groupSize[uf.unionSet(x, y)] = sizeX + sizeY;
            }
        } else {
            int c;
            cin >> c;

            ll res = 0;
            if (c == 0) {
                ll t = tree.query(0, N);
                res = t * (t - 1) / 2;
            } else {
                int i = findNext<int>(tree, 1, [](int x) { return x > 0; });
                while (i >= 0 && i <= N - c) {
                    res += 1ll * tree.query(i) * tree.query(i + c, N);
                    i = findNext<int>(tree, i + 1, [](int x) { return x > 0; });
                }
            }
            cout << res << endl;
        }
    }

    return 0;
}
