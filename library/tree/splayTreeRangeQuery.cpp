using namespace std;

#include "splayTree.h"
#include "splayTreeRangeQuery.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <numeric>
#include <set>
#include <vector>
#include <bitset>
#include <unordered_set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "redBlackTree.h"
#include "../rangeQuery/segmentTreeLazy.h"

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

template <typename T, typename MergeOp, typename BlockOp>
static void checkSearch(SplayTreeRangeQuery<T, MergeOp, BlockOp>& spt, vector<int>& in) {
    for (int i = 0; i < int(in.size()); i++) {
        int x = in[i];
        assert(spt.find(x)->value == x);
    }
}

template <typename T, typename MergeOp, typename BlockOp>
static void checkIndex(SplayTreeRangeQuery<T, MergeOp, BlockOp>& spt, vector<int>& in) {
    assert((spt.tree != nullptr ? spt.tree->cnt : 0) == spt.size());
    assert(spt.size() == int(in.size()));
    for (int i = 0; i < int(in.size()); i++) {
        assert(spt[i]->value == in[i]);
        assert(spt.indexOf(spt[i]) == i);
    }
}

void testSplayRangeQuery() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Splay Tree with Range Query ----------------------------------" << endl;

    {
        RBTree<int> rbt;
        auto spt = makeSplayTreeRangeQuery([](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);

        vector<int> in(1000);
        iota(in.begin(), in.end(), 0);

        {
            vector<int> t(in);
            random_shuffle(t.begin(), t.end());
            for (int i = 0; i < int(in.size()); i++) {
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
                assert(rbt.size() == int(t.size()));
                checkSearch(rbt, org);
                checkIndex(rbt, org);

                bool b2 = spt.erase(x);
                if (!b2)
                    cerr << "It'll never be shown!" << endl;
                assert(b2);
                assert(spt.size() == int(t.size()));
                checkSearch(spt, org);
                checkIndex(spt, org);
            }
        }
    }
    cout << "OK!" << endl;
    {
        int N = 10000;
#ifdef _DEBUG
        N = 1000;
#endif
        auto segt = makeSegmentTreeLazy(N, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);
        auto spt = makeSplayTreeRangeQuery([](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);

        spt.build(N);

        for (int i = 0; i < N; i++) {
            int x = RandInt32::get() % 100;
            segt.update(i, x);
            spt.update(i, x);
        }

        assert(segt.query(0, N - 1) == spt.query(0, N - 1));
        assert(segt.query(1, N - 1) == spt.query(1, N - 1));
        assert(segt.query(0, N - 2) == spt.query(0, N - 2));
        assert(segt.query(1, N - 2) == spt.query(1, N - 2));

        for (int i = 0; i < N * 4; i++) {
            int left, right, value;

            switch (RandInt32::get() % 4) {
            case 0:
                left = RandInt32::get() % N;
                value = RandInt32::get() % 100;
                segt.update(left, value);
                spt.update(left, value);
                assert(segt.query(left, left) == spt.query(left));
                assert(segt.query(0, N - 1) == spt.query(0, N - 1));
                assert(segt.query(1, N - 1) == spt.query(1, N - 1));
                assert(segt.query(0, N - 2) == spt.query(0, N - 2));
                assert(segt.query(1, N - 2) == spt.query(1, N - 2));
                break;
            case 1:
                left = RandInt32::get() % N;
                right = RandInt32::get() % N;
                if (left > right)
                    swap(left, right);
                value = RandInt32::get() % 100;
                segt.updateRange(left, right, value);
                spt.update(left, right, value);
                assert(segt.query(left, right) == spt.query(left, right));
                assert(segt.query(0, N - 1) == spt.query(0, N - 1));
                assert(segt.query(1, N - 1) == spt.query(1, N - 1));
                assert(segt.query(0, N - 2) == spt.query(0, N - 2));
                assert(segt.query(1, N - 2) == spt.query(1, N - 2));
                break;
            case 2:
            {
                left = RandInt32::get() % N;
                int v1 = segt.query(left, left);
                int v2 = spt.query(left);
                if (v1 != v2)
                    cerr << "Mismatched! : " << v1 << ", " << v2 << endl;
                assert(v1 == v2);
                break;
            }
            case 3:
            {
                left = RandInt32::get() % N;
                right = RandInt32::get() % N;
                if (left > right)
                    swap(left, right);
                int v1 = segt.query(left, right);
                int v2 = spt.query(left, right);
                if (v1 != v2) {
                    cerr << "Mismatched! : " << v1 << ", " << v2 << endl;
                    spt.query(left, right);
                }
                assert(v1 == v2);
                break;
            }
            }
        }
    }
    cout << "OK!" << endl;

    {
#ifdef _DEBUG
        const int N = 1000;
#else
        const int N = 10000;
#endif
        auto spt = makeSplayTreeRangeQuery([](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);
        vector<int> vec;

        vector<int> possible;
        for (int i = 0; i < N; i++)
            possible.push_back(i);
        random_shuffle(possible.begin(), possible.end());

        for (int i = 0; i < N * 4; i++) {
            switch (RandInt32::get() % 4) {
            case 0: // insert
                if (!possible.empty()) {
                    int x = possible.front();
                    possible.erase(possible.begin());
                    vec.insert(lower_bound(vec.begin(), vec.end(), x), x);
                    spt.insert(x);
                    assert(accumulate(vec.begin(), vec.end(), 0) == spt.query(0, spt.size() - 1));
                }
                break;
            case 1: // erase
                if (!vec.empty()) {
                    int idx = RandInt32::get() % int(vec.size());
                    int x = vec[idx];
                    possible.push_back(x);
                    vec.erase(vec.begin() + idx);
                    spt.erase(x);

                    int v1 = accumulate(vec.begin(), vec.end(), 0);
                    int v2 = spt.query(0, spt.size() - 1);
                    if (v1 != v2) {
                        accumulate(vec.begin(), vec.end(), 0);
                        spt.query(0, spt.size() - 1);
                    }
                    assert(accumulate(vec.begin(), vec.end(), 0) == spt.query(0, spt.size() - 1));
                }
                break;
            case 2: // query
                if (!vec.empty()) {
                    int idx = RandInt32::get() % int(vec.size());
                    assert(vec[idx] == spt.query(idx));
                    assert(accumulate(vec.begin(), vec.end(), 0) == spt.query(0, spt.size() - 1));
                }
                break;
            case 3: // query
                if (!vec.empty()) {
                    int left = RandInt32::get() % int(vec.size());
                    int right = RandInt32::get() % int(vec.size());;
                    if (left > right)
                        swap(left, right);

                    int v1 = accumulate(vec.begin() + left, vec.begin() + right + 1, 0);
                    int v2 = spt.query(left, right);
                    if (v1 != v2) {
                        accumulate(vec.begin() + left, vec.begin() + right + 1, 0);
                        spt.query(left, right);
                    }
                    assert(v1 == v2);
                    assert(accumulate(vec.begin(), vec.end(), 0) == spt.query(0, spt.size() - 1));
                }
                break;
            }
        }
    }
    cout << "OK!" << endl;

    cout << "*** Speed test SegmentTreeLazy vs SplayTreeRangeQuery ***" << endl;
    {
        int N = 100000;
#ifdef _DEBUG
        N = 10000;
#endif
        auto segt = makeSegmentTreeLazy(N, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);
        auto spt = makeSplayTreeRangeQuery([](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);

        spt.build(N);

        for (int i = 0; i < N; i++) {
            int x = RandInt32::get() % 100;
            segt.update(i, x);
            spt.update(i, x);
        }

        int tN = 1000000;
#ifdef _DEBUG
        tN = 10000;
#endif
        vector<tuple<int, int, int, int>> in;
        in.reserve(tN);

        for (int i = 0; i < tN; i++) {
            int l = RandInt32::get() % N;
            int r = RandInt32::get() % N;
            if (l > r)
                swap(l, r);
            in.emplace_back(RandInt32::get() % 2, l, r, RandInt32::get() % 100);
        }

        PROFILE_START(0);
        for (int i = 0; i < int(in.size()); i++) {
            if (!get<0>(in[i])) {
                segt.updateRange(get<1>(in[i]), get<2>(in[i]), get<3>(in[i]));
                if (segt.N <= 0)
                    cerr << "It'll never be shown!" << endl;
            } else {
                if (segt.query(get<1>(in[i]), get<2>(in[i])) < 0)
                    cerr << "It'll never be shown!" << endl;
            }
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        for (int i = 0; i < int(in.size()); i++) {
            if (!get<0>(in[i])) {
                spt.update(get<1>(in[i]), get<2>(in[i]), get<3>(in[i]));
                if (spt.count <= 0)
                    cerr << "It'll never be shown!" << endl;
            } else {
                if (spt.query(get<1>(in[i]), get<2>(in[i])) < 0)
                    cerr << "It'll never be shown!" << endl;
            }
        }
        PROFILE_STOP(1);
    }
    cout << "OK!" << endl;
}
