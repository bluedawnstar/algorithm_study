#include <string>
#include <set>
#include <vector>
#include <algorithm>
#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

using namespace std;

#include "prefixFunctionTree.h"
#include "prefixFunctionTreeAlgo.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static long long countPrefixSuffixSubstring(const string& pattern, const string& S) {
    long long res = 0;

    int N = int(pattern.length());
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            string P = pattern.substr(0, i) + pattern.substr(N - j, j);
            if (S.find(P) != string::npos)
                res++;
        }
    }

    return res;
}


void testPrefixFunctionTree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Prefix Function Tree ---------" << endl;
    {
        string P = "abac";
        string S = "aacabc";
        
        auto ans1 = countPrefixSuffixSubstring(P, S);

        PrefixSuffixSubstringCounter solver;
        solver.build(P, S);
        auto ans2 = solver.count();

        if (ans1 != ans2) {
            cout << "Mismatched at " << __LINE__ << " : " << ans1 << ", " << ans2 << endl;
        }
        assert(ans1 == ans2);
    }

    cout << "OK!" << endl;
}
