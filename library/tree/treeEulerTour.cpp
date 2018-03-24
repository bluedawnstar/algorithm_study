#include <memory.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "treeEulerTour.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testEulerTour() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- Euler Tour Tree -----------------------------" << endl;
    {
        int N = 8;
        EulerTourTree tree(N);
        tree.addEdge(0, 1);
        tree.addEdge(0, 2);
        tree.addEdge(1, 3);
        tree.addEdge(1, 4);
        tree.addEdge(1, 5);
        tree.addEdge(2, 6);
        tree.addEdge(2, 7);
        tree.build(0);

        assert(tree.findLCAFast(5, 6) == 0);
        assert(tree.findLCAFast(5, 3) == 1);
    }
    {
        int N = 1000;
        EulerTourTree tree(N);

        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
        }
        tree.build(0);

        for (int i = 0; i < 1000; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);

            int lca1 = tree.findLCA(L, R);
            int lca2 = tree.findLCAFast(L, R);
            if (lca1 != lca2) {
                cout << "LCA(" << L << ", " << R << ") = " << lca1 << endl;
                cout << "LCA(" << L << ", " << R << ") = " << lca2 << endl;
            }
            assert(lca1 == lca2);
        }
    }
    cout << "* Speed test..." << endl;
    {
        //int TESTN = 10000000;
        int TESTN = 10000;
        int N = 75000;
        EulerTourTree tree(N);

        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
        }
        tree.build(0);

        PROFILE_START(0);
        for (int i = 0; i < TESTN; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);
            int lca1 = tree.findLCA(L, R);
            if (lca1 < 0)
                cerr << "What?" << endl;
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        for (int i = 0; i < TESTN; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);

            int lca2 = tree.findLCAFast(L, R);
            if (lca2 < 0)
                cerr << "What?" << endl;
        }
        PROFILE_STOP(1);
    }

    cout << "OK!" << endl;
}
