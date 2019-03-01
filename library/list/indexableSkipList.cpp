#include <vector>
#include <algorithm>

using namespace std;

#include "indexableSkipList.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include <set>
#include "../tree/redBlackTree.h"

static void checkSearch(IndexableSkipList<int>& skl, vector<int>& in) {
    for (int i = 0; i < int(in.size()); i++) {
        int x = in[i];
        assert(skl.find(x)->value == x);
    }
}

static void checkIndex(IndexableSkipList<int>& skl, vector<int>& in) {
    assert(skl.size() == int(in.size()));
    for (int i = 0; i < int(in.size()); i++) {
        auto ok = (skl[i]->value == in[i]);
        if (!ok)
            cerr << "Mismatched!" << endl;
        assert(ok);
    }
}

void testIndexableSkipList() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Indexable Skip List ------------------------" << endl;
#ifdef _DEBUG
    for (int T = 0; T < 2; T++) {
#else
    for (int T = 0; T < 10; T++) {
#endif
        IndexableSkipList<int> skl(26);

        int N = 1000;
        vector<int> in(N);
        iota(in.begin(), in.end(), 0);

        assert(skl.empty());
        {
            vector<int> t(in);
            random_shuffle(t.begin(), t.end());
            for (int i = 0; i < int(in.size()); i++) {
                auto p = skl.insert(t[i]);
                if (!p.second)
                    cerr << "It'll never be shown!" << endl;
                auto ok = (p.first->value == t[i] && p.second);
                assert(ok);
                if (!ok)
                    cerr << "Mismatched!" << endl;
            }
            checkSearch(skl, in);
            checkIndex(skl, in);
        }
        {
            auto ok1 = (skl.lowerBound(-1)->value == 0);
            auto ok2 = (skl.upperBound(N - 1) == nullptr);

            assert(ok1);
            assert(ok2);
            if (!ok1)
                cerr << "Mismatched!" << endl;
            if (!ok2)
                cerr << "Mismatched!" << endl;

            for (int j = 0; j < 20; j++) {
                int gt = RandInt32::get() % N;
                auto ok = (skl.lowerBound(gt)->value == gt);
                assert(ok);
                if (!ok)
                    cerr << "Mismatched!" << endl;
            }

            auto it = skl.lowerBound(10);
            auto itE = skl.upperBound(77);
            for (int i = 10; i < 77; i++) {
                assert(it->value == i);
                if (it->value != i)
                    cerr << "Mismatched!" << endl;
                it = skl.next(it);
            }
        }
        {
            vector<int> t(in), org(in);

            random_shuffle(t.begin(), t.end());
            while (!skl.empty()) {
                int x = t.back();
                t.pop_back();
                org.erase(find(org.begin(), org.end(), x));

                bool b = skl.erase(x);
                if (!b)
                    cerr << "It'll never be shown!" << endl;
                assert(b);
                auto ok = (skl.size() == int(t.size()));
                assert(ok);
                if (!ok)
                    cerr << "Mismatched!" << endl;
                checkSearch(skl, org);
                checkIndex(skl, org);
            }
        }
    }
    cout << "OK!" << endl;

#ifdef _DEBUG
    for (int T = 0; T < 2; T++) {
#else
    for (int T = 0; T < 10; T++) {
#endif
        IndexableSkipList<int> skl(26);

        int N = 1000;
        vector<int> in;

        {
            assert(skl.empty());
            for (int i = 0; i < N; i++) {
                int index = RandInt32::get() % (int(in.size()) + 1);
                int x = RandInt32::get();

                in.insert(in.begin() + index, x);
                auto p = skl.insertAt(index, x);
                if (!p.second)
                    cerr << "It'll never be shown!" << endl;
                auto ok = (p.first->value == in[index] && p.second);
                assert(ok);
                if (!ok)
                    cerr << "Mismatched!" << endl;
            }
            checkIndex(skl, in);
        }
        {
            for (int i = 0; i < N; i++) {
                int index = RandInt32::get() % int(in.size());

                in.erase(in.begin() + index);
                bool b = skl.eraseAt(index);
                if (!b)
                    cerr << "It'll never be shown!" << endl;
                assert(b);
                auto ok = (skl.size() == int(in.size()));
                assert(ok);
                if (!ok)
                    cerr << "Mismatched!" << endl;
                checkIndex(skl, in);
            }
            assert(skl.empty());
        }
    }
    cout << "OK!" << endl;

    cout << "*** Speed test BST vs RBTree vs IndexableSkipList ***" << endl;
    {
        BST<int> bst;
        RBTree<int> rbt;
        set<int> M;
        IndexableSkipList<int> skl(30);

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

            PROFILE_START(3);
            for (int i = 0; i < int(t.size()); i++) {
                auto p = skl.insert(t[i]);
                if (!p.second)
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(3);
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

            PROFILE_START(3);
            for (int i = 0; i < int(t.size()); i++) {
                if (skl.find(t[i])->value != t[i])
                    cerr << "It'll never be shown!" << endl;
            }
            PROFILE_STOP(3);
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

            int sum4 = 0;
            PROFILE_START(3);
            for (auto it = skl.begin(); it; it = skl.next(it)) {
                sum4 += it->value;
            }
            PROFILE_STOP(3);
            if (!sum4)
                cerr << "dummy message" << endl;

            if (sum1 != sum2 || sum1 != sum3 || sum1 != sum4)
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

            PROFILE_START(3);
            for (int i = 0; i < int(t.size()); i++) {
                skl.erase(t[i]);
            }
            PROFILE_STOP(3);
        }
    }
    cout << "OK!" << endl;
}
