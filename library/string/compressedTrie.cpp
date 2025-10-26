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
    cout << "text=" << p->text
         << ", terminal=" << p->terminal
         << ", children=" << p->countChildren() << endl;
    for (int i = 0; i < 26; i++) {
        if (p->children[i])
            dump(p->children[i], level + 1);
    }
}

void testCompressedTrie() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Compressed Trie -------------------------------------" << endl;

    CompressedTrie<> trie;

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

    //cout << "*** after insertion ***" << endl;
    //dump(&trie.root, 0);

    assert(trie.erase("th") == false);
    assert(trie.erase("these") == true);
    assert(trie.erase("thei") == false);
    assert(trie.erase("theirs") == false);
    assert(trie.erase("thaw") == true);

    assert(trie.find("the")->terminal);
    assert(trie.find("these") == nullptr);
    assert(trie.find("their")->terminal);
    assert(trie.find("thaw") == nullptr);

    //cout << "*** after erase #1 ***" << endl;
    //dump(&trie.root, 0);

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

    //cout << "*** after erase #2 ***" << endl;
    //dump(&trie.root, 0);

    auto t = trie.findLongestPrefix("t");
    assert(get<0>(t) == 1 && get<1>(t)->terminal && get<2>(t) == 1);
    t = trie.findLongestPrefix("th");
    assert(get<0>(t) == 2 && get<1>(t)->terminal && get<2>(t) == 2);
    t = trie.findLongestPrefix("the");
    assert(get<0>(t) == 3 && get<1>(t)->terminal && get<2>(t) == 3);
    t = trie.findLongestPrefix("thei");
    assert(get<0>(t) == 4 && get<1>(t)->terminal && get<2>(t) == 1);
    t = trie.findLongestPrefix("their");
    assert(get<0>(t) == 5 && get<1>(t)->terminal && get<2>(t) == 2);

    cout << "OK!" << endl;
}
