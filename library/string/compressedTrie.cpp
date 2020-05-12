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

static void dump(CompressedTrie<>::Node* p, int level) {
    if (!p)
        return;

    for (int i = 0; i < level; i++)
        cout << "+-";
    cout << "text=";
    for (int i = 0; i < p->textLen; i++)
        cout << p->text[i];
    cout << ", leaf=" << p->leafCount << endl;
    for (int i = 0; i < 26; i++) {
        if (p->children[i])
            dump(p->children[i], level + 1);
    }
}

void testCompressedTrie() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Compressed Trie -------------------------------------" << endl;

    CompressedTrie<> compTrie;

    const char* keys[] = { "the", "a", "there", "answer", "any", "by", "bye", "their" };
    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
        compTrie.insert(keys[i], int(strlen(keys[i])));

    auto* x = compTrie.find("the", int(strlen("the")));
    assert(compTrie.find("the", int(strlen("the")))->leafCount == 1);
    assert(compTrie.find("these", int(strlen("these"))) == nullptr);
    x = compTrie.find("their", int(strlen("their")));
    assert(compTrie.find("their", int(strlen("their")))->leafCount == 1);
    assert(compTrie.find("thaw", int(strlen("thaw"))) == nullptr);

    compTrie.insert("the", int(strlen("the")));
    compTrie.insert("these", int(strlen("these")));
    compTrie.insert("their", int(strlen("their")));
    compTrie.insert("thaw", int(strlen("thaw")));

    assert(compTrie.find("th", int(strlen("th"))) == nullptr);
    assert(compTrie.find("the", int(strlen("the")))->leafCount == 2);
    assert(compTrie.find("these", int(strlen("these")))->leafCount == 1);
    assert(compTrie.find("their", int(strlen("their")))->leafCount == 2);
    assert(compTrie.find("thaw", int(strlen("thaw")))->leafCount == 1);

    //cout << "*** after insertion ***" << endl;
    //dump(&compTrie.mRoot, 0);

    compTrie.remove("the", int(strlen("the")));
    compTrie.remove("these", int(strlen("these")));
    compTrie.remove("their", int(strlen("their")));
    compTrie.remove("thaw", int(strlen("thaw")));

    assert(compTrie.find("the", int(strlen("the")))->leafCount == 1);
    assert(compTrie.find("these", int(strlen("these"))) == nullptr);
    assert(compTrie.find("their", int(strlen("their")))->leafCount == 1);
    assert(compTrie.find("thaw", int(strlen("thaw"))) == nullptr);

    //cout << "*** after removal ***" << endl;
    //dump(&compTrie.mRoot, 0);

    compTrie.insert("the", int(strlen("the")));
    compTrie.insert("these", int(strlen("these")));
    compTrie.insert("their", int(strlen("their")));
    compTrie.insert("thaw", int(strlen("thaw")));

    assert(compTrie.find("the", int(strlen("the")))->leafCount == 2);
    assert(compTrie.find("these", int(strlen("these")))->leafCount == 1);
    assert(compTrie.find("their", int(strlen("their")))->leafCount == 2);
    assert(compTrie.find("thaw", int(strlen("thaw")))->leafCount == 1);

    //cout << "*** after insertion ***" << endl;
    //dump(&compTrie.mRoot, 0);

    compTrie.erase("the", int(strlen("the")));
    compTrie.erase("these", int(strlen("these")));
    compTrie.erase("their", int(strlen("their")));
    compTrie.erase("thaw", int(strlen("thaw")));

    compTrie.erase("any", int(strlen("any")));

    assert(compTrie.find("the", int(strlen("the")))->leafCount == 1);
    assert(compTrie.find("these", int(strlen("these"))) == nullptr);
    assert(compTrie.find("their", int(strlen("their")))->leafCount == 1);
    assert(compTrie.find("thaw", int(strlen("thaw"))) == nullptr);
    assert(compTrie.find("any", int(strlen("any"))) == nullptr);

    assert(compTrie.search("t", int(strlen("t"))) == make_pair(1, false));
    assert(compTrie.search("th", int(strlen("th"))) == make_pair(2, false));
    assert(compTrie.search("the", int(strlen("the"))) == make_pair(3, true));
    assert(compTrie.search("thei", int(strlen("thei"))) == make_pair(4, false));
    assert(compTrie.search("their", int(strlen("their"))) == make_pair(5, true));

    //cout << "*** after deletion ***" << endl;
    //dump(&compTrie.mRoot, 0);

    cout << "OK!" << endl;
}
