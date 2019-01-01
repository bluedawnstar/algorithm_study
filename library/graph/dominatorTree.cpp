#include <climits>
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "dominatorTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testDominatorTree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Dominator Tree ---------" << endl;
    {
        enum { A, B, C, D, E, F, G, H, R, MAX };
        char* symbol[] = { "-1", "A", "B", "C", "D", "E", "F", "G", "H", "R" };

        DominatorTree graph(MAX);
        graph.add(R, A); graph.add(R, B);
        graph.add(A, C);
        graph.add(B, C);
        graph.add(C, D); graph.add(C, E);
        graph.add(D, F);
        graph.add(E, G);
        graph.add(F, D); graph.add(F, H);
        graph.add(G, H);

        graph.build(R);

        for (int u = 0; u < MAX; u++)
            cout << "idom[" << symbol[1 + u] << "] = " << symbol[1 + graph.treeParent[u]] << endl;

        vector<int> idomGt{ R, R, R, C, C, D, E, C, -1 };
        assert(graph.treeParent == idomGt);

        vector<vector<int>> treeGt{
            { R },                          // A
            { R },                          // B
            { D, E, H, R },                 // C
            { C, F },                       // D
            { C, G },                       // E
            { D },                          // F
            { E },                          // G
            { C },                          // H
            { A, B, C },                    // R
        };
        for (int u = A; u < MAX; u++) {
            sort(graph.tree[u].begin(), graph.tree[u].end());
            assert(graph.tree[u] == treeGt[u]);
        }
    }
    cout << "OK!" << endl;
    {
        // FIGURE - 1 in https://tanujkhattar.wordpress.com/2016/01/11/dominator-tree-of-a-directed-graph/
        enum { A, B, C, D, E, F, G, H, I, J, K, L, R, MAX };
        char* symbol[] = { "-1", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "R" };

        DominatorTree graph(MAX);
        graph.add(R, A); graph.add(R, B); graph.add(R, C);
        graph.add(A, D);
        graph.add(B, A); graph.add(B, D); graph.add(B, E);
        graph.add(C, F); graph.add(C, G);
        graph.add(D, L);
        graph.add(E, H);
        graph.add(F, I);
        graph.add(G, I); graph.add(G, J);
        graph.add(H, E); graph.add(H, K);
        graph.add(I, K);
        graph.add(J, I);
        graph.add(K, I); graph.add(K, R);
        graph.add(L, H);

        graph.build(R);

        for (int u = 0; u < MAX; u++)
            cout << "idom[" << symbol[1 + u] << "] = " << symbol[1 + graph.treeParent[u]] << endl;

        vector<int> idomGt{ R, R, R, R, R, C, C, R, R, G, R, D, -1 };
        assert(graph.treeParent == idomGt);

        vector<vector<int>> treeGt{
            { R },                          // A
            { R },                          // B
            { F, G, R },                    // C
            { L, R },                       // D
            { R },                          // E
            { C },                          // F
            { C, J },                       // G
            { R },                          // H
            { R },                          // I
            { G },                          // J
            { R },                          // K
            { D },                          // L
            { A, B, C, D, E, H, I, K },     // R
        };
        for (int u = A; u < MAX; u++) {
            sort(graph.tree[u].begin(), graph.tree[u].end());
            assert(graph.tree[u] == treeGt[u]);
        }
    }
    cout << "OK!" << endl;
}
