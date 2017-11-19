#include <vector>
#include <algorithm>

using namespace std;

#include "intervalTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

static ostream& operator <<(ostream& os, IntervalTree<int,int>::IntervalNode* rhs) {
    os << "(" << rhs->interval.low << ", " << rhs->interval.high << ")";
    return os;
}

static ostream& operator <<(ostream& os, vector<IntervalTree<int,int>::IntervalNode*>& rhs) {
    os << "{ ";
    if (!rhs.empty())
        os << rhs[0];
    for (int i = 1; i < (int)rhs.size(); i++)
        os << ", " << rhs[i];
    os << " }";

    return os;
}

void testIntervalTree() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Interval Tree ----------------------------------------" << endl;

    vector<pair<int,int>> in{ { 15, 20 }, { 10, 30 }, { 17, 19 }, { 5, 20 }, { 12, 15 }, { 30, 40 } };

    IntervalTree<int, int> tree;
    for (int i = 0; i < (int)in.size(); i++) {
        auto* p = tree.insert(in[i].first, in[i].second);
        assert(p->interval.low == in[i].first);
        assert(p->interval.high == in[i].second);
    }

    auto contained = tree.searchContained(9, 30);
    cout << contained << endl;

    vector<pair<int, int>> containedGT{ {10, 30}, {12, 15}, {15, 20}, {17, 19} };
    assert(contained.size() == containedGT.size());
    for (int i = 0; i < (int)contained.size(); i++)
        assert(contained[i]->interval.low == containedGT[i].first && contained[i]->interval.high == containedGT[i].second);

    auto* p = tree.searchOverlap(6, 7);
    assert(p->interval.low == 5);
    assert(p->interval.high == 20);

    tree.erase(10, 30);
    assert(tree.tree->left->interval.low == 12);
    assert(tree.tree->left->interval.high == 15);

    cout << "OK!" << endl;
}
