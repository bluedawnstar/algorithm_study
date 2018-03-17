#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "distinctSubstringCounter.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include <map>
#include "suffixAutomaton.h"
#include "../common/iostreamhelper.h"

#include "suffixArray.h"

void testDistinctSubstringCounter() {
    return; //TODO: if you want to test string functions, make this line a comment.

    cout << "-- DistinctSubstringCounter class -------------" << endl;

    string S("qqqqqqqqqqzrzrrzrzrrzrrzrzrrzrzrrzttttttttttttttttttttttttttttttttttttttttttttttttttttttqncpqzcxpbwa");
    cout << "String is \"" << S << "\"" << endl;
    
    map<pair<int, int>, long long> M;

    cout << "*** test with Suffix Array" << endl;
    {
        vector<int> SA = SuffixArray::build(S);
        vector<int> lcpArray = SuffixArray::buildLcpArray(SA, S);

        DistinctSubstringCounterWithSuffixArray<long long> dsc(SA, lcpArray, (int)lcpArray.size());

        int suffixPos = dsc.currSuffixPos;
        do {
            suffixPos = dsc.extend();
            for (int i = suffixPos; i < (int)S.length(); i++) {
                long long result = dsc.query(i);
                //cout << "substring count (" << suffixPos << ", " << i << ") = " << result << endl;
                M[make_pair(suffixPos, i)] = result;
            }
        } while (suffixPos > 0);
    }

    cout << "*** test with Suffix Automation" << endl;
    {
        DistinctSubstringCounterWithSuffixAutomaton<long long> sa((int)S.length());

        sa.init();
        for (int i = 0; i < (int)S.length(); i++) {
            sa.extend(S[i]);

            for (int j = 0; j <= i; j++) {
                long long result = sa.query(j);
                //cout << "substring count (" << j << ", " << i << ") = " << result << endl;
                assert(M[make_pair(j, i)] == result);
            }
        }
    }

    cout << "OK!" << endl;
}
