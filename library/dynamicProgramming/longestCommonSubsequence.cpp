#include <cmath>
#include <tuple>
#include <string>
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

static void test(const string& s1, const string& s2) {
    auto cnt = countOfAllLcs(s1, s2);
    cout << "countOfLcs(\"" << s1 << "\", \"" << s2 << "\") : " << cnt << endl;

    auto lcs = LcsFinder::allLcs(s1, s2);
    cout << "allLcs(\"" << s1 << "\", \"" << s2 << "\") : " << lcs.size() << endl;
    cout << lcs << endl;
    assert(int(lcs.size()) == cnt);

    auto distinctLcs = LcsFinder::allDistinctLcs(s1, s2);
    cout << "allDistinctLcs(\"" << s1 << "\", \"" << s2 << "\") : " << distinctLcs.size() << endl;
    assert(int(distinctLcs.size()) == 1 && distinctLcs.begin()->second == cnt);

    LcsDAG dag;
    dag.build(s1, s2);
    lcs = dag.findAll();
    cout << "LcsDAG::allLcs(\"" << s1 << "\", \"" << s2 << "\") : " << lcs.size() << endl;
    cout << lcs << endl;
    assert(dag.getTotalCount() == cnt);
    assert(int(lcs.size()) == cnt);

    distinctLcs = dag.findAllDistinct();
    cout << "LcsDAG::allDistinctLcs(\"" << s1 << "\", \"" << s2 << "\") : " << distinctLcs.size() << endl;
    assert(int(distinctLcs.size()) == 1 && distinctLcs.begin()->second == cnt);
}

void testLongestCommonSubsequence() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Longest Common Subsequence ------------------------" << endl;
    {
        assert(lengthOfLcs("aggtab", "gxtxayb") == 4);
        assert(lcs("aggtab", "gxtxayb") == "gtab");

        test("aggtab", "gxtxayb");
        test("aaaaaa", "aaa");
        test("abcabc", "abc");
    }

    cout << "OK!" << endl;
}
