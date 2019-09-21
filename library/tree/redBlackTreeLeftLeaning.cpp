#include <cassert>
#include <functional>
#include <algorithm>

using namespace std;

#include "redBlackTreeLeftLeaning.h"

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

#include "redBlackTreeSimple.h"

static void checkSearch(LeftLeaningRBTree<int>& rbt, vector<int>& in) {
    for (int i = 0; i < int(in.size()); i++) {
        int x = in[i];
        assert(rbt.find(x)->value == x);
    }
}

static void checkIndex(LeftLeaningRBTree<int>& rbt, vector<int>& in) {
    assert(rbt.size() == int(in.size()));
    for (int i = 0; i < int(in.size()); i++) {
        assert(rbt[i]->value == in[i]);
        assert(rbt.indexOf(rbt[i]) == i);
    }
}

void testRedBlackTreeLeftLeaning() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Left-Leaning Red-Black Tree ----------------------------------" << endl;
    {
        LeftLeaningRBTree<int> rbt;

        int N = 5;
        vector<int> in{ 0, 1, 2, 3, 4 };

        assert(rbt.empty());
        {
            vector<int> t{ 0, 4, 2, 1, 3 };
            for (int i = 0; i < int(in.size()); i++) {
                int size = rbt.size();
                rbt.insert(t[i]);
                if (size == rbt.size())
                    cerr << "ERROR at " << __LINE__ << endl;
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

            auto it = rbt.lowerBound(0);
            auto itE = rbt.upperBound(N);
            for (int i = 0; i < N; i++) {
                assert(it->value == i);
                it = it->next();
            }
            for (int i = N - 1; i >= 0; i--) {
                if (!it)
                    it = rbt.end();
                else
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

                auto size = rbt.size();
                rbt.erase(x);
                if (size == rbt.size())
                    cerr << "ERROR at " << __LINE__ << endl;
                assert(rbt.size() == int(t.size()));
                checkSearch(rbt, org);
                checkIndex(rbt, org);
            }
        }
    }
    {
        for (int T = 0; T < 10; T++) {
            LeftLeaningRBTree<int> rbt;

            int N = 1000;
            vector<int> in(N);
            iota(in.begin(), in.end(), 0);

            assert(rbt.empty());
            {
                vector<int> t(in);
                random_shuffle(t.begin(), t.end());
                for (int i = 0; i < int(in.size()); i++) {
                    int size = rbt.size();
                    rbt.insert(t[i]);
                    if (size == rbt.size())
                        cerr << "ERROR at " << __LINE__ << endl;
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

                    auto size = rbt.size();
                    rbt.erase(x);
                    if (size == rbt.size())
                        cerr << "ERROR at " << __LINE__ << endl;
                    assert(rbt.size() == int(t.size()));
                    checkSearch(rbt, org);
                    checkIndex(rbt, org);
                }
            }
        }
    }
    cout << "OK!" << endl;
}
