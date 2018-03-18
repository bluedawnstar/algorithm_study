#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "suffixArraySparseTable.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

#include "suffixArray.h"
#include "suffixArrayAlgo.h"

void testSuffixArraySparseTable() {
    return; //TODO: if you want to test string functions, make this line a comment.

    cout << "-- LcpArraySparseTable class -------------" << endl;
    {
        string S("abdaaaaaaaaaaaaaaacccccccccccccccccccccaaaddddddddddddddddaaaaaaadddddddaaafaeef");

        vector<int> a = SuffixArray::buildSuffixArray(S);
        SuffixArraySparseTable suffixArrayST(S);

        for (int i = 0; i < 100; i++) {
            int left = RandInt32::get() % S.length();
            int right = RandInt32::get() % S.length();
            if (left > right)
                swap(left, right);

            int lcp = suffixArrayST.lcp(left, right);

            cout << "LCP(" << left << ", " << right << ") = " << lcp << endl;
            assert(lcp == SuffixArrayAlgo::commonPrefixNaive(S, left, right));
        }
    }
    {
        string S("abdaaaaaaaaaaaaaaacccccccccccccccccccccaaaddddddddddddddddaaaaaaadddddddaaafaeef");

        SuffixArray SA(S);

        for (int i = 0; i < 100; i++) {
            int left = RandInt32::get() % S.length();
            int right = RandInt32::get() % S.length();
            if (left > right)
                swap(left, right);

            int lcp = SA.lcp(left, right);
            int gt = SuffixArrayAlgo::commonPrefixNaive(S, SA[left], SA[right]);

            assert(lcp == gt);
        }
    }
    {
        string S("abdaaaaaaaaaaaaaaacccccccccccccccccccccaaaddddddddddddddddaaaaaaadddddddaaafaeef");

        SuffixArray SA(S);

        for (int i = 0; i < 100; i++) {
            int left = RandInt32::get() % S.length();
            int right = RandInt32::get() % S.length();
            if (left > right)
                swap(left, right);

            int lcp = SA.lcpWithSuffixIndex(left, right);
            int gt = SuffixArrayAlgo::commonPrefixNaive(S, left, right);

            assert(lcp == gt);
        }
    }

    cout << "OK!" << endl;
}
