#include <string>
#include <vector>

using namespace std;

#include "hashing.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <algorithm>
#include <iostream>
#include "../common/iostreamhelper.h"

static string makeRandomString(int n) {
    string s;
    s.reserve(n);
    for (int i = 0; i < n; i++)
        s.push_back(rand() % 26 + 'a');
    return s;
}

void testHashing() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Hashing ---------------------------------------------" << endl;

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
        hashValue[i] = genHash(s[i]);

    auto cnt = hashValue.size();
    sort(hashValue.begin(), hashValue.end());
    hashValue.erase(unique(hashValue.begin(), hashValue.end()), hashValue.end());

    if (cnt != hashValue.size())
        cerr << "Duplicate hash value found!" << endl;
    assert(cnt == hashValue.size());

    cout << "OK!" << endl;
}
