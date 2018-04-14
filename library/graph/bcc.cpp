#include <climits>
#include <numeric>
#include <queue>
#include <algorithm>
#include <vector>

using namespace std;

// Undirected Graph

#include "bcc.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

namespace BasicDigraphTest {
    vector<vector<int>> edges;

    void link(vector<vector<int>>& E, int u, int v) {
        E[u].push_back(v); E[v].push_back(u);
    }
}

using namespace BasicDigraphTest;

void testBCC() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Biconnected Components ---------" << endl;

    BiconnectedComponents bcc;

    edges = vector<vector<int>>(5, vector<int>());
    link(edges, 0, 1);
    link(edges, 0, 2);
    link(edges, 1, 2);
    link(edges, 0, 3);
    link(edges, 0, 4);
    link(edges, 3, 4);
    bcc.findBCC(edges, (int)edges.size());

    assert((int)bcc.comps.size() == 2);
    for (int i = 0; i < (int)bcc.comps.size(); i++)
        sort(bcc.comps[i].begin(), bcc.comps[i].end());
    assert(bcc.comps[0] == vector<int>({0, 1, 2}));
    assert(bcc.comps[1] == vector<int>({0, 3, 4}));

    //------------------------------------------------

    edges = vector<vector<int>>(3, vector<int>());
    link(edges, 0, 1);
    link(edges, 0, 2);
    bcc.findBCC(edges, (int)edges.size());

    assert((int)bcc.comps.size() == 2);
    for (int i = 0; i < (int)bcc.comps.size(); i++)
        sort(bcc.comps[i].begin(), bcc.comps[i].end());
    assert(bcc.comps[0] == vector<int>({ 0, 1 }));
    assert(bcc.comps[1] == vector<int>({ 0, 2 }));

    //------------------------------------------------
    
    edges = vector<vector<int>>(6, vector<int>());
    link(edges, 0, 1);
    link(edges, 1, 2);
    link(edges, 1, 3);
    link(edges, 2, 3);
    link(edges, 2, 4);
    link(edges, 3, 5);
    bcc.findBCC(edges, (int)edges.size());

    assert((int)bcc.comps.size() == 4);
    for (int i = 0; i < (int)bcc.comps.size(); i++)
        sort(bcc.comps[i].begin(), bcc.comps[i].end());
    assert(bcc.comps[0] == vector<int>({ 3, 5 }));
    assert(bcc.comps[1] == vector<int>({ 2, 4 }));
    assert(bcc.comps[2] == vector<int>({ 1, 2, 3 }));
    assert(bcc.comps[3] == vector<int>({ 0, 1 }));

    //------------------------------------------------

    edges = vector<vector<int>>(7, vector<int>());
    link(edges, 0, 1);
    link(edges, 0, 2);
    link(edges, 1, 2);
    link(edges, 2, 3);
    link(edges, 3, 4);
    link(edges, 3, 5);
    link(edges, 4, 5);
    link(edges, 5, 6);
    bcc.findBCC(edges, (int)edges.size());

    assert((int)bcc.comps.size() == 4);
    for (int i = 0; i < (int)bcc.comps.size(); i++)
        sort(bcc.comps[i].begin(), bcc.comps[i].end());
    assert(bcc.comps[0] == vector<int>({ 5, 6 }));
    assert(bcc.comps[1] == vector<int>({ 3, 4, 5 }));
    assert(bcc.comps[2] == vector<int>({ 2, 3 }));
    assert(bcc.comps[3] == vector<int>({ 0, 1, 2 }));

    cout << "OK" << endl;
}
