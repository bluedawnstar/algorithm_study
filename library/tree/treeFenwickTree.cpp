#include <memory.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

// for sum from 0 to pos
template <typename T>
struct FenwickTree {
    vector<T> mTree;

    FenwickTree(int n) : mTree(n + 1) {
        // no action
    }

    void clear() {
        fill(mTree.begin(), mTree.end(), 0);
    }

    void init(T arr[], int n) {
        for (int i = 0; i < n; i++)
            add(i, arr[i]);
    }

    void init(vector<T>& v, int n) {
        for (int i = 0; i < n; i++)
            add(i, v[i]);
    }

    // sum from 0 to pos
    T sum(int pos) {
        pos++;

        T res = 0;
        while (pos > 0) {
            res += mTree[pos];
            pos &= pos - 1;         // clear lowest bit
        }

        return res;
    }

    // inclusive
    T rangeSum(int left, int right) {
        T res = sum(right);
        if (left > 0)
            res -= sum(left - 1);
        return res;
    }

    void add(int pos, T val) {
        pos++;

        while (pos < (int)mTree.size()) {
            mTree[pos] += val;
            pos += pos & -pos;      // add lowest bit
        }
    }

    // inclusive
    void addRange(int left, int right, T val) {
        add(left, val);
        if (right + 1 < (int)mTree.size() - 1)
            add(right + 1, -val);
    }
};

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testFenwickTree() {
    return; //TODO: if you want to test a split function, make this line to a comment.

    cout << "-- FenwickTree -----------------------------------------" << endl;

    int freq[] = { 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9 };
    int n = sizeof(freq) / sizeof(freq[0]);

    FenwickTree<int> fenwick(n);

    fenwick.init(freq, n);

    int ans = fenwick.sum(5);
    cout << "fenwick.sum(5) = " << ans << endl;
    assert(ans == 12);

    fenwick.add(3, 6);
    cout << "after fenwick.add(3, 6)" << endl;

    ans = fenwick.sum(5);
    cout << "fenwick.sum(5) = " << ans << endl;
    assert(ans == 18);

    ans = fenwick.rangeSum(1, 5);
    cout << "fenwick.rangeSum(1, 5) = " << ans << endl;
    assert(ans == 16);

    cout << "OK!" << endl;
}
