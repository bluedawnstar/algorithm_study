#include <cmath>
#include <tuple>
#include <vector>
#include <queue>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "longestCommonSubsequence.h"
#include "longestCommonSubsequenceDAG.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

static ostream& operator <<(ostream& os, const unordered_set<string>& rhs) {
    for (auto it : rhs)
        os << it << ", ";
    os << endl;
    return os;
}

void testLongestCommonSubsequence() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Longest Common Subsequence ------------------------" << endl;
    {
        assert(lengthOfLcs("aggtab", "gxtxayb") == 4);
        assert(lcs("aggtab", "gxtxayb") == "gtab");


        {
            string s1 = "aggtab";
            string s2 = "gxtxayb";
            auto cnt = countOfAllLcs(s1, s2);
            int gt = 2;
            cout << "countOfLcs(\"" << s1 << "\", \"" << s2 << "\") : " << cnt << endl;
            assert(cnt == gt);

            auto lcs = LcsFinder::allLcs(s1, s2);
            cout << "allLcs(\"" << s1 << "\", \"" << s2 << "\") : " << lcs.size() << endl;
            cout << lcs << endl;
            assert(int(lcs.size()) == gt);

            auto distinctLcs = LcsFinder::allDistinctLcs(s1, s2);
            cout << "allDistinctLcs(\"" << s1 << "\", \"" << s2 << "\") : " << distinctLcs.size() << endl;
            assert(int(distinctLcs.size()) == 1 && distinctLcs.begin()->second == gt);

            LcsDAG dag;
            dag.makeLcsDAG(s1, s2);
            lcs = dag.allLcs();
            cout << "LcsDAG::allLcs(\"" << s1 << "\", \"" << s2 << "\") : " << lcs.size() << endl;
            cout << lcs << endl;
            assert(int(lcs.size()) == gt);
            distinctLcs = dag.allDistinctLcs();
            cout << "LcsDAG::allDistinctLcs(\"" << s1 << "\", \"" << s2 << "\") : " << distinctLcs.size() << endl;
            assert(int(distinctLcs.size()) == 1 && distinctLcs.begin()->second == gt);
        }
        {
            string s1 = "aaaaaa";
            string s2 = "aaa";
            int cnt = countOfAllLcs(s1, s2);
            int gt = 20;
            cout << "countOfLcs(\"" << s1 << "\", \"" << s2 << "\") : " << cnt << endl;
            assert(cnt == gt);

            auto lcs = LcsFinder::allLcs(s1, s2);
            cout << "allLcs(\"" << s1 << "\", \"" << s2 << "\") : " << lcs.size() << endl;
            cout << lcs << endl;
            assert(int(lcs.size()) == gt);

            auto distinctLcs = LcsFinder::allDistinctLcs(s1, s2);
            cout << "allDistinctLcs(\"" << s1 << "\", \"" << s2 << "\") : " << distinctLcs.size() << endl;
            assert(int(distinctLcs.size()) == 1 && distinctLcs.begin()->second == gt);

            LcsDAG dag;
            dag.makeLcsDAG(s1, s2);
            lcs = dag.allLcs();
            cout << "LcsDAG::allLcs(\"" << s1 << "\", \"" << s2 << "\") : " << lcs.size() << endl;
            cout << lcs << endl;
            assert(int(lcs.size()) == gt);
            distinctLcs = dag.allDistinctLcs();
            cout << "LcsDAG::allDistinctLcs(\"" << s1 << "\", \"" << s2 << "\") : " << distinctLcs.size() << endl;
            assert(int(distinctLcs.size()) == 1 && distinctLcs.begin()->second == gt);
        }
        {
            string s1 = "abcabc";
            string s2 = "abc";
            auto cnt = countOfAllLcs(s1, s2);
            int gt = 4;
            cout << "countOfLcs(\"" << s1 << "\", \"" << s2 << "\") : " << cnt << endl;
            assert(cnt == gt);

            auto lcs = LcsFinder::allLcs(s1, s2);
            cout << "allLcs(\"" << s1 << "\", \"" << s2 << "\") : " << lcs.size() << endl;
            cout << lcs << endl;
            assert(int(lcs.size()) == gt);

            auto distinctLcs = LcsFinder::allDistinctLcs(s1, s2);
            cout << "allDistinctLcs(\"" << s1 << "\", \"" << s2 << "\") : " << distinctLcs.size() << endl;
            assert(int(distinctLcs.size()) == 1 && distinctLcs.begin()->second == gt);

            LcsDAG dag;
            dag.makeLcsDAG(s1, s2);
            lcs = dag.allLcs();
            cout << "LcsDAG::allLcs(\"" << s1 << "\", \"" << s2 << "\") : " << lcs.size() << endl;
            cout << lcs << endl;
            assert(int(lcs.size()) == gt);
            distinctLcs = dag.allDistinctLcs();
            cout << "LcsDAG::allDistinctLcs(\"" << s1 << "\", \"" << s2 << "\") : " << distinctLcs.size() << endl;
            assert(int(distinctLcs.size()) == 1 && distinctLcs.begin()->second == gt);
        }
    }

    cout << "OK!" << endl;
}
