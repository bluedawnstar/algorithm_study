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
        TrieAM<> trie;

        const char* keys[] = { "the", "a", "there", "answer", "any", "by", "bye", "their" };
        for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
            trie.insert(keys[i], int(strlen(keys[i])));

        assert(trie.find("the", int(strlen("the")))->leafCount == 1);
        assert(trie.find("these", int(strlen("these"))) == nullptr);
        assert(trie.find("their", int(strlen("their")))->leafCount == 1);
        assert(trie.find("thaw", int(strlen("thaw"))) == nullptr);

        trie.insert("the", int(strlen("the")));
        trie.insert("these", int(strlen("these")));
        trie.insert("their", int(strlen("their")));
        trie.insert("thaw", int(strlen("thaw")));

        assert(trie.find("the", int(strlen("the")))->leafCount == 2);
        assert(trie.find("these", int(strlen("these")))->leafCount == 1);
        assert(trie.find("their", int(strlen("their")))->leafCount == 2);
        assert(trie.find("thaw", int(strlen("thaw")))->leafCount == 1);

        trie.remove("the", int(strlen("the")));
        trie.remove("these", int(strlen("these")));
        trie.remove("their", int(strlen("their")));
        trie.remove("thaw", int(strlen("thaw")));

        assert(trie.find("the", int(strlen("the")))->leafCount == 1);
        assert(trie.find("these", int(strlen("these"))) == nullptr);
        assert(trie.find("their", int(strlen("their")))->leafCount == 1);
        assert(trie.find("thaw", int(strlen("thaw"))) == nullptr);

        trie.insert("the", int(strlen("the")));
        trie.insert("these", int(strlen("these")));
        trie.insert("their", int(strlen("their")));
        trie.insert("thaw", int(strlen("thaw")));

        assert(trie.find("the", int(strlen("the")))->leafCount == 2);
        assert(trie.find("these", int(strlen("these")))->leafCount == 1);
        assert(trie.find("their", int(strlen("their")))->leafCount == 2);
        assert(trie.find("thaw", int(strlen("thaw")))->leafCount == 1);

        trie.erase("the", int(strlen("the")));
        trie.erase("these", int(strlen("these")));
        trie.erase("their", int(strlen("their")));
        trie.erase("thaw", int(strlen("thaw")));

        assert(trie.find("the", int(strlen("the")))->leafCount == 1);
        assert(trie.find("these", int(strlen("these"))) == nullptr);
        assert(trie.find("their", int(strlen("their")))->leafCount == 1);
        assert(trie.find("thaw", int(strlen("thaw"))) == nullptr);

        assert(trie.search("t", int(strlen("t"))) == make_pair(1, false));
        assert(trie.search("th", int(strlen("th"))) == make_pair(2, false));
        assert(trie.search("the", int(strlen("the"))) == make_pair(3, true));
        assert(trie.search("thei", int(strlen("thei"))) == make_pair(4, false));
        assert(trie.search("their", int(strlen("their"))) == make_pair(5, true));
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
            Trie<> tree;
            for (int i = 0; i < N; i++) {
                tree.insert(in[i]);
            }
            if (count(tree.mRoot.children, tree.mRoot.children + 26, nullptr) == 26)
                cerr << "What?" << endl;
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        {
            TrieAM<> tree;
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
