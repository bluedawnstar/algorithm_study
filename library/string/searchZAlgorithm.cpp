#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "searchZAlgorithm.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <iostream>
#include <cassert>
#include "../common/iostreamhelper.h"

static vector<int> searchNaive(const string& text, const string& pattern) {
    vector<int> res;

    int textLength = (int)text.length();
    int patternLength = (int)pattern.length();

    for (int i = 0; i <= textLength - patternLength; i++) {
        bool matched = true;
        for (int j = 0; j < patternLength; j++) {
            if (text[i + j] != pattern[j]) {
                matched = false;
                break;
            }
        }

        if (matched)
            res.push_back(i);
    }

    return res;
}

void testZAlgorithm() {
    return; //TODO: if you want to test string functions, make this line a comment.

    cout << "-- Z Algorithm ---------" << endl;

    string S = "AABAACAADAABAABA", key = "AABA";

    auto gt = searchNaive(S, key);
    auto ans = searchZ(S, key);
    if (gt != ans) {
        cerr << "Error: " << gt << " <-> " << ans << endl;
    }
    assert(gt == ans);

    cout << "OK!" << endl;
}
