#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "subsequenceDAG.h"
#include "subsequenceDAG_Algo.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#include "subsequenceCount.h"

void testSubsequenceDAG() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Subsequence DAG ------------------------" << endl;
    {
        string s = "abcab";

        SubsequenceDAG sdag(s);
        int cnt = 0;
        sdag.iterateAll([&cnt](int node, const string& s) {
            //cout << s << endl;
            cnt++;
        });

        assert(cnt == countDistinctSubsequence(s));

        //--- algorithm test

        vector<string> allSubsequences{
            "",
            "a",
            "aa",
            "aab",
            "ab",
            "aba",
            "abab",
            "abb",
            "abc",
            "abca",
            "abcab",
            "abcb",
            "ac",
            "aca",
            "acab",
            "acb",
            "b",
            "ba",
            "bab",
            "bb",
            "bc",
            "bca",
            "bcab",
            "bcb",
            "c",
            "ca",
            "cab",
            "cb"
        };

        SubsequenceDAGAlgo algo(sdag, int(s.length()));
        // 1. count all distinct subsequences
        auto cnt2 = algo.countSubsequences();
        assert(cnt == cnt2);

        // 2. total length of all distinct subsequences
        int totLength = 0;
        for (auto& s : allSubsequences)
            totLength += int(s.length());
        assert(algo.totalLengthOfAllDistinctSubsequences() == totLength);

        // 3. lexicographically kth subsequence
        for (int i = 1; i < int(allSubsequences.size()); i++)
            assert(algo.kthSubsequence(i - 1) == allSubsequences[i]);

        // 4. fast finding lexicographically kth subsequence
        algo.makeCountTable();
        for (int i = 1; i < int(allSubsequences.size()); i++)
            assert(algo.kthSubsequenceFast(i - 1) == allSubsequences[i]);
    }

    cout << "OK!" << endl;
}
