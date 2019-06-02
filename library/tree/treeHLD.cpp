#include <memory.h>
#include <cassert>
#include <vector>
#include <queue>
#include <stack>
#include <functional>
#include <algorithm>

using namespace std;

#include "../rangeQuery/segmentTreeCompact.h"
#include "../rangeQuery/segmentTreeCompactLazy.h"
#include "../rangeQuery/fenwickTreeMultAdd.h"
#include "treeHLD.h"
#include "treeHLDPathQuery.h"
#include "treeHLDPathQueryLazy.h"
#include "treeHLDPathQueryBIT.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#define MAXN    50000
#define LOGN    17              // log2(MAXN)

// https://algospot.com/judge/problem/read/NAVI
void testHLD_org() {
    return; //TODO: if you want to test, make this line a comment.

    int T ;
    cin >> T;

    while (T-- > 0) {
        HLD tree(MAXN, LOGN);
        auto pathQuery = makeSimpleHLDPathQuery(tree, [](int a, int b) { return max(a, b); }, -1);

        cin >> tree.N;
        for (int v = 0; v < tree.N; v++) {
            int u;
            cin >> u;
            if (u < 0)
                continue;

            tree.addEdge(u, v);
        }

        tree.build(0);
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

void testHLD() {
    //return; //TODO: if you want to test a split function, make this line to a comment.

    cout << "--- Simple Heavy-Light Decomposition -----------------------" << endl;
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
            HLD tree(int(TR[i].size()), LOGN);
            auto pathQuery = makeSimpleHLDPathQuery(tree, [](int a, int b) { return max(a, b); }, -1);
            auto pathQueryLazy = makeSimpleHLDPathQueryLazy(tree, [](int a, int b) { return max(a, b); }, [](int a, int n) { return a; }, -1);

            for (int v = 0; v < tree.N; v++) {
                int u = TR[i][v];
                if (u < 0)
                    continue;

                tree.addEdge(u, v);
            }

            tree.build(0);
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
    {
        int N = 10000;
        int CASE = 100;
        int T = 10000;
#if _DEBUG
        N = 1000;
        CASE = 10;
        T = 1000;
#endif
        while (CASE-- > 0) {
            HLD tree(N);
            for (int v = 1; v < N; v++) {
                int u = RandInt32::get() % v;
                tree.addEdge(u, v);
            }
            tree.build(0);

            for (int i = 0; i < T; i++) {
                int u = RandInt32::get() % N;
                int v = RandInt32::get() % N;
                int lca1 = tree.findLCA(u, v);
                int lca2 = tree.findLCA2(u, v);
                if (lca1 != lca2)
                    cout << "Mismatched LCA : " << lca1 << ", " << lca2 << endl;
                assert(lca1 == lca2);

                int d = RandInt32::get() % (tree.level[u] + 1);
                int ua1 = tree.climbTree(u, d);
                int ua2 = tree.climbTree2(u, d);
                if (ua1 != ua2)
                    cout << "Mismatched value : " << ua1 << ", " << ua2 << endl;
                assert(ua1 == ua2);
            }
        }
    }
    {
        int N = 100000;
        int T = 1000000;
#if _DEBUG
        N = 1000;
        T = 1000;
#endif
        HLD tree(N);
        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
        }
        tree.build(0);

        vector<pair<int, int>> query(T);
        for (int i = 0; i < T; i++) {
            query[i].first = RandInt32::get() % N;
            query[i].second = RandInt32::get() % N;
        }

        PROFILE_START(0);
        long long sum1 = 0;
        {
            auto Q = query;
            for (auto it : Q)
                sum1 += tree.findLCA(it.first, it.second);
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        long long sum2 = 0;
        {
            auto Q = query;
            for (auto it : Q)
                sum2 += tree.findLCA2(it.first, it.second);
        }
        PROFILE_STOP(1);

        if (sum1 != sum2)
            cout << "Mismatched : " << sum1 << ", " << sum2 << endl;
        assert(sum1 == sum2);


        PROFILE_START(2);
        long long sum3 = 0;
        {
            auto Q = query;
            for (auto it : Q)
                sum3 += tree.climbTree(it.first, tree.level[it.first] - 1);
        }
        PROFILE_STOP(2);

        PROFILE_START(3);
        long long sum4 = 0;
        {
            auto Q = query;
            for (auto it : Q)
                sum4 += tree.climbTree2(it.first, tree.level[it.first] - 1);
        }
        PROFILE_STOP(3);

        if (sum3 != sum4)
            cout << "Mismatched : " << sum3 << ", " << sum4 << endl;
        assert(sum3 == sum4);
    }
    cout << "OK!" << endl;
}
