#include <memory.h>
#include <cassert>
#include <cstring>
#include <memory>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

#include "trieAM.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include <vector>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static string makeRandomString(int n) {
    string s;
    s.reserve(n);
    for (int i = 0; i < n; i++)
        s.push_back(RandInt32::get() % 26 + 'a');
    return s;
}

void testTrieAM() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Array Mapped Trie ---------------------------------" << endl;
    {
        TrieAM trie;

        const char* keys[] = { "the", "a", "there", "answer", "any", "by", "bye", "their" };
        for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
            trie.insert(keys[i], strlen(keys[i]));

        assert(trie.find("the", strlen("the"))->leafCount == 1);
        assert(trie.find("these", strlen("these")) == nullptr);
        assert(trie.find("their", strlen("their"))->leafCount == 1);
        assert(trie.find("thaw", strlen("thaw")) == nullptr);

        trie.insert("the", strlen("the"));
        trie.insert("these", strlen("these"));
        trie.insert("their", strlen("their"));
        trie.insert("thaw", strlen("thaw"));

        assert(trie.find("the", strlen("the"))->leafCount == 2);
        assert(trie.find("these", strlen("these"))->leafCount == 1);
        assert(trie.find("their", strlen("their"))->leafCount == 2);
        assert(trie.find("thaw", strlen("thaw"))->leafCount == 1);

        trie.remove("the", strlen("the"));
        trie.remove("these", strlen("these"));
        trie.remove("their", strlen("their"));
        trie.remove("thaw", strlen("thaw"));

        assert(trie.find("the", strlen("the"))->leafCount == 1);
        assert(trie.find("these", strlen("these")) == nullptr);
        assert(trie.find("their", strlen("their"))->leafCount == 1);
        assert(trie.find("thaw", strlen("thaw")) == nullptr);

        trie.insert("the", strlen("the"));
        trie.insert("these", strlen("these"));
        trie.insert("their", strlen("their"));
        trie.insert("thaw", strlen("thaw"));

        assert(trie.find("the", strlen("the"))->leafCount == 2);
        assert(trie.find("these", strlen("these"))->leafCount == 1);
        assert(trie.find("their", strlen("their"))->leafCount == 2);
        assert(trie.find("thaw", strlen("thaw"))->leafCount == 1);

        trie.erase("the", strlen("the"));
        trie.erase("these", strlen("these"));
        trie.erase("their", strlen("their"));
        trie.erase("thaw", strlen("thaw"));

        assert(trie.find("the", strlen("the"))->leafCount == 1);
        assert(trie.find("these", strlen("these")) == nullptr);
        assert(trie.find("their", strlen("their"))->leafCount == 1);
        assert(trie.find("thaw", strlen("thaw")) == nullptr);

        assert(trie.search("t", strlen("t")) == make_pair(1, false));
        assert(trie.search("th", strlen("th")) == make_pair(2, false));
        assert(trie.search("the", strlen("the")) == make_pair(3, true));
        assert(trie.search("thei", strlen("thei")) == make_pair(4, false));
        assert(trie.search("their", strlen("their")) == make_pair(5, true));
    }
    // memory & speed test
    {
        int L = 500;
        int N = 10000;
        vector<string> in;

        for (int i = 0; i < N; i++)
            in.push_back(makeRandomString(RandInt32::get() % L + 1));

        PROFILE_START(0);
        {
            Trie tree;
            for (int i = 0; i < N; i++) {
                tree.insert(in[i]);
            }
            if (count(tree.mRoot.children, tree.mRoot.children + Trie::MaxCharN, nullptr) == Trie::MaxCharN)
                cerr << "What?" << endl;
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        {
            TrieAM tree;
            for (int i = 0; i < N; i++) {
                tree.insert(in[i]);
            }
            if (tree.mRoot.children.empty())
                cerr << "What?" << endl;
        }
        PROFILE_STOP(1);
    }

    cout << "OK!" << endl;
}
