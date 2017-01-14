#include <vector>
#include <string>
#include <stack>
#include <memory>
#include <string>
#include <algorithm>

using namespace std;

#include "suffixTree.h"

long long countSubstrings(SuffixTree::Node* node) {
    if (!node)
        return 0;

    long long count = node->edgeLength();
    for (auto p : node->children) {
        if (p)
            count += countSubstrings(p);
    }

    return count;
}

long long countSubstrings(SuffixTree& tree) {
    return countSubstrings(tree.root) - 1;
}


/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

template <typename T>
vector<int> makeSuffixArrayFast(T s, int n);

template <typename T>
long long countSubstringsFast(const vector<int>& suffixArray, T s, int n);

void testSuffixTree() {
    return; //TODO: if you want to test string functions, make this line to a comment.

    cout << "-- makeSuffixTree() -------------" << endl;

    string S("abdadafaaabdfaeef");

    long long cnt1, cnt2;
    {
        vector<int> a2 = makeSuffixArrayFast(S, (int)S.length());
        cnt1 = countSubstringsFast(a2, S, (int)S.length());
    }
    {
        SuffixTree tree;
        tree.build(&S[0], (int)S.length());
        cnt2 = countSubstrings(tree);
    }

    assert(cnt1 == cnt2);

    cout << "OK! : countSubstrings(\"" << S << "\") = " << cnt2 << endl;
}
