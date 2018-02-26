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

    FenwickTree(int n) : tree(n + 1) {
        // no action
    }

    void clear() {
        fill(tree.begin(), tree.end(), 0);
    }

    void init(T arr[], int n) {
        for (int i = 0; i < n; i++)
            add(i, arr[i]);
    }

    void init(vector<T>& v) {
        for (int i = 0; i < (int)v.size(); i++)
            add(i, v[i]);
    }

    // sum from 0 to pos
    T sum(int pos) const {
        pos++;

        T res = 0;
        while (pos > 0) {
            res += tree[pos];
            pos &= pos - 1;         // clear lowest bit
        }

        return res;
    }

    // inclusive
    T sumRange(int left, int right) const {
        T res = sum(right);
        if (left > 0)
            res -= sum(left - 1);
        return res;
    }

    void add(int pos, T val) {
        pos++;

        while (pos < (int)tree.size()) {
            tree[pos] += val;
            pos += pos & -pos;      // add lowest bit
        }
    }

    // inclusive
    void addRange(int left, int right, T val) {
        add(left, val);
        if (right + 1 < (int)tree.size() - 1)
            add(right + 1, -val);
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

    void set(int pos, T val) {
        add(pos, val - get(pos));
    }

    // returns min(i | sum[0,i] >= sum)
    int lowerBound(T sum) const {
        --sum;

        int N = (int)tree.size() - 1;
        int pos = 0;

        int blockSize = N;
        while (blockSize & (blockSize - 1))
            blockSize &= blockSize - 1;

        for (; blockSize > 0; blockSize >>= 1) {
            int nextPos = pos + blockSize;
            if (nextPos < N && sum >= tree[nextPos]) {
                sum -= tree[nextPos];
                pos = nextPos;
            }
        }

        return pos;
    }
};

vector<int> countUniqueNumbers(const vector<int>& in, const vector<pair<int, int>>& rangeQuery) {
    int N = (int)in.size();

    //--- compaction of input values

    vector<int> t(in.begin(), in.end());
    sort(t.begin(), t.end());
    t.erase(unique(t.begin(), t.end()), t.end());

    unordered_map<int, int> M;
    for (int i = 0; i < (int)t.size(); i++) {
        M[t[i]] = i;
    }

    vector<int> A(N);
    for (int i = 0; i < N; i++)
        A[i] = M[in[i]];

    //--- query

    int Q = (int)rangeQuery.size();

    vector<int> ans(Q);
    vector<vector<pair<int, int>>> qry(N);   // [left] -> (right, ans index)
    for (int i = 0; i < Q; i++)
        qry[rangeQuery[i].first].emplace_back(rangeQuery[i].second, i);

    FenwickTree<int> tree(N + 1);

    vector<int> first(N, -1);
    vector<int> last(N, -1);
    vector<int> next(N + 1, N);

    for (int i = 0; i < N; i++) {
        auto l = last[A[i]];
        if (l >= 0)
            next[l] = i;
        else
            first[A[i]] = i;
        last[A[i]] = i;
    }
    for (int i = 0; i < N; i++) {
        if (first[i] >= 0) {
            tree.add(first[i], 1);
            tree.add(next[first[i]], -1);
        }
    }

    for (int i = 0; i < N; i++) {
        for (auto it : qry[i])
            ans[it.second] = tree.sumRange(i, it.first);

        tree.add(next[i], 2);
        tree.add(next[next[i]], -1);
    }

    return ans;
}

#define MAXN    1000000
#define MAXQ    1000000

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<int> A(N);
    for (int i = 0; i < N; i++)
        cin >> A[i];

    int Q;
    cin >> Q;

    vector<pair<int, int>> rangeQuery(Q);
    for (int i = 0; i < Q; i++) {
        cin >> rangeQuery[i].first >> rangeQuery[i].second;
        --rangeQuery[i].first;
        --rangeQuery[i].second;
    }

    auto ans = countUniqueNumbers(A, rangeQuery);

    for (int i = 0; i < Q; i++)
        cout << ans[i] << endl;

    return 0;
}
