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

        string keys[] = { "the", "a", "there", "answer", "any", "by", "bye", "their" };
        vector<int> roots;
        roots.push_back(0);
        for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
            auto res = trie.insert(roots.back(), keys[i]);
            roots.push_back(res);
        }

        int node;
        {
            node = trie.find(roots.back(), "the");
            assert(node >= 0 && trie.nodes[node].terminal);
            node = trie.find(roots.back(), "these");
            assert(node == -1);
            node = trie.find(roots.back(), "their");
            assert(node >= 0 && trie.nodes[node].terminal);
            node = trie.find(roots.back(), "thaw");
            assert(node == -1);
        }
        int root1 = roots.back();
        roots.push_back(trie.insert(roots.back(), "the"));
        roots.push_back(trie.insert(roots.back(), "these"));
        roots.push_back(trie.insert(roots.back(), "their"));
        roots.push_back(trie.insert(roots.back(), "thaw"));
        {
            node = trie.find(root1, "the");
            assert(node >= 0 && trie.nodes[node].terminal);
            node = trie.find(root1, "these");
            assert(node == -1);
            node = trie.find(root1, "their");
            assert(node >= 0 && trie.nodes[node].terminal);
            node = trie.find(root1, "thaw");
            assert(node == -1);
        }
        {
            node = trie.find(roots.back(), "the");
            assert(node >= 0 && trie.nodes[node].terminal);
            node = trie.find(roots.back(), "these");
            assert(node >= 0 && trie.nodes[node].terminal);
            node = trie.find(roots.back(), "their");
            assert(node >= 0 && trie.nodes[node].terminal);
            node = trie.find(roots.back(), "thaw");
            assert(node >= 0 && trie.nodes[node].terminal);
        }
    }
    cout << "OK!" << endl;
    {
        PersistentTrieAM<> trie;

        string keys[] = { "the", "a", "there", "answer", "any", "by", "bye", "their" };
        vector<int> roots;
        roots.push_back(0);
        for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
            auto res = trie.insert(roots.back(), keys[i]);
            roots.push_back(res);
        }

        int node;
        {
            node = trie.find(roots.back(), "the");
            assert(node >= 0 && trie.nodes[node].terminal);
            node = trie.find(roots.back(), "these");
            assert(node == -1);
            node = trie.find(roots.back(), "their");
            assert(node >= 0 && trie.nodes[node].terminal);
            node = trie.find(roots.back(), "thaw");
            assert(node == -1);
        }
        int root1 = roots.back();
        roots.push_back(trie.insert(roots.back(), "the"));
        roots.push_back(trie.insert(roots.back(), "these"));
        roots.push_back(trie.insert(roots.back(), "their"));
        roots.push_back(trie.insert(roots.back(), "thaw"));
        {
            node = trie.find(root1, "the");
            assert(node >= 0 && trie.nodes[node].terminal);
            node = trie.find(root1, "these");
            assert(node == -1);
            node = trie.find(root1, "their");
            assert(node >= 0 && trie.nodes[node].terminal);
            node = trie.find(root1, "thaw");
            assert(node == -1);
        }
        {
            node = trie.find(roots.back(), "the");
            assert(node >= 0 && trie.nodes[node].terminal);
            node = trie.find(roots.back(), "these");
            assert(node >= 0 && trie.nodes[node].terminal);
            node = trie.find(roots.back(), "their");
            assert(node >= 0 && trie.nodes[node].terminal);
            node = trie.find(roots.back(), "thaw");
            assert(node >= 0 && trie.nodes[node].terminal);
        }
    }
    cout << "OK!" << endl;
}
