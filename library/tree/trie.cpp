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
        cout << "++";
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
    //return; //TODO: if you want to test a split function, make this line to a comment.

    cout << "-- Trie ------------------------------------------------" << endl;

    Trie trie;

    const char* keys[] = { "the", "a", "there", "answer", "any", "by", "bye", "their" };
    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
        trie.insertKey(keys[i], strlen(keys[i]));

    assert(trie.searchKey("the", strlen("the"))->leafCount == 1);
    assert(trie.searchKey("these", strlen("these")) == nullptr);
    assert(trie.searchKey("their", strlen("their"))->leafCount == 1);
    assert(trie.searchKey("thaw", strlen("thaw")) == nullptr);

    trie.insertKey("the", strlen("the"));
    trie.insertKey("these", strlen("these"));
    trie.insertKey("their", strlen("their"));
    trie.insertKey("thaw", strlen("thaw"));

    assert(trie.searchKey("the", strlen("the"))->leafCount == 2);
    assert(trie.searchKey("these", strlen("these"))->leafCount == 1);
    assert(trie.searchKey("their", strlen("their"))->leafCount == 2);
    assert(trie.searchKey("thaw", strlen("thaw"))->leafCount == 1);

    trie.removeKey("the", strlen("the"));
    trie.removeKey("these", strlen("these"));
    trie.removeKey("their", strlen("their"));
    trie.removeKey("thaw", strlen("thaw"));

    assert(trie.searchKey("the", strlen("the"))->leafCount == 1);
    assert(trie.searchKey("these", strlen("these")) == nullptr);
    assert(trie.searchKey("their", strlen("their"))->leafCount == 1);
    assert(trie.searchKey("thaw", strlen("thaw")) == nullptr);

    trie.insertKey("the", strlen("the"));
    trie.insertKey("these", strlen("these"));
    trie.insertKey("their", strlen("their"));
    trie.insertKey("thaw", strlen("thaw"));

    assert(trie.searchKey("the", strlen("the"))->leafCount == 2);
    assert(trie.searchKey("these", strlen("these"))->leafCount == 1);
    assert(trie.searchKey("their", strlen("their"))->leafCount == 2);
    assert(trie.searchKey("thaw", strlen("thaw"))->leafCount == 1);

    trie.deleteKey("the", strlen("the"));
    trie.deleteKey("these", strlen("these"));
    trie.deleteKey("their", strlen("their"));
    trie.deleteKey("thaw", strlen("thaw"));

    assert(trie.searchKey("the", strlen("the"))->leafCount == 1);
    assert(trie.searchKey("these", strlen("these")) == nullptr);
    assert(trie.searchKey("their", strlen("their"))->leafCount == 1);
    assert(trie.searchKey("thaw", strlen("thaw")) == nullptr);

    assert(trie.prefixKey("t", strlen("t")) == make_pair(1, false));
    assert(trie.prefixKey("th", strlen("th")) == make_pair(2, false));
    assert(trie.prefixKey("the", strlen("the")) == make_pair(3, true));
    assert(trie.prefixKey("thei", strlen("thei")) == make_pair(4, false));
    assert(trie.prefixKey("their", strlen("their")) == make_pair(5, true));

    cout << "-- Compressed Trie -------------------------------------" << endl;

    CompressedTrie compTrie;

    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
        compTrie.insertKey(keys[i], strlen(keys[i]));

    assert(compTrie.searchKey("the", strlen("the"))->leafCount == 1);
    assert(compTrie.searchKey("these", strlen("these")) == nullptr);
    assert(compTrie.searchKey("their", strlen("their"))->leafCount == 1);
    assert(compTrie.searchKey("thaw", strlen("thaw")) == nullptr);

    compTrie.insertKey("the", strlen("the"));
    compTrie.insertKey("these", strlen("these"));
    compTrie.insertKey("their", strlen("their"));
    compTrie.insertKey("thaw", strlen("thaw"));

    assert(compTrie.searchKey("th", strlen("th")) == nullptr);
    assert(compTrie.searchKey("the", strlen("the"))->leafCount == 2);
    assert(compTrie.searchKey("these", strlen("these"))->leafCount == 1);
    assert(compTrie.searchKey("their", strlen("their"))->leafCount == 2);
    assert(compTrie.searchKey("thaw", strlen("thaw"))->leafCount == 1);

    cout << "*** after insertion ***" << endl;
    dump(&compTrie.mRoot, 0);

    compTrie.removeKey("the", strlen("the"));
    compTrie.removeKey("these", strlen("these"));
    compTrie.removeKey("their", strlen("their"));
    compTrie.removeKey("thaw", strlen("thaw"));

    assert(compTrie.searchKey("the", strlen("the"))->leafCount == 1);
    assert(compTrie.searchKey("these", strlen("these")) == nullptr);
    assert(compTrie.searchKey("their", strlen("their"))->leafCount == 1);
    assert(compTrie.searchKey("thaw", strlen("thaw")) == nullptr);

    cout << "*** after removal ***" << endl;
    dump(&compTrie.mRoot, 0);

    compTrie.insertKey("the", strlen("the"));
    compTrie.insertKey("these", strlen("these"));
    compTrie.insertKey("their", strlen("their"));
    compTrie.insertKey("thaw", strlen("thaw"));

    assert(compTrie.searchKey("the", strlen("the"))->leafCount == 2);
    assert(compTrie.searchKey("these", strlen("these"))->leafCount == 1);
    assert(compTrie.searchKey("their", strlen("their"))->leafCount == 2);
    assert(compTrie.searchKey("thaw", strlen("thaw"))->leafCount == 1);

    cout << "*** after insertion ***" << endl;
    dump(&compTrie.mRoot, 0);

    compTrie.deleteKey("the", strlen("the"));
    compTrie.deleteKey("these", strlen("these"));
    compTrie.deleteKey("their", strlen("their"));
    compTrie.deleteKey("thaw", strlen("thaw"));
    compTrie.deleteKey("any", strlen("any"), false);

    assert(compTrie.searchKey("the", strlen("the"))->leafCount == 1);
    assert(compTrie.searchKey("these", strlen("these")) == nullptr);
    assert(compTrie.searchKey("their", strlen("their"))->leafCount == 1);
    assert(compTrie.searchKey("thaw", strlen("thaw")) == nullptr);
    assert(compTrie.searchKey("any", strlen("any")) == nullptr);

    assert(compTrie.prefixKey("t", strlen("t")) == make_pair(1, false));
    assert(compTrie.prefixKey("th", strlen("th")) == make_pair(2, false));
    assert(compTrie.prefixKey("the", strlen("the")) == make_pair(3, true));
    assert(compTrie.prefixKey("thei", strlen("thei")) == make_pair(4, false));
    assert(compTrie.prefixKey("their", strlen("their")) == make_pair(5, true));

    cout << "*** after deletion ***" << endl;
    dump(&compTrie.mRoot, 0);

    cout << "OK!" << endl;
}
