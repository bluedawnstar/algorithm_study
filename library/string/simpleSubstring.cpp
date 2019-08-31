#include <memory.h>
#include <cassert>
#include <cstring>
#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

#include "simpleSubstring.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <map>
#include <unordered_map>
#include <iostream>
#include <vector>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static string makeRandomString(int len, int charN) {
    string s;
    for (int i = 0; i < len; i++) {
        s.push_back('a' + RandInt32::get() % charN);
    }
    return s;
}

void testSimpleSubstring() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Simple Substring -------------------------" << endl;
    {
        int N = 100;
        string s = makeRandomString(N, 3);

        SimpleSubstringBuilder builder;
        builder.init(N);
        builder.prepare(s);


        map<string, int> treeMap1;
        unordered_map<string, int> hashMap1;
        map<SimpleSubstring, int> treeMap2;
        unordered_map<SimpleSubstring, int> hashMap2;

        for (int i = 0; i < N; i++) {
            for (int j = i; j < N; j++) {
                ++treeMap1[s.substr(i, j - i + 1)];
                ++hashMap1[s.substr(i, j - i + 1)];

                ++treeMap2[SimpleSubstring(s.c_str() + i, j - i + 1, builder.substrHash(i, j - i + 1))];
                ++hashMap2[SimpleSubstring(s.c_str() + i, j - i + 1, builder.substrHash(i, j - i + 1))];
            }
        }
        assert(treeMap1.size() == treeMap2.size());
        assert(treeMap1.size() == hashMap2.size());
        assert(hashMap1.size() == hashMap2.size());

        {
            auto it1 = treeMap1.begin();
            auto it2 = treeMap2.begin();
            for (; it1 != treeMap1.end() && it2 != treeMap2.end(); ++it1, ++it2) {
                assert(strncmp(it1->first.c_str(), it2->first.s, it2->first.len) == 0);
                assert(it1->second == it2->second);
            }
        }
        {
            auto it1 = hashMap1.begin();
            auto it2 = hashMap2.begin();
            vector<pair<string, int>> v1, v2;
            for (; it1 != hashMap1.end() && it2 != hashMap2.end(); ++it1, ++it2) {
                v1.emplace_back(it1->first, it1->second);
                v2.emplace_back(string(it2->first.s, it2->first.len), it2->second);
            }
            sort(v1.begin(), v1.end());
            sort(v2.begin(), v2.end());
            assert(v1 == v2);
        }
    }
    {
        int N = 1000;
#ifdef _DEBUG
        N = 100;
#endif

        string s = makeRandomString(N, 3);

        SimpleSubstringBuilder builder;
        builder.init(N);
        builder.prepare(s);


        map<string, int> treeMap1;
        unordered_map<string, int> hashMap1;
        map<SimpleSubstring, int> treeMap2;
        unordered_map<SimpleSubstring, int> hashMap2;

        PROFILE_START(0);
        for (int i = 0; i < N; i++) {
            for (int j = i; j < N; j++)
                ++treeMap1[s.substr(i, j - i + 1)];
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        for (int i = 0; i < N; i++) {
            for (int j = i; j < N; j++)
                ++treeMap2[SimpleSubstring(s.c_str() + i, j - i + 1, builder.substrHash(i, j - i + 1))];
        }
        PROFILE_STOP(1);

        PROFILE_START(2);
        for (int i = 0; i < N; i++) {
            for (int j = i; j < N; j++)
                ++hashMap1[s.substr(i, j - i + 1)];
        }
        PROFILE_STOP(2);

        PROFILE_START(3);
        for (int i = 0; i < N; i++) {
            for (int j = i; j < N; j++)
                ++hashMap2[SimpleSubstring(s.c_str() + i, j - i + 1, builder.substrHash(i, j - i + 1))];
        }
        PROFILE_STOP(3);

        if (treeMap1.size() != treeMap2.size())
            cout << "ERROR : mismatched!" << endl;

        if (treeMap1.size() != hashMap2.size())
            cout << "ERROR : mismatched!" << endl;

        if (hashMap1.size() != hashMap2.size())
            cout << "ERROR : mismatched!" << endl;
    }

    cout << "OK!" << endl;
}
