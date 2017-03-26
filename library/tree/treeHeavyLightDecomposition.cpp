#include <memory.h>
#include <vector>
#include <queue>
#include <stack>
#include <functional>
#include <algorithm>

using namespace std;

#include "treeBasic.h"
#include "treeSegmentTree.h"
#include "treeHeavyLightDecomposition.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

#define MAXN    50000
#define LOGN    17              // log2(MAXN)

// https://algospot.com/judge/problem/read/NAVI
void testHeavyLightDecomposition_org() {
    return; //TODO: if you want to test a split function, make this line to a comment.

    //TreeHLD<MAXN, LOGN, int, MaxOp<int>> tree;
    TreeHLD<MAXN, LOGN, int> tree([](int a, int b) { return max(a, b); });

    int T ;
    scanf("%d", &T);

    while (T-- > 0) {
        tree.clear();

        scanf("%d", &tree.mN);
        for (int v = 0; v < tree.mN; v++) {
            int u;
            scanf("%d", &u);
            if (u < 0)
                continue;

            tree.addEdge(u, v);
        }

        tree.dfs(0, -1);
        tree.makeLcaTable();

        tree.doHLD(0);
        tree.initSegTree(1);

        int ans = 0;

        int Q;
        scanf("%d", &Q);
        while (Q-- > 0) {
            char type[10];
            int u, v, cost;

            scanf("%s", type);
            if (!strcmp(type, "update")) {
                scanf("%d %d %d", &u, &v, &cost);
                tree.update(u, v, cost);
            } else {
                scanf("%d %d", &u, &v);
                ans ^= tree.query(u, v);
            }
        }
        printf("%d\n", ans);
    }
}

void testHeavyLightDecomposition() {
    //return; //TODO: if you want to test a split function, make this line to a comment.

    int T = 2;

    vector<vector<int>> TR = {
        { -1, 0, 1, 2, 3 },
        { -1, 0, 1, 2, 2, 4, 1, 6, 6, 7, 0, 10, 10 }
    };

    struct QR {
        string cmd;
        int a, b, cost;
    };
    vector<vector<QR>> QV = {
        { { "update", 1, 2, 7 },
          { "query", 0, 3, },
          { "update", 0, 1, 8 },
          { "query", 0, 3 } },
        { { "query", 0, 10 },
          { "query", 7, 8 },
          { "update", 11, 10, 7 },
          { "update", 0, 10, 4 },
          { "query", 0, 11 } }
    };

    TreeHLD<MAXN, LOGN, int> tree([](int a, int b) { return max(a, b); });

    vector<int> rightAns{ 15, 7 };
    for (int i = 0; i < T; i++) {
        tree.clear();

        //scanf("%d", &gN);
        tree.setVertexCount((int)TR[i].size());
        for (int v = 0; v < tree.mN; v++) {
            int u = TR[i][v];
            if (u < 0)
                continue;

            tree.addEdge(u, v);
        }

        tree.dfs(0, -1);
        tree.makeLcaTable();

        tree.doHLD(0);
        tree.initSegTree(1);

        int ans = 0;

        int Q;
        //scanf("%d", &Q);
        Q = (int)QV[i].size();
        for (int j = 0; j < Q; j++) {
            string type;
            int u, v, cost;

            //scanf("%s", type);
            type = QV[i][j].cmd;
            if (type == "update") {
                //scanf("%d %d %d", &u, &v, &cost);
                u = QV[i][j].a;
                v = QV[i][j].b;
                cost = QV[i][j].cost;
                tree.update(u, v, cost);
            } else {
                //scanf("%d %d", &u, &v);
                u = QV[i][j].a;
                v = QV[i][j].b;
                ans ^= tree.query(u, v);
            }
        }
        printf("%d\n", ans);
        assert(rightAns[i] == ans);
    }
}
