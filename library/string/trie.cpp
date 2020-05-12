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
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Trie ------------------------------------------------" << endl;

    Trie<> trie;

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

    cout << "OK!" << endl;
}
