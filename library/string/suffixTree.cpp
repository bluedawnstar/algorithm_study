#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

#include "suffixTree.h"
#include "suffixTreeAlgo.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include <vector>
#include "../common/iostreamhelper.h"

static void dump(SuffixTree& tree, SuffixTree::Node* p, int idx, int level, bool showSuffixIndex = false) {
    if (!p)
        return;

    for (int i = 0; i < level; i++)
        cout << "+-";
    if (idx >= 0) {
        cout << " ";
        for (int i = 0; i < p->getLength(); i++)
            cout << tree.mText[p->begin + i];
        if (showSuffixIndex)
            cout << " (" << p->suffixIndex << ")";
        cout << endl;
    }
    for (int i = 0; i < tree.charSize(); i++) {
        if (p->hasChild(i))
            dump(tree, p->getChild(i), i, level + 1, showSuffixIndex);
    }
}


void testSuffixTree() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Suffix Tree -----------------------------------------" << endl;

    {
        SuffixTree tree(100);
        const char* s;

        const char* treeS = "abdadafaaabdfaeef";
        tree.build(treeS, strlen(treeS));

        dump(tree, &tree.mRoot, -1, 0, true);

        s = "afaabda"; assert(tree.search(s, strlen(s)) == make_pair(4, -1));
        s = "abdadafaaabdfaeef"; assert(tree.search(s, strlen(s)) == make_pair((int)strlen(s), 0));
        s = "aeef"; assert(tree.search(s, strlen(s)) == make_pair((int)strlen(s), (int)strlen(treeS) - (int)strlen(s)));
        s = "abdfaeef"; assert(tree.search(s, strlen(s)) == make_pair((int)strlen(s), (int)strlen(treeS) - (int)strlen(s)));
    }
    {
        SuffixTree tree(100);
        const char* s;

        s = "a"; assert(tree.search(s, strlen(s)) == make_pair(0, -1));
        s = "ab"; assert(tree.search(s, strlen(s)) == make_pair(0, -1));
        s = "b"; assert(tree.search(s, strlen(s)) == make_pair(0, -1));
        s = "c"; assert(tree.search(s, strlen(s)) == make_pair(0, -1));

        tree.extend('a');
        cout << "*** after extend 'a'" << endl;
        dump(tree, &tree.mRoot, -1, 0);
        s = "a"; assert(tree.search(s, strlen(s)) == make_pair(1, -1));
        s = "ab"; assert(tree.search(s, strlen(s)) == make_pair(1, -1));
        s = "b"; assert(tree.search(s, strlen(s)) == make_pair(0, -1));
        s = "c"; assert(tree.search(s, strlen(s)) == make_pair(0, -1));

        tree.extend('b');
        cout << "*** after extend 'b'" << endl;
        dump(tree, &tree.mRoot, -1, 0);
        s = "a"; assert(tree.search(s, strlen(s)) == make_pair(1, -1));
        s = "ab"; assert(tree.search(s, strlen(s)) == make_pair(2, -1));
        s = "b"; assert(tree.search(s, strlen(s)) == make_pair(1, -1));
        s = "c"; assert(tree.search(s, strlen(s)) == make_pair(0, -1));

        cout << "OK!" << endl;
    }

    cout << "*** suffix tree of \"xabxac\" ***" << endl;
    {
        SuffixTree tree(100);

        const char* treeS = "xabxac";
        {
            for (int i = 0; treeS[i]; i++) {
                tree.extend(treeS[i]);
                cout << "*** character '" << treeS[i] << "' added " << endl;
                dump(tree, &tree.mRoot, -1, 0);
            }

            tree.setSuffixIndex();
            cout << "*** with suffix index *** " << endl;
            dump(tree, &tree.mRoot, -1, 0, true);
        }
    }
}
