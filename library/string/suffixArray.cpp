#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "suffixArray.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

void testSuffixArray() {
    return; //TODO: if you want to test string functions, make this line a comment.

    cout << "-- makeSuffixArray() -------------" << endl;

    string S("abdadafaaabdfaeef");

    vector<int> a1 = makeSuffixArray(S, (int)S.length());
    vector<int> a2 = makeSuffixArrayFast(S, (int)S.length());

    assert(a1 == a2);

    cout << "OK!" << endl;

    cout << "-- countSubstrings() -------------" << endl;

    long long cnt11 = countSubstrings(a1, S, (int)S.length());
    long long cnt12 = countSubstrings(a2, S, (int)S.length());
    long long cnt21 = countSubstringsFast(a1, S, (int)S.length());
    long long cnt22 = countSubstringsFast(a2, S, (int)S.length());

    assert(cnt11 == cnt12);
    assert(cnt21 == cnt22);
    assert(cnt11 == cnt21);

    cout << "OK!" << endl;
}
