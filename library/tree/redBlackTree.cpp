#include <cassert>
#include <functional>
#include <algorithm>

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

static void checkSearch(RBTree<int>& rbt, vector<int>& in) {
    for (int i = 0; i < int(in.size()); i++) {
        int x = in[i];
        assert(rbt.find(x)->value == x);
    }
}

static void checkIndex(RBTree<int>& rbt, vector<int>& in) {
    assert(rbt.size() == int(in.size()));
    for (int i = 0; i < int(in.size()); i++) {
        assert(rbt[i]->value == in[i]);
        assert(rbt.indexOf(rbt[i]) == i);
    }
}

void testRedBlackTree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Red-Black Tree --------------------------------------" << endl;

    {
        RBTree<int> rbt;

        vector<int> in(1000);
        iota(in.begin(), in.end(), 0);

        {
            vector<int> t(in);
            random_shuffle(t.begin(), t.end());
            for (int i = 0; i < int(in.size()); i++) {
                auto p = rbt.insert(t[i]);
                if (!p.second)
                    cerr << "It'll never be shown!" << endl;
                assert(p.first->value == t[i] && p.second);
            }
            checkSearch(rbt, in);
            checkIndex(rbt, in);
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
            while (!rbt.empty()) {
                int x = t.back();
                t.pop_back();
                org.erase(find(org.begin(), org.end(), x));

                bool b = rbt.erase(x);
                if (!b)
                    cerr << "It'll never be shown!" << endl;
                assert(b);
                assert(rbt.size() == int(t.size()));
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
            for (int i = 0; i < int(t.size()); i++) {
                auto p = bst.insert(t[i]);
                if (!p.second)
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(0);


            PROFILE_START(1);
            for (int i = 0; i < int(t.size()); i++) {
                auto p = rbt.insert(t[i]);
                if (!p.second)
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(1);

            PROFILE_START(2);
            for (int i = 0; i < int(t.size()); i++) {
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
            for (int i = 0; i < int(t.size()); i++) {
                if (bst.find(t[i])->value != t[i])
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(0);

            PROFILE_START(1);
            for (int i = 0; i < int(t.size()); i++) {
                if (rbt.find(t[i])->value != t[i])
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(1);

            PROFILE_START(2);
            for (int i = 0; i < int(t.size()); i++) {
                if (M.find(t[i]) == M.end())
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(2);
        }
        cout << "3) Iteration test" << endl;
        {
            int sum1 = 0;
            PROFILE_START(0);
            for (auto it = bst.leftmost(); it != bst.nullNode(); it = bst.next(it)) {
                sum1 += it->value;
            }
            PROFILE_STOP(0);
            if (!sum1)
                cerr << "dummy message" << endl;

            int sum2 = 0;
            PROFILE_START(1);
            for (auto it = rbt.leftmost(); it != rbt.nullNode(); it = rbt.next(it)) {
                sum2 += it->value;
            }
            PROFILE_STOP(1);
            if (!sum2)
                cerr << "dummy message" << endl;

            int sum3 = 0;
            PROFILE_START(2);
            for (auto it = M.begin(); it != M.end(); ++it) {
                sum3 += *it;
            }
            PROFILE_STOP(2);
            if (!sum3)
                cerr << "dummy message" << endl;

            if (sum1 != sum2 || sum1 != sum3)
                cerr << "Mismatched!" << endl;
        }
        cout << "4) Erase test" << endl;
        {
            vector<int> t(in);
            random_shuffle(t.begin(), t.end());

            PROFILE_START(0);
            for (int i = 0; i < int(t.size()); i++) {
                if (!bst.erase(t[i]))
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(0);

            PROFILE_START(1);
            for (int i = 0; i < int(t.size()); i++) {
                if (!rbt.erase(t[i]))
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(1);

            PROFILE_START(2);
            for (int i = 0; i < int(t.size()); i++) {
                M.erase(t[i]);
            }
            PROFILE_STOP(2);
        }
    }

    cout << "OK!" << endl;
}
