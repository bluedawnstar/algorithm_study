#include <math.h>
#include <cassert>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "scapegoatTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <numeric>
#include <set>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static void checkSearch(ScapegoatTree<int>& tree, vector<int>& in) {
    for (int i = 0; i < (int)in.size(); i++) {
        int x = in[i];
        assert(tree.find(x)->value == x);
    }
}

static void checkIndex(ScapegoatTree<int>& tree, vector<int>& in) {
    assert(tree.size() == (int)in.size());
    for (int i = 0; i < (int)in.size(); i++) {
        assert(tree[i]->value == in[i]);
        assert(tree.indexOf(tree[i]) == i);
    }
}

void testScapegoatTree() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- Scalegoat Tree -------------------------" << endl;

    {
        ScapegoatTree<int> tree;

        vector<int> in(1000);
        iota(in.begin(), in.end(), 0);

        assert(tree.empty());
        {
            vector<int> t(in);
            random_shuffle(t.begin(), t.end());
            for (int i = 0; i < (int)in.size(); i++) {
                auto p = tree.insert(t[i]);
                if (!p.second)
                    cerr << "It'll never be shown!" << endl;
                assert(p.first->value == t[i] && p.second);
            }
            checkSearch(tree, in);
            checkIndex(tree, in);
        }
        {
            assert(tree.lowerBound(-1)->value == 0);
            assert(tree.lowerBound(0)->value == 0);
            assert(tree.lowerBound(3)->value == 3);
            assert(tree.lowerBound(77)->value == 77);
            assert(tree.lowerBound(999)->value == 999);
            assert(tree.upperBound(0)->value == 1);
            assert(tree.upperBound(3)->value == 4);
            assert(tree.upperBound(77)->value == 78);
            assert(tree.upperBound(999) == tree.nullNode());

            auto it = tree.lowerBound(10);
            auto itE = tree.upperBound(77);
            for (int i = 10; i < 77; i++) {
                assert(it->value == i);
                it = tree.next(it);
            }
            for (int i = 76; i >= 10; i--) {
                it = tree.prev(it);
                assert(it->value == i);
            }
        }
        {
            vector<int> t(in), org(in);

            random_shuffle(t.begin(), t.end());
            while (!tree.empty()) {
                int x = t.back();
                t.pop_back();
                org.erase(find(org.begin(), org.end(), x));

                bool b = tree.erase(x);
                if (!b)
                    cerr << "It'll never be shown!" << endl;

                assert(b);
                assert(tree.size() == (int)t.size());
                checkSearch(tree, org);
                checkIndex(tree, org);
            }
        }
    }

    cout << "OK!" << endl;
}
