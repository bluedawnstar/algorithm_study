#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

#include "triePersistent.h"
#include "trieAMPersistent.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

void testTriePersistent() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Persistent Trie --------------------------------" << endl;
    {
        PersistentTrie<> trie;

        const char* keys[] = { "the", "a", "there", "answer", "any", "by", "bye", "their" };
        vector<int> roots;
        roots.push_back(0);
        for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
            auto res = trie.insert(roots.back(), keys[i], int(strlen(keys[i])));
            roots.push_back(res.first);
        }

        int node;
        {
            node = trie.find(roots.back(), "the", int(strlen("the")));
            assert(node >= 0 && trie.nodes[node].leafCount == 1);
            node = trie.find(roots.back(), "these", int(strlen("these")));
            assert(node == -1);
            node = trie.find(roots.back(), "their", int(strlen("their")));
            assert(node >= 0 && trie.nodes[node].leafCount == 1);
            node = trie.find(roots.back(), "thaw", int(strlen("thaw")));
            assert(node == -1);
        }
        int root1 = roots.back();
        roots.push_back(trie.insert(roots.back(), "the", int(strlen("the"))).first);
        roots.push_back(trie.insert(roots.back(), "these", int(strlen("these"))).first);
        roots.push_back(trie.insert(roots.back(), "their", int(strlen("their"))).first);
        roots.push_back(trie.insert(roots.back(), "thaw", int(strlen("thaw"))).first);
        {
            node = trie.find(root1, "the", int(strlen("the")));
            assert(node >= 0 && trie.nodes[node].leafCount == 1);
            node = trie.find(root1, "these", int(strlen("these")));
            assert(node == -1);
            node = trie.find(root1, "their", int(strlen("their")));
            assert(node >= 0 && trie.nodes[node].leafCount == 1);
            node = trie.find(root1, "thaw", int(strlen("thaw")));
            assert(node == -1);
        }
        {
            node = trie.find(roots.back(), "the", int(strlen("the")));
            assert(node >= 0 && trie.nodes[node].leafCount == 2);
            node = trie.find(roots.back(), "these", int(strlen("these")));
            assert(node >= 0 && trie.nodes[node].leafCount == 1);
            node = trie.find(roots.back(), "their", int(strlen("their")));
            assert(node >= 0 && trie.nodes[node].leafCount == 2);
            node = trie.find(roots.back(), "thaw", int(strlen("thaw")));
            assert(node >= 0 && trie.nodes[node].leafCount == 1);
        }
    }
    cout << "OK!" << endl;
    {
        PersistentTrieAM<> trie;

        const char* keys[] = { "the", "a", "there", "answer", "any", "by", "bye", "their" };
        vector<int> roots;
        roots.push_back(0);
        for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
            auto res = trie.insert(roots.back(), keys[i], int(strlen(keys[i])));
            roots.push_back(res.first);
        }

        int node;
        {
            node = trie.find(roots.back(), "the", int(strlen("the")));
            assert(node >= 0 && trie.nodes[node].leafCount == 1);
            node = trie.find(roots.back(), "these", int(strlen("these")));
            assert(node == -1);
            node = trie.find(roots.back(), "their", int(strlen("their")));
            assert(node >= 0 && trie.nodes[node].leafCount == 1);
            node = trie.find(roots.back(), "thaw", int(strlen("thaw")));
            assert(node == -1);
        }
        int root1 = roots.back();
        roots.push_back(trie.insert(roots.back(), "the", int(strlen("the"))).first);
        roots.push_back(trie.insert(roots.back(), "these", int(strlen("these"))).first);
        roots.push_back(trie.insert(roots.back(), "their", int(strlen("their"))).first);
        roots.push_back(trie.insert(roots.back(), "thaw", int(strlen("thaw"))).first);
        {
            node = trie.find(root1, "the", int(strlen("the")));
            assert(node >= 0 && trie.nodes[node].leafCount == 1);
            node = trie.find(root1, "these", int(strlen("these")));
            assert(node == -1);
            node = trie.find(root1, "their", int(strlen("their")));
            assert(node >= 0 && trie.nodes[node].leafCount == 1);
            node = trie.find(root1, "thaw", int(strlen("thaw")));
            assert(node == -1);
        }
        {
            node = trie.find(roots.back(), "the", int(strlen("the")));
            assert(node >= 0 && trie.nodes[node].leafCount == 2);
            node = trie.find(roots.back(), "these", int(strlen("these")));
            assert(node >= 0 && trie.nodes[node].leafCount == 1);
            node = trie.find(roots.back(), "their", int(strlen("their")));
            assert(node >= 0 && trie.nodes[node].leafCount == 2);
            node = trie.find(roots.back(), "thaw", int(strlen("thaw")));
            assert(node >= 0 && trie.nodes[node].leafCount == 1);
        }
    }
    cout << "OK!" << endl;
}
