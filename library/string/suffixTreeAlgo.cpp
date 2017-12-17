#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

#include "suffixTreeAlgo.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include <vector>
#include "../common/iostreamhelper.h"

void testSuffixTreeAlgo() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Suffix Tree Algorithm -------------------------------" << endl;
    {
        SuffixTree tree(100);
        //                   01234567
        const char* treeS = "abcabcab$";
        int N = strlen(treeS);
        tree.build(treeS, strlen(treeS));

        SuffixTreeAlgo stAlgo(tree);
        for (int i = 0; i < N; i++) {
            const SuffixTree::Node* p = stAlgo.suffixes[i];
            auto ans = tree.getString(p);
            auto gt = string(treeS + i);
            if (ans != gt) {
                cerr << "Mismatched at " << i << ": " << ans << " != " << gt << endl;
            }
            assert(tree.getString(p) == string(treeS + i));
        }

        assert(stAlgo.lcp(0, 3) == string("abcab"));
        assert(stAlgo.lcp(1, 7) == string("b"));
        assert(stAlgo.isSubstring("cab") == true);
        assert(stAlgo.isSubstring("cac") == false);

        auto cntSS = stAlgo.search("ab");
        assert(cntSS.second == 3);

        cout << stAlgo.findLongestRepeatedSubstring() << endl;
        assert(stAlgo.findLongestRepeatedSubstring() == "abcab");

        vector<int> gtSA{ 0, 3, 6, 1, 4, 7, 2, 5, 8 };
        auto ansSA = stAlgo.buildSuffixArray();
        assert(stAlgo.buildSuffixArray() == gtSA);
    }

    cout << "OK!" << endl;
}
