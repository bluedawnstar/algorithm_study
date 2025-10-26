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

#include "trie.h"
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
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Array Mapped Trie ---------------------------------" << endl;
    {
        TrieAM<> trie;

        string keys[] = { "the", "a", "there", "answer", "any", "by", "bye", "their" };
        for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
            trie.insert(keys[i]);

        assert(trie.find("the")->terminal);
        assert(trie.find("these") == nullptr);
        assert(trie.find("their")->terminal);
        assert(trie.find("thaw") == nullptr);

        assert(trie.insert("the") == false);
        assert(trie.insert("these") == true);
        assert(trie.insert("their") == false);
        assert(trie.insert("thaw") == true);

        assert(trie.find("the")->terminal);
        assert(trie.find("these")->terminal);
        assert(trie.find("their")->terminal);
        assert(trie.find("thaw")->terminal);

        assert(trie.erase("th") == false);
        assert(trie.erase("these") == true);
        assert(trie.erase("thei") == false);
        assert(trie.erase("theirs") == false);
        assert(trie.erase("thaw") == true);

        assert(trie.find("the")->terminal);
        assert(trie.find("these") == nullptr);
        assert(trie.find("their")->terminal);
        assert(trie.find("thaw") == nullptr);

        assert(trie.insert("the") == false);
        assert(trie.insert("these") == true);
        assert(trie.insert("their") == false);
        assert(trie.insert("thaw") == true);

        assert(trie.find("the")->terminal);
        assert(trie.find("these")->terminal);
        assert(trie.find("their")->terminal);
        assert(trie.find("thaw")->terminal);

        assert(trie.erase("thee") == false);
        assert(trie.erase("these") == true);
        assert(trie.erase("thei") == false);
        assert(trie.erase("thaw") == true);

        assert(trie.find("the")->terminal);
        assert(trie.find("these") == nullptr);
        assert(trie.find("their")->terminal);
        assert(trie.find("thaw") == nullptr);

        auto t = trie.findLongestPrefix("t");
        assert(t.first == 1 && !t.second->terminal);
        t = trie.findLongestPrefix("th");
        assert(t.first == 2 && !t.second->terminal);
        t = trie.findLongestPrefix("the");
        assert(t.first == 3 && t.second->terminal);
        t = trie.findLongestPrefix("thei");
        assert(t.first == 4 && !t.second->terminal);
        t = trie.findLongestPrefix("their");
        assert(t.first == 5 && t.second->terminal);
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
            if (count(tree.root.children, tree.root.children + 26, nullptr) == 26)
                cerr << "What?" << endl;
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        {
            TrieAM<> tree;
            for (int i = 0; i < N; i++) {
                tree.insert(in[i]);
            }
            if (tree.root.children.empty())
                cerr << "What?" << endl;
        }
        PROFILE_STOP(1);
    }

    cout << "OK!" << endl;
}
