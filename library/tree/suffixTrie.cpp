#include <memory.h>
#include <cassert>
#include <cstring>
#include <string>
#include <stack>
#include <memory>
#include <algorithm>
#include <iostream>

using namespace std;

namespace TrieSuffix {
#include "suffixTrie.h"
}


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include <vector>
#include "../common/iostreamhelper.h"

using namespace TrieSuffix;

void dump(SuffixTrie::Node* p, int idx, int level) {
    if (!p)
        return;

    for (int i = 0; i < level; i++)
        cout << "+-";
    if (idx >= 0)
        cout << " " << char('a' + idx) << endl;

    for (int i = 0; i < SuffixTrie::MaxCharN; i++) {
        if (p->children[i])
            dump(p->children[i], i, level + 1);
    }
}


void testSuffixTrie() {
    return; //TODO: if you want to test a split function, make this line to a comment.

    cout << "-- Suffix Trie -----------------------------------------" << endl;

    const char* s;
    SuffixTrie trie;

    // test buildSuffix()

    const char* trieS = "abdadafaaabdfaeef";
    trie.build(trieS, strlen(trieS));

    cout << "*** after build ***" << endl;
    dump(&trie.mRoot, -1, 0);

    s = "afaabda"; assert(trie.search(s, strlen(s)) == make_pair(4, -1));
    s = "abdadafaaabdfaeef"; assert(trie.search(s, strlen(s)) == make_pair((int)strlen(s), 0));
    s = "aeef"; assert(trie.search(s, strlen(s)) == make_pair((int)strlen(s), (int)strlen(trieS) - (int)strlen(s)));
    s = "abdfaeef"; assert(trie.search(s, strlen(s)) == make_pair((int)strlen(s), (int)strlen(trieS) - (int)strlen(s)));

    // test extendSuffix() and shrinkSuffix()

    trie.clear();

    cout << "*** after clear ***" << endl;
    dump(&trie.mRoot, -1, 0);
    s = "a"; assert(trie.search(s, strlen(s)) == make_pair(0, -1));
    s = "ab"; assert(trie.search(s, strlen(s)) == make_pair(0, -1));
    s = "b"; assert(trie.search(s, strlen(s)) == make_pair(0, -1));
    s = "c"; assert(trie.search(s, strlen(s)) == make_pair(0, -1));

    trie.extendSuffix('a');
    cout << "*** after extend 'a'" << endl;
    dump(&trie.mRoot, -1, 0);
    s = "a"; assert(trie.search(s, strlen(s)) == make_pair(1, -1));
    s = "ab"; assert(trie.search(s, strlen(s)) == make_pair(1, -1));
    s = "b"; assert(trie.search(s, strlen(s)) == make_pair(0, -1));
    s = "c"; assert(trie.search(s, strlen(s)) == make_pair(0, -1));

    trie.extendSuffix('b');
    cout << "*** after extend 'b'" << endl;
    dump(&trie.mRoot, -1, 0);
    s = "a"; assert(trie.search(s, strlen(s)) == make_pair(1, -1));
    s = "ab"; assert(trie.search(s, strlen(s)) == make_pair(2, -1));
    s = "b"; assert(trie.search(s, strlen(s)) == make_pair(1, -1));
    s = "c"; assert(trie.search(s, strlen(s)) == make_pair(0, -1));

    trie.shrinkSuffix();
    cout << "*** after shink" << endl;
    dump(&trie.mRoot, -1, 0);
    s = "a"; assert(trie.search(s, strlen(s)) == make_pair(1, -1));
    s = "ab"; assert(trie.search(s, strlen(s)) == make_pair(1, -1));
    s = "b"; assert(trie.search(s, strlen(s)) == make_pair(0, -1));
    s = "c"; assert(trie.search(s, strlen(s)) == make_pair(0, -1));

    trie.shrinkSuffix();
    cout << "*** after shink" << endl;
    dump(&trie.mRoot, -1, 0);
    s = "a"; assert(trie.search(s, strlen(s)) == make_pair(0, -1));
    s = "ab"; assert(trie.search(s, strlen(s)) == make_pair(0, -1));
    s = "b"; assert(trie.search(s, strlen(s)) == make_pair(0, -1));
    s = "c"; assert(trie.search(s, strlen(s)) == make_pair(0, -1));

    cout << "OK!" << endl;
}
