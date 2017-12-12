#include <vector>
#include <queue>
#include <stack>
#include <tuple>
#include <functional>
#include <algorithm>

using namespace std;

#include "treeMO.h"

///////////////////////////////////////////////////////////////////////////////

#include <iostream>

#define N       10000           // TODO: modify the maximum number of nodes
#define LOGN    15              // TODO: modify LCA table size (log2(MAXN))

// template codes for MO's algorithm on a tree
void testTreeMO() {
    return; //TODO: if you want to test a split function, make this line a comment.

    //TODO: step1 - make a tree
    DfsTourTree tree(N, LOGN);
    //TODO: add edges to tree
    tree.build(0);

    vector<pair<int, int>> Q; // (L, R), inclusive
    //TODO: step2 - read all queries (L's visit time must be less than R's visit time)
    /*
    Q.reserve(T);
    for (int i = 0; i < T; i++) {
        int L, R;
        scanf("%d %d", &L, &R);
        L--;
        R--;

        if (tree.mVisTime[L].first > tree.mVisTime[R].first)
            swap(L, R);
        Q.push_back(make_pair(L, R));
    }
    */

    int curAns = 0;
    TreeMO treeMO(tree);
    treeMO.build(Q,
        [&treeMO, &curAns](int t, int u) {
            //TODO: implement adding function
            //curAns += ...;
        },
        [&treeMO, &curAns](int t, int u) {
            //TODO: implement removing function
            //curAns -= ...;
        }
    );

    int currL = get<0>(treeMO.MO[0]);
    int currR = get<1>(treeMO.MO[0]);
    for (int i = currL; i <= currR; i++)
        treeMO.add(i);

    vector<int> ans(Q.size());
    for (int i = 0; i < (int)Q.size(); i++) {
        int nextL = get<0>(treeMO.MO[i]);
        int nextR = get<1>(treeMO.MO[i]);
        int qi = get<2>(treeMO.MO[i]);

        while (currL < nextL)
            treeMO.remove(currL++);
        while (currL > nextL)
            treeMO.add(--currL);
        while (currR < nextR)
            treeMO.add(++currR);
        while (currR > nextR)
            treeMO.remove(currR--);

        int addLca = 0;
        if (treeMO.lca[qi] >= 0) {
            int u = treeMO.lca[qi];

            //TODO: implement adding function about LCA(L, R)
            //      Any global variables mustn't be affected by this codes.
            //addLca = ...;
        }

        ans[qi] = curAns + addLca;
    }

    for (int i = 0; i < (int)Q.size(); i++) {
        cout << ans[i] << endl;
    }
}
