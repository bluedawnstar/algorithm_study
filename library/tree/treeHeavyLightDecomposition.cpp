#include <memory.h>
#include <cassert>
#include <vector>
#include <queue>
#include <stack>
#include <functional>
#include <algorithm>

using namespace std;

#include "treeBasic.h"
#include "../rangeQuery/segmentTreeCompact.h"
#include "../rangeQuery/segmentTreeCompactLazy.h"
#include "../rangeQuery/fenwickTreeMultAdd.h"
#include "treeHeavyLightDecomposition.h"
#include "treeHeavyLightDecompositionPathQuery.h"
#include "treeHeavyLightDecompositionPathQueryLazy.h"
#include "treeHeavyLightDecompositionPathQueryBIT.h"

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
    return; //TODO: if you want to test, make this line a comment.

    int T ;
    cin >> T;

    while (T-- > 0) {
        Tree tree(MAXN, LOGN);
        HeavyLightDecomposition hld(tree);
        auto pathQuery = makeHLDPathQuery(hld, [](int a, int b) { return max(a, b); }, -1);

        cin >> tree.N;
        for (int v = 0; v < tree.N; v++) {
            int u;
            cin >> u;
            if (u < 0)
                continue;

            tree.addEdge(u, v);
        }

        tree.dfs(0, -1);
        tree.makeLcaTable();

        hld.doHLD(0);
        pathQuery.build(1);

        int ans = 0;

        int Q;
        cin >> Q;
        while (Q-- > 0) {
            char type[10];
            int u, v, cost;

            cin >> type;
            if (!strcmp(type, "update")) {
                cin >> u >> v >> cost;
                pathQuery.update(u, v, cost);
            } else {
                cin >> u >> v;
                ans ^= pathQuery.query(u, v);
            }
        }
        cout << ans << endl;
    }
}

void testHeavyLightDecomposition() {
    return; //TODO: if you want to test a split function, make this line to a comment.

    cout << "--- Heavy-Light Decomposition -----------------------" << endl;
    {
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

        vector<int> rightAns{ 15, 7 };
        for (int i = 0; i < T; i++) {
            Tree tree(int(TR[i].size()), LOGN);
            HeavyLightDecomposition hld(tree);
            auto pathQuery = makeHLDPathQuery(hld, [](int a, int b) { return max(a, b); }, -1);
            auto pathQueryLazy = makeHLDPathQueryLazy(hld, [](int a, int b) { return max(a, b); }, [](int a, int n) { return a; }, -1);

            for (int v = 0; v < tree.N; v++) {
                int u = TR[i][v];
                if (u < 0)
                    continue;

                tree.addEdge(u, v);
            }

            tree.dfs(0, -1);
            tree.makeLcaTable();

            hld.doHLD(0);
            pathQuery.build(1);
            pathQueryLazy.build(1);

            int ans = 0;
            int ansLazy = 0;

            int Q;

            Q = int(QV[i].size());
            for (int j = 0; j < Q; j++) {
                string type;
                int u, v, cost;

                type = QV[i][j].cmd;
                if (type == "update") {
                    u = QV[i][j].a;
                    v = QV[i][j].b;
                    cost = QV[i][j].cost;
                    pathQuery.update(u, v, cost);
                    pathQueryLazy.update(u, v, cost);
                } else {
                    u = QV[i][j].a;
                    v = QV[i][j].b;
                    ans ^= pathQuery.query(u, v);
                    ansLazy ^= pathQueryLazy.query(u, v);
                }
            }
            cout << ans << ", " << ansLazy << endl;
            assert(rightAns[i] == ans);
            assert(rightAns[i] == ansLazy);
        }
    }
    cout << "OK!" << endl;
}
