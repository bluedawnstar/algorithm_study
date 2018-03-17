#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "lcpArraySparseTable.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

#include "suffixArray.h"
#include "suffixArrayAlgo.h"

void testLcpArraySparseTable() {
    return; //TODO: if you want to test string functions, make this line a comment.

    cout << "-- LcpArray class -------------" << endl;

    string S("abdadafaaabdfaeef");

    vector<int> a = SuffixArray::build(S);
    vector<int> lcpArray = SuffixArray::buildLcpArray(a, S);
    LcpArraySparseTable lcpArrayST(lcpArray);

    for (int i = 0; i < 100; i++) {
        int left = RandInt32::get() % S.length(), right = RandInt32::get() % S.length();
        if (left > right)
            swap(left, right);

        // if left == right, lcp() will return INT_MAX
        int lcp = lcpArrayST.lcp(left, right);
        if (lcp == INT_MAX)
            lcp = (int)S.length() - a[left];

        cout << "LCP(" << left << ", " << right << ") = " << lcp << endl;
        assert(lcp == SuffixArrayAlgo::commonPrefixNaive(S, a[left], a[right]));
    }

    cout << "OK!" << endl;
}
