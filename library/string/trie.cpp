#include <memory.h>
#include <cassert>
#include <cstring>
#include <string>
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
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Trie ------------------------------------------------" << endl;

    Trie trie;

    const char* keys[] = { "the", "a", "there", "answer", "any", "by", "bye", "their" };
    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
        trie.insertWord(keys[i], strlen(keys[i]));

    assert(trie.findWord("the", strlen("the"))->leafCount == 1);
    assert(trie.findWord("these", strlen("these")) == nullptr);
    assert(trie.findWord("their", strlen("their"))->leafCount == 1);
    assert(trie.findWord("thaw", strlen("thaw")) == nullptr);

    trie.insertWord("the", strlen("the"));
    trie.insertWord("these", strlen("these"));
    trie.insertWord("their", strlen("their"));
    trie.insertWord("thaw", strlen("thaw"));

    assert(trie.findWord("the", strlen("the"))->leafCount == 2);
    assert(trie.findWord("these", strlen("these"))->leafCount == 1);
    assert(trie.findWord("their", strlen("their"))->leafCount == 2);
    assert(trie.findWord("thaw", strlen("thaw"))->leafCount == 1);

    trie.removeWord("the", strlen("the"));
    trie.removeWord("these", strlen("these"));
    trie.removeWord("their", strlen("their"));
    trie.removeWord("thaw", strlen("thaw"));

    assert(trie.findWord("the", strlen("the"))->leafCount == 1);
    assert(trie.findWord("these", strlen("these")) == nullptr);
    assert(trie.findWord("their", strlen("their"))->leafCount == 1);
    assert(trie.findWord("thaw", strlen("thaw")) == nullptr);

    trie.insertWord("the", strlen("the"));
    trie.insertWord("these", strlen("these"));
    trie.insertWord("their", strlen("their"));
    trie.insertWord("thaw", strlen("thaw"));

    assert(trie.findWord("the", strlen("the"))->leafCount == 2);
    assert(trie.findWord("these", strlen("these"))->leafCount == 1);
    assert(trie.findWord("their", strlen("their"))->leafCount == 2);
    assert(trie.findWord("thaw", strlen("thaw"))->leafCount == 1);

    trie.deleteWord("the", strlen("the"));
    trie.deleteWord("these", strlen("these"));
    trie.deleteWord("their", strlen("their"));
    trie.deleteWord("thaw", strlen("thaw"));

    assert(trie.findWord("the", strlen("the"))->leafCount == 1);
    assert(trie.findWord("these", strlen("these")) == nullptr);
    assert(trie.findWord("their", strlen("their"))->leafCount == 1);
    assert(trie.findWord("thaw", strlen("thaw")) == nullptr);

    assert(trie.searchWord("t", strlen("t")) == make_pair(1, false));
    assert(trie.searchWord("th", strlen("th")) == make_pair(2, false));
    assert(trie.searchWord("the", strlen("the")) == make_pair(3, true));
    assert(trie.searchWord("thei", strlen("thei")) == make_pair(4, false));
    assert(trie.searchWord("their", strlen("their")) == make_pair(5, true));

    cout << "OK!" << endl;
}
