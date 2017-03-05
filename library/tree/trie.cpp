#include <memory.h>
#include <cassert>
#include <cstring>
#include <string>
#include <stack>
#include <memory>
#include <algorithm>
#include <iostream>

using namespace std;

namespace TreeTrie {
#include "trie.h"
}

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include <vector>
#include "../common/iostreamhelper.h"

using namespace TreeTrie;

void dump(CompressedTrie::Node* p, int level) {
    if (!p)
        return;

    for (int i = 0; i < level; i++)
        cout << "+-";
    cout << "text=";
    for (int i = 0; i < p->textLen; i++)
        cout << p->text[i];
    cout << ", leaf=" << p->leafCount << endl;
    for (int i = 0; i < CompressedTrie::MaxCharN; i++) {
        if (p->children[i])
            dump(p->children[i], level + 1);
    }
}


void testTrie() {
    return; //TODO: if you want to test a split function, make this line to a comment.

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

    cout << "-- Compressed Trie -------------------------------------" << endl;

    CompressedTrie compTrie;

    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
        compTrie.insertWord(keys[i], strlen(keys[i]));

    assert(compTrie.findWord("the", strlen("the"))->leafCount == 1);
    assert(compTrie.findWord("these", strlen("these")) == nullptr);
    assert(compTrie.findWord("their", strlen("their"))->leafCount == 1);
    assert(compTrie.findWord("thaw", strlen("thaw")) == nullptr);

    compTrie.insertWord("the", strlen("the"));
    compTrie.insertWord("these", strlen("these"));
    compTrie.insertWord("their", strlen("their"));
    compTrie.insertWord("thaw", strlen("thaw"));

    assert(compTrie.findWord("th", strlen("th")) == nullptr);
    assert(compTrie.findWord("the", strlen("the"))->leafCount == 2);
    assert(compTrie.findWord("these", strlen("these"))->leafCount == 1);
    assert(compTrie.findWord("their", strlen("their"))->leafCount == 2);
    assert(compTrie.findWord("thaw", strlen("thaw"))->leafCount == 1);

    cout << "*** after insertion ***" << endl;
    dump(&compTrie.mRoot, 0);

    compTrie.removeWord("the", strlen("the"));
    compTrie.removeWord("these", strlen("these"));
    compTrie.removeWord("their", strlen("their"));
    compTrie.removeWord("thaw", strlen("thaw"));

    assert(compTrie.findWord("the", strlen("the"))->leafCount == 1);
    assert(compTrie.findWord("these", strlen("these")) == nullptr);
    assert(compTrie.findWord("their", strlen("their"))->leafCount == 1);
    assert(compTrie.findWord("thaw", strlen("thaw")) == nullptr);

    cout << "*** after removal ***" << endl;
    dump(&compTrie.mRoot, 0);

    compTrie.insertWord("the", strlen("the"));
    compTrie.insertWord("these", strlen("these"));
    compTrie.insertWord("their", strlen("their"));
    compTrie.insertWord("thaw", strlen("thaw"));

    assert(compTrie.findWord("the", strlen("the"))->leafCount == 2);
    assert(compTrie.findWord("these", strlen("these"))->leafCount == 1);
    assert(compTrie.findWord("their", strlen("their"))->leafCount == 2);
    assert(compTrie.findWord("thaw", strlen("thaw"))->leafCount == 1);

    cout << "*** after insertion ***" << endl;
    dump(&compTrie.mRoot, 0);

    compTrie.deleteWord("the", strlen("the"));
    compTrie.deleteWord("these", strlen("these"));
    compTrie.deleteWord("their", strlen("their"));
    compTrie.deleteWord("thaw", strlen("thaw"));

    //compTrie.insertWord("an", strlen("an"));
    compTrie.deleteWord("any", strlen("any"));

    assert(compTrie.findWord("the", strlen("the"))->leafCount == 1);
    assert(compTrie.findWord("these", strlen("these")) == nullptr);
    assert(compTrie.findWord("their", strlen("their"))->leafCount == 1);
    assert(compTrie.findWord("thaw", strlen("thaw")) == nullptr);
    assert(compTrie.findWord("any", strlen("any")) == nullptr);

    assert(compTrie.searchWord("t", strlen("t")) == make_pair(1, false));
    assert(compTrie.searchWord("th", strlen("th")) == make_pair(2, false));
    assert(compTrie.searchWord("the", strlen("the")) == make_pair(3, true));
    assert(compTrie.searchWord("thei", strlen("thei")) == make_pair(4, false));
    assert(compTrie.searchWord("their", strlen("their")) == make_pair(5, true));

    cout << "*** after deletion ***" << endl;
    dump(&compTrie.mRoot, 0);

    cout << "OK!" << endl;
}
