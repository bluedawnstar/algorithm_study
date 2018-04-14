#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

#include "hashing.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <algorithm>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

static string makeRandomString(int n) {
    string s;
    s.reserve(n);
    for (int i = 0; i < n; i++)
        s.push_back(RandInt32::get() % 26 + 'a');
    return s;
}

static int countUniqueSubstrings(const string& s) {
    unordered_set<string> hs;
    for (int i = 0; i < (int)s.length(); i++) {
        for (int j = i; j < (int)s.length(); j++)
            hs.insert(s.substr(i, j - i + 1));
    }
    return (int)hs.size();
}

void testHashing() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Hashing ---------------------------------------------" << endl;
    {
        //int N = 10000000;
        //int L = 30;
        int N = 1000;
        int L = 10;
        vector<string> s;
        for (int i = 0; i < N; i++) {
            s.push_back(makeRandomString(L));
        }
        sort(s.begin(), s.end());
        s.erase(unique(s.begin(), s.end()), s.end());

        vector<unsigned long long> hashValue(s.size());
        for (int i = 0; i < (int)s.size(); i++)
            hashValue[i] = StringHash::hash(s[i]);

        auto cnt = hashValue.size();
        sort(hashValue.begin(), hashValue.end());
        hashValue.erase(unique(hashValue.begin(), hashValue.end()), hashValue.end());

        if (cnt != hashValue.size())
            cerr << "Duplicate hash value found!" << endl;
        assert(cnt == hashValue.size());
    }
    {
        int N = 50;
        int T = 10;
        for (int i = 0; i < T; i++) {
            auto s = makeRandomString(N);
            assert(StringHashAlgo::countUniqueSubstrings(s) == countUniqueSubstrings(s));
        }
    }
    cout << "OK!" << endl;
}
