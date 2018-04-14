#include <memory.h>
#include <cassert>
#include <cstring>
#include <string>
#include <stack>
#include <memory>
#include <algorithm>
#include <iostream>

using namespace std;

#include "compressedTrie.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include <vector>
#include "../common/iostreamhelper.h"

static void dump(CompressedTrie::Node* p, int level) {
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

void testCompressedTrie() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Compressed Trie -------------------------------------" << endl;

    CompressedTrie compTrie;

    const char* keys[] = { "the", "a", "there", "answer", "any", "by", "bye", "their" };
    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
        compTrie.insert(keys[i], strlen(keys[i]));

    auto* x = compTrie.find("the", strlen("the"));
    assert(compTrie.find("the", strlen("the"))->leafCount == 1);
    assert(compTrie.find("these", strlen("these")) == nullptr);
    x = compTrie.find("their", strlen("their"));
    assert(compTrie.find("their", strlen("their"))->leafCount == 1);
    assert(compTrie.find("thaw", strlen("thaw")) == nullptr);

    compTrie.insert("the", strlen("the"));
    compTrie.insert("these", strlen("these"));
    compTrie.insert("their", strlen("their"));
    compTrie.insert("thaw", strlen("thaw"));

    assert(compTrie.find("th", strlen("th")) == nullptr);
    assert(compTrie.find("the", strlen("the"))->leafCount == 2);
    assert(compTrie.find("these", strlen("these"))->leafCount == 1);
    assert(compTrie.find("their", strlen("their"))->leafCount == 2);
    assert(compTrie.find("thaw", strlen("thaw"))->leafCount == 1);

    //cout << "*** after insertion ***" << endl;
    //dump(&compTrie.mRoot, 0);

    compTrie.remove("the", strlen("the"));
    compTrie.remove("these", strlen("these"));
    compTrie.remove("their", strlen("their"));
    compTrie.remove("thaw", strlen("thaw"));

    assert(compTrie.find("the", strlen("the"))->leafCount == 1);
    assert(compTrie.find("these", strlen("these")) == nullptr);
    assert(compTrie.find("their", strlen("their"))->leafCount == 1);
    assert(compTrie.find("thaw", strlen("thaw")) == nullptr);

    //cout << "*** after removal ***" << endl;
    //dump(&compTrie.mRoot, 0);

    compTrie.insert("the", strlen("the"));
    compTrie.insert("these", strlen("these"));
    compTrie.insert("their", strlen("their"));
    compTrie.insert("thaw", strlen("thaw"));

    assert(compTrie.find("the", strlen("the"))->leafCount == 2);
    assert(compTrie.find("these", strlen("these"))->leafCount == 1);
    assert(compTrie.find("their", strlen("their"))->leafCount == 2);
    assert(compTrie.find("thaw", strlen("thaw"))->leafCount == 1);

    //cout << "*** after insertion ***" << endl;
    //dump(&compTrie.mRoot, 0);

    compTrie.erase("the", strlen("the"));
    compTrie.erase("these", strlen("these"));
    compTrie.erase("their", strlen("their"));
    compTrie.erase("thaw", strlen("thaw"));

    compTrie.erase("any", strlen("any"));

    assert(compTrie.find("the", strlen("the"))->leafCount == 1);
    assert(compTrie.find("these", strlen("these")) == nullptr);
    assert(compTrie.find("their", strlen("their"))->leafCount == 1);
    assert(compTrie.find("thaw", strlen("thaw")) == nullptr);
    assert(compTrie.find("any", strlen("any")) == nullptr);

    assert(compTrie.search("t", strlen("t")) == make_pair(1, false));
    assert(compTrie.search("th", strlen("th")) == make_pair(2, false));
    assert(compTrie.search("the", strlen("the")) == make_pair(3, true));
    assert(compTrie.search("thei", strlen("thei")) == make_pair(4, false));
    assert(compTrie.search("their", strlen("their")) == make_pair(5, true));

    //cout << "*** after deletion ***" << endl;
    //dump(&compTrie.mRoot, 0);

    cout << "OK!" << endl;
}
