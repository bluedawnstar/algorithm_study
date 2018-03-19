using namespace std;

#include "redBlackTree.h"

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

void checkSearch(BST<int>& bst, vector<int>& in) {
    for (int i = 0; i < (int)in.size(); i++) {
        int x = in[i];
        assert(bst.find(x)->value == x);
    }
}

void checkIndex(BST<int>& bst, vector<int>& in) {
    assert(bst.size() == (int)in.size());
    for (int i = 0; i < (int)in.size(); i++) {
        assert(bst[i]->value == in[i]);
        assert(bst.indexOf(bst[i]) == i);
    }
}

void checkSearch(RBTree<int>& rbt, vector<int>& in) {
    for (int i = 0; i < (int)in.size(); i++) {
        int x = in[i];
        assert(rbt.find(x)->value.value == x);
    }
}

void checkIndex(RBTree<int>& rbt, vector<int>& in) {
    assert(rbt.size() == (int)in.size());
    for (int i = 0; i < (int)in.size(); i++) {
        assert(rbt[i]->value.value == in[i]);
        assert(rbt.indexOf(rbt[i]) == i);
    }
}

void testBST() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- Binary Search Tree ----------------------------------" << endl;

    {
        BST<int> bst;
        RBTree<int> rbt;

        vector<int> in(1000);
        iota(in.begin(), in.end(), 0);

        assert(bst.empty());
        {
            vector<int> t(in);
            random_shuffle(t.begin(), t.end());
            for (int i = 0; i < (int)in.size(); i++) {
                auto p = bst.insert(t[i]);
                if (!p.second)
                    cerr << "It'll never be shown!" << endl;
                assert(p.first->value == t[i] && p.second);

                auto p2 = rbt.insert(t[i]);
                if (!p2.second)
                    cerr << "It'll never be shown!" << endl;
                assert(p2.first->value == t[i] && p2.second);
            }
            checkSearch(bst, in);
            checkIndex(bst, in);
            checkSearch(rbt, in);
            checkIndex(rbt, in);
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
            assert(rbt.lowerBound(-1)->value == 0);
            assert(rbt.lowerBound(0)->value == 0);
            assert(rbt.lowerBound(3)->value == 3);
            assert(rbt.lowerBound(77)->value == 77);
            assert(rbt.lowerBound(999)->value == 999);
            assert(rbt.upperBound(0)->value == 1);
            assert(rbt.upperBound(3)->value == 4);
            assert(rbt.upperBound(77)->value == 78);
            assert(rbt.upperBound(999) == rbt.nullNode());

            auto it = rbt.lowerBound(10);
            auto itE = rbt.upperBound(77);
            for (int i = 10; i < 77; i++) {
                assert(it->value == i);
                it = rbt.next(it);
            }
            for (int i = 76; i >= 10; i--) {
                it = rbt.prev(it);
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
                assert(bst.size() == (int)t.size());
                checkSearch(bst, org);
                checkIndex(bst, org);

                bool b2 = rbt.erase(x);
                if (!b2)
                    cerr << "It'll never be shown!" << endl;
                assert(b2);
                assert(rbt.size() == (int)t.size());
                checkSearch(rbt, org);
                checkIndex(rbt, org);
            }
        }
    }
    cout << "OK!" << endl;

    cout << "*** Speed test BST vs RBTree ***" << endl;
    {
        BST<int> bst;
        RBTree<int> rbt;
        set<int> M;

#ifdef _DEBUG
        vector<int> in(300000);
#else
        vector<int> in(3000000);
#endif
        iota(in.begin(), in.end(), 0);

        cout << "1) Insertion test" << endl;
        {
            vector<int> t(in);
            random_shuffle(t.begin(), t.end());

            PROFILE_START(0);
            for (int i = 0; i < (int)t.size(); i++) {
                auto p = bst.insert(t[i]);
                if (!p.second)
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(0);

            PROFILE_START(1);
            for (int i = 0; i < (int)t.size(); i++) {
                auto p = rbt.insert(t[i]);
                if (!p.second)
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(1);

            PROFILE_START(2);
            for (int i = 0; i < (int)t.size(); i++) {
                auto p = M.insert(t[i]);
                if (!p.second)
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(2);
        }
        cout << "2) Find test" << endl;
        {
            vector<int> t(in);
            random_shuffle(t.begin(), t.end());

            PROFILE_START(0);
            for (int i = 0; i < (int)t.size(); i++) {
                if (bst.find(t[i])->value != t[i])
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(0);

            PROFILE_START(1);
            for (int i = 0; i < (int)t.size(); i++) {
                if (rbt.find(t[i])->value.value != t[i])
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(1);

            PROFILE_START(2);
            for (int i = 0; i < (int)t.size(); i++) {
                if (M.find(t[i]) == M.end())
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(2);
        }
        cout << "3) Erase test" << endl;
        {
            vector<int> t(in);
            random_shuffle(t.begin(), t.end());

            PROFILE_START(0);
            for (int i = 0; i < (int)t.size(); i++) {
                if (!bst.erase(t[i]))
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(0);

            PROFILE_START(1);
            for (int i = 0; i < (int)t.size(); i++) {
                if (!rbt.erase(t[i]))
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(1);

            PROFILE_START(2);
            for (int i = 0; i < (int)t.size(); i++) {
                M.erase(t[i]);
            }
            PROFILE_STOP(2);
        }
    }

    cout << "OK!" << endl;
}
