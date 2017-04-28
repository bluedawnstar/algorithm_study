#include <memory.h>
#include <cassert>
#include <cstring>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

// https://saisumit.wordpress.com/2016/01/26/suffix-automaton/

namespace SuffixAutomationNS {
#include "suffixAutomation.h"
}
using namespace SuffixAutomationNS;

#define MAXN    100000

//1. Number of different substrings of a given string .
static vector<long long> gD(MAXN + 10, 0);
long long countSubstrings(SuffixAutomation& sa, int u) {
    if (gD[u])
        return gD[u];

    long long res = 1;

    for (int i = 0; i < SuffixAutomation::MaxCharN; i++) {
        if (sa.state[u].edges[i])
            res += countSubstrings(sa, sa.state[u].edges[i]);
    }

    return gD[u] = res;
}

//
//2. Total length of various substrings.
//3. Lexographically kth substring.
//4. smallest Cyclic shift.
//5. No.of occurrences of a pattern in the given Text.
//6. Position of all Occurrences.

//7. Longest common substring.
// 
// It's better that t's length is shorter than SuffixAutomation's length
int lcs(SuffixAutomation& sa, string& t) {
    int v = 0, l = 0, best = 0, bestpos = 0;
    for (int i = 0; i < (int)t.length(); ++i) {
        while (v && !sa.state[v].edges[t[i] - 'a']) {
            v = sa.state[v].suffixLink;
            l = sa.state[v].len;
        }
        if (sa.state[v].edges[t[i] - 'a']) {
            v = sa.state[v].edges[t[i] - 'a'];
            ++l;
        }
        if (l > best)
            best = l, bestpos = i;
    }
    return best;
}

string lcsStr(SuffixAutomation& sa, string& t) {
    int v = 0, l = 0, best = 0, bestpos = 0;
    for (int i = 0; i < (int)t.length(); ++i) {
        while (v && !sa.state[v].edges[t[i] - 'a']) {
            v = sa.state[v].suffixLink;
            l = sa.state[v].len;
        }
        if (sa.state[v].edges[t[i] - 'a']) {
            v = sa.state[v].edges[t[i] - 'a'];
            ++l;
        }
        if (l > best)
            best = l, bestpos = i;
    }
    return t.substr(bestpos - best + 1, best);
}

//8. Longest common substring of multiple substring.
//9. Search for shortest substring that is not included in this string.
//

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include <vector>
#include "../common/iostreamhelper.h"

/*
void dump(SuffixTree& tree, SuffixTree::Node* p, int idx, int level, bool showSuffixIndex = false) {
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
    for (int i = 0; i < SuffixTree::MaxCharN; i++) {
        if (p->children[i])
            dump(tree, p->children[i], i, level + 1, showSuffixIndex);
    }
}
*/

void testSuffixAutomation() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Suffix Automation -----------------------------------" << endl;

    cout << "Test cases are not ready." << endl;
    
    /*
    {
        SuffixAutomation sa(100);
        const char* s;

        const char* treeS = "abdadafaaabdfaeef";
        tree.build(treeS, strlen(treeS));

        cout << "*** after build ***" << endl;
        dump(tree, &tree.mRoot, -1, 0, true);

        s = "afaabda"; assert(tree.search(s, strlen(s)) == make_pair(4, -1));
        s = "abdadafaaabdfaeef"; assert(tree.search(s, strlen(s)) == make_pair((int)strlen(s), 0));
        s = "aeef"; assert(tree.search(s, strlen(s)) == make_pair((int)strlen(s), (int)strlen(treeS) - (int)strlen(s)));
        s = "abdfaeef"; assert(tree.search(s, strlen(s)) == make_pair((int)strlen(s), (int)strlen(treeS) - (int)strlen(s)));

        // test extendSuffix()

        tree.clear();

        cout << "*** after clear ***" << endl;
        dump(tree, &tree.mRoot, -1, 0);
        s = "a"; assert(tree.search(s, strlen(s)) == make_pair(0, -1));
        s = "ab"; assert(tree.search(s, strlen(s)) == make_pair(0, -1));
        s = "b"; assert(tree.search(s, strlen(s)) == make_pair(0, -1));
        s = "c"; assert(tree.search(s, strlen(s)) == make_pair(0, -1));

        tree.extendSuffix('a');
        cout << "*** after extend 'a'" << endl;
        dump(tree, &tree.mRoot, -1, 0);
        s = "a"; assert(tree.search(s, strlen(s)) == make_pair(1, -1));
        s = "ab"; assert(tree.search(s, strlen(s)) == make_pair(1, -1));
        s = "b"; assert(tree.search(s, strlen(s)) == make_pair(0, -1));
        s = "c"; assert(tree.search(s, strlen(s)) == make_pair(0, -1));

        tree.extendSuffix('b');
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
        SuffixTree tree;

        const char* treeS = "xabxac";
        {
            tree.clear();

            for (int i = 0; treeS[i]; i++) {
                tree.extendSuffix(treeS[i]);
                cout << "*** character '" << treeS[i] << "' added " << endl;
                dump(tree, &tree.mRoot, -1, 0);
            }

            tree.setSuffixIndex();
            cout << "*** with suffix index *** " << endl;
            dump(tree, &tree.mRoot, -1, 0, true);
        }
    }
    */
}
