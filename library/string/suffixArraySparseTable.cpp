#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "suffixArraySparseTable.h"

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

void testSuffixArraySparseTable() {
    //return; //TODO: if you want to test string functions, make this line to a comment.

    cout << "-- LcpArray class -------------" << endl;

    string S("abdaaaaaaaaaaaaaaacccccccccccccccccccccaaaddddddddddddddddaaaaaaadddddddaaafaeef");

    SuffixArraySparseTable suffixArrayST(S, (int)S.length());

    for (int i = 0; i < 100; i++) {
        int left = rand() % S.length(), right = rand() % S.length();
        if (left > right)
            swap(left, right);

        int lcp = suffixArrayST.lcp(left, right);

        cout << "LCP(" << left << ", " << right << ") = " << lcp << endl;
        assert(lcp == commonPrefix(S, (int)S.length(), left, right));
    }

    cout << "OK!" << endl;
}
