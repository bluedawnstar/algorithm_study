#include <vector>

using namespace std;

#include "lcaTarjan.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include <queue>
#include <algorithm>

#include "treeBasic.h"

void testLcaTarjan() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Lca - Tarjan ----------------------------------" << endl;
    {
        int N = 8;
        int T = 10000;

        Tree tree(N);

        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
        }
        tree.build(0);

        vector<vector<pair<int, int>>> queries(N);
        for (int i = 0; i < T; i++) {
            int u = RandInt32::get() % N;
            int v = RandInt32::get() % N;
            if (u < v)
                swap(u, v);
            queries[u].emplace_back(v, 0);  // CAUTION! : v should be visited before u 
        }

        for (int u = 0; u < N; u++) {
            sort(queries[u].begin(), queries[u].end());
            queries[u].erase(unique(queries[u].begin(), queries[u].end()), queries[u].end());
        }

        LcaTarjan tarjan;
        tarjan.lca(tree.edges, 0, queries);

        for (int u = 0; u < N; u++) {
            for (auto& v : queries[u]) {
                int a = v.second;
                int b = tree.findLCA(u, v.first);
                if (a != b)
                    cout << "LCA(" << u << "," << v.first << ") = " << a << ", " << b << endl;
                assert(v.second == tree.findLCA(u, v.first));
            }
        }
    }
    cout << "OK!" << endl;
}
