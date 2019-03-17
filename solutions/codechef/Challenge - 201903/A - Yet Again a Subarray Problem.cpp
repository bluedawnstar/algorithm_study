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

template <typename T>
struct FenwickTree {
    vector<T> tree;

    FenwickTree() {
    }

    explicit FenwickTree(int n) : tree(n + 1) {
    }

    FenwickTree(const T value, int n) {
        build(value, n);
    }

    FenwickTree(const T arr[], int n) {
        build(arr, n);
    }

    FenwickTree(const vector<T>& v) {
        build(v);
    }


    void init(int n) {
        tree = vector<T>(n + 1);
    }

    void build(T value, int n) {
        init(n);
        for (int i = 0; i < n; i++)
            add(i, value);
    }

    void build(const T arr[], int n) {
        init(n);
        for (int i = 0; i < n; i++)
            add(i, arr[i]);
    }

    void build(const vector<T>& v) {
        build(&v[0], int(v.size()));
    }


    T sum(int pos) const {
        pos++;

        T res = 0;
        while (pos > 0) {
            res += tree[pos];
            pos &= pos - 1;
        }

        return res;
    }

    T sumRange(int left, int right) const {
        T res = sum(right);
        if (left > 0)
            res -= sum(left - 1);
        return res;
    }

    void add(int pos, T val) {
        pos++;

        while (pos < int(tree.size())) {
            tree[pos] += val;
            pos += pos & -pos;
        }
    }

    T get(int pos) const {
        T res = tree[pos + 1];
        if (pos > 0) {
            int lca = pos & (pos + 1);
            for (pos; pos != lca; pos &= pos - 1)
                res -= tree[pos];
        }

        return res;
    }

    int lowerBound(ll sum, ll scale) {
        --sum;

        int N = int(tree.size()) - 1;

        int blockSize = N;
        while (blockSize & (blockSize - 1))
            blockSize &= blockSize - 1;

        int lo = 0;
        for (; blockSize > 0; blockSize >>= 1) {
            int next = lo + blockSize;
            if (next <= N && sum >= tree[next] * scale) {
                sum -= tree[next] * scale;
                lo = next;
            }
        }

        return lo;
    }
};

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T-- > 0) {
        int N, K;
        cin >> N >> K;

        vector<int> A(N);
        for (int i = 0; i < N; i++)
            cin >> A[i];

        int ans = 0;
        for (int L = 0; L < N; L++) {
            FenwickTree<int> tr(2001);
            for (int R = L; R < N; R++) {
                tr.add(A[R], 1);

                int m = (K + R - L) / (R - L + 1);

                int lo = tr.lowerBound(K, m);
                int x = tr.get(lo);
                if (x <= 2000 && tr.get(x) > 0)
                    ans++;
            }
        }

        cout << ans << endl;
    }

    return 0;
}
