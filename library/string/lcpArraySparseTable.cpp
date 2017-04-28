#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "lcpArraySparseTable.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

template <typename T>
vector<int> makeSuffixArrayFast(T s, int n);

template <typename T>
int commonPrefix(T s, int n, int i, int j);

template <typename T>
vector<int> makeLcpArray(const vector<int>& suffixArray, T s, int n);

void testLcpArraySparseTable() {
    return; //TODO: if you want to test string functions, make this line a comment.

    cout << "-- LcpArray class -------------" << endl;

    string S("abdadafaaabdfaeef");

    vector<int> a = makeSuffixArrayFast(S, (int)S.length());
    vector<int> lcpArray = makeLcpArray(a, S, (int)S.length());
    LcpArraySparseTable lcpArrayST(lcpArray, (int)lcpArray.size());

    for (int i = 0; i < 100; i++) {
        int left = rand() % S.length(), right = rand() % S.length();
        if (left > right)
            swap(left, right);

        // if left == right, lcp() will return INT_MAX
        int lcp = lcpArrayST.lcp(left, right);
        if (lcp == INT_MAX)
            lcp = (int)S.length() - a[left];

        cout << "LCP(" << left << ", " << right << ") = " << lcp << endl;
        assert(lcp == commonPrefix(S, (int)S.length(), a[left], a[right]));
    }

    cout << "OK!" << endl;
}
