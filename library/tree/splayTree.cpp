using namespace std;

// http://cubelover.tistory.com/10

#include "splayTree.h"

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

#include "redBlackTree.h"

void checkSearch(RBTree<int>& rbt, vector<int>& in);
void checkIndex(RBTree<int>& rbt, vector<int>& in);

void checkSearch(SplayTree<int>& spt, vector<int>& in) {
    for (int i = 0; i < (int)in.size(); i++) {
        int x = in[i];
        assert(spt.find(x)->value == x);
    }
}

void checkIndex(SplayTree<int>& spt, vector<int>& in) {
    assert((spt.tree != nullptr ? spt.tree->cnt : 0) == spt.size());
    assert(spt.size() == (int)in.size());
    for (int i = 0; i < (int)in.size(); i++) {
        assert(spt[i]->value == in[i]);
        assert(spt.indexOf(spt[i]) == i);
    }
}

void testSplay() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- Splay Tree ----------------------------------" << endl;

    {
        RBTree<int> rbt;
        SplayTree<int> spt;

        vector<int> in(1000);
        iota(in.begin(), in.end(), 0);

        {
            vector<int> t(in);
            random_shuffle(t.begin(), t.end());
            for (int i = 0; i < (int)in.size(); i++) {
                auto p1 = rbt.insert(t[i]);
                if (!p1.second)
                    cerr << "It'll never be shown!" << endl;
                assert(p1.first->value == t[i] && p1.second);

                auto p2 = spt.insert(t[i]);
                if (!p2.second)
                    cerr << "It'll never be shown!" << endl;
                assert(p2.first->value == t[i] && p2.second);
            }
            checkSearch(rbt, in);
            checkIndex(rbt, in);
            checkSearch(spt, in);
            checkIndex(spt, in);
        }
        {
            vector<int> t(in), org(in);

            random_shuffle(t.begin(), t.end());
            while (!rbt.empty()) {
                int x = t.back();
                t.pop_back();
                org.erase(find(org.begin(), org.end(), x));

                bool b1 = rbt.erase(x);
                if (!b1)
                    cerr << "It'll never be shown!" << endl;
                assert(b1);
                assert(rbt.size() == (int)t.size());
                checkSearch(rbt, org);
                checkIndex(rbt, org);

                bool b2 = spt.erase(x);
                if (!b2)
                    cerr << "It'll never be shown!" << endl;
                assert(b2);
                assert(spt.size() == (int)t.size());
                checkSearch(spt, org);
                checkIndex(spt, org);
            }
        }
    }
    cout << "OK!" << endl;

    cout << "*** Speed test RBTree vs SplayTree ***" << endl;
    {
        RBTree<int> rbt;
        SplayTree<int> spt;

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
                auto p = rbt.insert(t[i]);
                if (!p.second)
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(0);

            PROFILE_START(1);
            for (int i = 0; i < (int)t.size(); i++) {
                auto p = spt.insert(t[i]);
                if (!p.second)
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(1);
        }
        cout << "2) Find test" << endl;
        {
            vector<int> t(in);
            random_shuffle(t.begin(), t.end());

            PROFILE_START(0);
            for (int i = 0; i < (int)t.size(); i++) {
                if (rbt.find(t[i])->value.value != t[i])
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(0);

            PROFILE_START(1);
            for (int i = 0; i < (int)t.size(); i++) {
                if (spt.find(t[i])->value != t[i])
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(1);
        }
        cout << "3) Erase test" << endl;
        {
            vector<int> t(in);
            random_shuffle(t.begin(), t.end());

            PROFILE_START(0);
            for (int i = 0; i < (int)t.size(); i++) {
                if (!rbt.erase(t[i]))
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(0);

            PROFILE_START(1);
            for (int i = 0; i < (int)t.size(); i++) {
                if (!spt.erase(t[i]))
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(1);
        }
    }
    cout << "OK!" << endl;
}
