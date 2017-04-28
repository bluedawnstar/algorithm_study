#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "distinctSubstringCounter.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

template <typename T>
vector<int> makeSuffixArrayFast(T s, int n);

template <typename T>
vector<int> makeLcpArray(const vector<int>& suffixArray, T s, int n);

void testDistinctSubstringCounter() {
    return; //TODO: if you want to test string functions, make this line a comment.

    cout << "-- DistinctSubstringCounter class -------------" << endl;

    string S("aabaa");

    cout << "String is \"" << S << "\"" << endl;

    vector<int> SA = makeSuffixArrayFast(S, (int)S.length());
    vector<int> lcpArray = makeLcpArray(SA, S, (int)S.length());

    DistinctSubstringCounter<long long> dsc(SA, lcpArray, (int)lcpArray.size());

    int suffixPos = dsc.currSuffixPos;
    do {
        suffixPos = dsc.extend();
        for (int i = suffixPos; i < (int)S.length(); i++)
            cout << "substring count (" << suffixPos << ", " << i << ") = " << dsc.query(i) << endl;
    } while (suffixPos > 0);

    cout << "OK!" << endl;
}
