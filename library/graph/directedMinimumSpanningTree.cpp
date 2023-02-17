#include <tuple>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#include "directedMinimumSpanningTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

// <Related Problems>
// 1. https://www.codechef.com/problems/BASH
//    -> https://discuss.codechef.com/t/bash-editorial/85352

// https://judge.yosupo.jp/problem/directedmst

void testDirectedMST() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Directed Minimum Spanning Tree ---------" << endl;
    {
        // test case #1 from https://judge.yosupo.jp/problem/directedmst
        DirectedMST<int> dmst(4);
        dmst.addEdge(0, 1, 10);
        dmst.addEdge(0, 2, 10);
        dmst.addEdge(0, 3, 3);
        dmst.addEdge(3, 2, 4);

        auto ans = dmst.mst(0);
        vector<int> gt{ -1, 0, 3, 0 };
        if (ans.first != 17)
            cout << "Mismatched at " << __LINE__ << " : " << ans.first << ", 17" << endl;
        assert(ans.first == 17);
        if (ans.second != gt)
            cout << "Mismatched at " << __LINE__ << " : " << ans.second << ", " << gt << endl;
        assert(ans.second == gt);
    }
    {
        // test case #2 from https://judge.yosupo.jp/problem/directedmst
        DirectedMST<int> dmst(7);
        dmst.addEdge(3, 1, 10);
        dmst.addEdge(1, 2, 1);
        dmst.addEdge(2, 0, 1);
        dmst.addEdge(0, 1, 1);
        dmst.addEdge(2, 6, 10);
        dmst.addEdge(6, 4, 1);
        dmst.addEdge(4, 5, 1);
        dmst.addEdge(5, 6, 1);

        auto ans = dmst.mst(3);
        vector<int> gt{ 2, 3, 1, -1, 6, 4, 2 };
        if (ans.first != 24)
            cout << "Mismatched at " << __LINE__ << " : " << ans.first << ", 17" << endl;
        assert(ans.first == 24);
        if (ans.second != gt)
            cout << "Mismatched at " << __LINE__ << " : " << ans.second << ", " << gt << endl;
        assert(ans.second == gt);
    }

    cout << "OK" << endl;
}
