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
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Trie ------------------------------------------------" << endl;

    Trie trie;

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

    cout << "OK!" << endl;
}
