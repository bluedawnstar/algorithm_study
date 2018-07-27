using namespace std;

#include "binarySearchTree.h"

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

static void checkSearch(BST<int>& bst, vector<int>& in) {
    for (int i = 0; i < int(in.size()); i++) {
        int x = in[i];
        assert(bst.find(x)->value == x);
    }
}

static void checkIndex(BST<int>& bst, vector<int>& in) {
    assert(bst.size() == int(in.size()));
    for (int i = 0; i < int(in.size()); i++) {
        assert(bst[i]->value == in[i]);
        assert(bst.indexOf(bst[i]) == i);
    }
}

void testBST() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Binary Search Tree ----------------------------------" << endl;

    {
        BST<int> bst;

        vector<int> in(1000);
        iota(in.begin(), in.end(), 0);

        assert(bst.empty());
        {
            vector<int> t(in);
            random_shuffle(t.begin(), t.end());
            for (int i = 0; i < int(in.size()); i++) {
                auto p = bst.insert(t[i]);
                if (!p.second)
                    cerr << "It'll never be shown!" << endl;
                assert(p.first->value == t[i] && p.second);
            }
            checkSearch(bst, in);
            checkIndex(bst, in);
        }
        {
            assert(bst.lowerBound(-1)->value == 0);
            assert(bst.lowerBound(0)->value == 0);
            assert(bst.lowerBound(3)->value == 3);
            assert(bst.lowerBound(77)->value == 77);
            assert(bst.lowerBound(999)->value == 999);
            assert(bst.upperBound(0)->value == 1);
            assert(bst.upperBound(3)->value == 4);
            assert(bst.upperBound(77)->value == 78);
            assert(bst.upperBound(999) == bst.nullNode());

            auto it = bst.lowerBound(10);
            auto itE = bst.upperBound(77);
            for (int i = 10; i < 77; i++) {
                assert(it->value == i);
                it = bst.next(it);
            }
            for (int i = 76; i >= 10; i--) {
                it = bst.prev(it);
                assert(it->value == i);
            }
        }
        {
            vector<int> t(in), org(in);

            random_shuffle(t.begin(), t.end());
            while (!bst.empty()) {
                int x = t.back();
                t.pop_back();
                org.erase(find(org.begin(), org.end(), x));

                bool b = bst.erase(x);
                if (!b)
                    cerr << "It'll never be shown!" << endl;
                assert(b);
                assert(bst.size() == int(t.size()));
                checkSearch(bst, org);
                checkIndex(bst, org);
            }
        }
    }

    cout << "OK!" << endl;
}
