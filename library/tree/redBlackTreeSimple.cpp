using namespace std;

#include "redBlackTreeSimple.h"

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

static void checkSearch(RBTreeSimple<int>& rbt, vector<int>& in) {
    for (int i = 0; i < (int)in.size(); i++) {
        int x = in[i];
        assert(rbt.find(x)->value == x);
    }
}

static void checkIndex(RBTreeSimple<int>& rbt, vector<int>& in) {
    assert(rbt.size() == (int)in.size());
    for (int i = 0; i < (int)in.size(); i++) {
        assert(rbt[i]->value == in[i]);
        assert(rbt.indexOf(rbt[i]) == i);
    }
}

void testRBTreeSimple() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- Simple Red-Black Tree -------------------------------" << endl;

    for (int T = 0; T < 10; T++) {
        RBTreeSimple<int> rbt;

        int N = 1000;
        vector<int> in(N);
        iota(in.begin(), in.end(), 0);

        assert(rbt.empty());
        {
            vector<int> t(in);
            random_shuffle(t.begin(), t.end());
            for (int i = 0; i < (int)in.size(); i++) {
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
            assert(rbt.upperBound(N - 1) == nullptr);

            for (int j = 0; j < 20; j++) {
                int gt = RandInt32::get() % N;
                assert(rbt.lowerBound(gt)->value == gt);
            }

            auto it = rbt.lowerBound(10);
            auto itE = rbt.upperBound(77);
            for (int i = 10; i < 77; i++) {
                assert(it->value == i);
                it = it->next();
            }
            for (int i = 76; i >= 10; i--) {
                it = it->prev();
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
                assert(rbt.size() == (int)t.size());
                checkSearch(rbt, org);
                checkIndex(rbt, org);
            }
        }
    }
    cout << "OK!" << endl;
}
