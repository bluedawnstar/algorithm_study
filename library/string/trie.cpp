#include <memory.h>
#include <cassert>
#include <cstring>
#include <string>
#include <vector>
#include <stack>
#include <memory>
#include <algorithm>
#include <iostream>

using namespace std;

#include "trie.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include <vector>
#include "../common/iostreamhelper.h"

void testTrie() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Trie -------------------------------------------" << endl;

    Trie<> trie;

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

    cout << "OK!" << endl;
}
