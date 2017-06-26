#include <cassert>
#include <vector>

using namespace std;

#include "linkCutTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include <algorithm>
#include <numeric>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#include "redBlackTree.h"
#include "treap.h"
#include "splayTree.h"

void checkSearch(RBTree<int>& rbt, vector<int>& in);
void checkIndex(RBTree<int>& rbt, vector<int>& in);

void checkIndex(LinkCutTreeArray<int>& in) {
    for (int i = 0; i < (int)in.nodes.size(); i++) {
        assert(in.indexOf(i) == i);
    }
}

#define MAXN    10000
static LinkCutTreeArray<int> makeLcaTree() {
    LinkCutTreeArray<int> lct(MAXN);

    lct.link(1, 0);
    lct.link(2, 0);

    int i, p = 1;
    for (i = 3; i < (int)lct.nodes.size() / 4; i++) {
        lct.link(i, p);
        p = i;
    }

    p = 1;
    for (; i < (int)lct.nodes.size() * 2 / 4; i++) {
        lct.link(i, p);
        p = i;
    }

    p = 2;
    for (; i < (int)lct.nodes.size() * 3 / 4; i++) {
        lct.link(i, p);
        p = i;
    }

    p = 2;
    for (; i < (int)lct.nodes.size(); i++) {
        lct.link(i, p);
        p = i;
    }

    return lct;
}


void testLinkCutTree() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- Link-Cut Tree ----------------------------------" << endl;
    // LCA test 
    {
        LinkCutTreeArray<int> lct = makeLcaTree();

        int N = (int)lct.nodes.size();

        PROFILE_START(0);
        int errCnt = 0;
        for (int i = 0; i < 100000; i++) {
            int u = rand() % N;
            int v = rand() % N;
            int lca = lct.lca(u, v);
            int lcaAns;
            if (u == 0 || v == 0) {
                lcaAns = 0;
            } else if ((u != 2 && u < N / 2) != (v != 2 && v < N / 2)) {
                lcaAns = 0;
            } else if (u != 2 && u < N / 2) {
                if ((u >= N / 4) != (v >= N / 4))
                    lcaAns = 1;
                else
                    lcaAns = min(u, v);
            } else {
                if ((u >= N * 3 / 4) != (v >= N * 3 / 4))
                    lcaAns = 2;
                else
                    lcaAns = min(u, v);
            }
            if (lca != lcaAns) {
                cout << "mismatch : LCA(" << u << ", " << v << ") = " << lca << " (!= " << lcaAns << ")" << endl;
                errCnt++;
            }
        }
        PROFILE_STOP(0);
        if (!errCnt)
            cout << "OK!" << endl;
    }
    {
        vector<int> in(1000);
        iota(in.begin(), in.end(), 0);

        LinkCutTreeArray<int> lct(1000);
        lct.setValue(0, 0);
        for (int i = 1; i < 1000; i++) {
            lct.setValue(i, i);
            lct.link(i, i - 1);
        }
        checkIndex(lct);

        cout << "OK!" << endl;
    }
}
