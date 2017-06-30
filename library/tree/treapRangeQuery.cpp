#include <cassert>

using namespace std;

#include "treap.h"

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
#include "splayTree.h"
#include "segmentTreeLazy.h"

void checkSearch(RBTree<int>& rbt, vector<int>& in);
void checkIndex(RBTree<int>& rbt, vector<int>& in);

template <typename T, typename MergeOp, typename BlockOp>
void checkIndex(TreapRangeQuery<T, MergeOp, BlockOp>& tr, vector<int>& in) {
    assert((tr.tree != nullptr ? tr.tree->cnt : 0) == tr.size());
    assert(tr.size() == (int)in.size());
    for (int i = 0; i < (int)in.size(); i++) {
        assert(tr[i]->value == in[i]);
        assert(tr.indexOf(tr[i]) == i);
    }
}

void testTreapRangeQuery() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- Treap with Range Query ----------------------------------" << endl;

    srand((unsigned)time(nullptr));
    {
        RBTree<int> rbt;
        auto tr = makeTreapRangeQuery([](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);

        vector<int> in(1000);
        iota(in.begin(), in.end(), 0);

        {
            vector<int> t(in);
            for (int i = 0; i < (int)in.size(); i++) {
                auto p = tr.insert(i, t[i]);
                if (!p)
                    cerr << "It'll never be shown!" << endl;
                assert(p && p->value == t[i]);
            }
            checkIndex(tr, t);
        }
        {
            vector<int> t(in);

            for (int i = 0; i < (int)in.size(); i++) {
                int idx = rand() % (int)t.size();
                t.erase(t.begin() + idx);

                bool b = tr.erase(idx);
                if (!b)
                    cerr << "It'll never be shown!" << endl;
                assert(b);
                assert(tr.size() == (int)t.size());
                checkIndex(tr, t);
            }
        }
    }
    cout << "OK!" << endl;
    {
        int N = 10000;
        auto segt = makeSegmentTreeLazy(N, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);
        auto tr = makeTreapRangeQuery([](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);

        for (int i = 0; i < N; i++)
            tr.insert(i, 0);

        assert(segt.query(0, N - 1) == tr.query(0, N - 1));
        assert(segt.query(1, N - 1) == tr.query(1, N - 1));
        assert(segt.query(0, N - 2) == tr.query(0, N - 2));
        assert(segt.query(1, N - 2) == tr.query(1, N - 2));

        segt.update(0, 1);
        tr.update(0, 1);

        assert(segt.query(0, N - 1) == tr.query(0, N - 1));
        assert(segt.query(1, N - 1) == tr.query(1, N - 1));
        assert(segt.query(0, N - 2) == tr.query(0, N - 2));
        assert(segt.query(1, N - 2) == tr.query(1, N - 2));

        for (int i = 0; i < N; i++) {
            int x = rand() % 100;
            segt.update(i, x);
            tr.update(i, x);
        }

        assert(segt.query(0, N - 1) == tr.query(0, N - 1));
        assert(segt.query(1, N - 1) == tr.query(1, N - 1));
        assert(segt.query(0, N - 2) == tr.query(0, N - 2));
        assert(segt.query(1, N - 2) == tr.query(1, N - 2));

        for (int i = 0; i < N * 4; i++) {
            int left, right, value;

            switch (rand() % 4) {
            case 0:
                left = rand() % N;
                value = rand() % 100;
                segt.update(left, value);
                tr.update(left, value);
                assert(segt.query(left, left) == tr.query(left));
                assert(segt.query(0, N - 1) == tr.query(0, N - 1));
                assert(segt.query(1, N - 1) == tr.query(1, N - 1));
                assert(segt.query(0, N - 2) == tr.query(0, N - 2));
                assert(segt.query(1, N - 2) == tr.query(1, N - 2));
                break;
            case 1:
                left = rand() % N;
                right = rand() % N;
                if (left > right)
                    swap(left, right);
                value = rand() % 100;
                segt.updateRange(left, right, value);
                tr.update(left, right, value);
                assert(segt.query(left, right) == tr.query(left, right));
                assert(segt.query(0, N - 1) == tr.query(0, N - 1));
                assert(segt.query(1, N - 1) == tr.query(1, N - 1));
                assert(segt.query(0, N - 2) == tr.query(0, N - 2));
                assert(segt.query(1, N - 2) == tr.query(1, N - 2));
                break;
            case 2:
            {
                left = rand() % N;
                int v1 = segt.query(left, left);
                int v2 = tr.query(left);
                if (v1 != v2)
                    cerr << "Mismatched! : " << v1 << ", " << v2 << endl;
                assert(v1 == v2);
                break;
            }
            case 3:
            {
                left = rand() % N;
                right = rand() % N;
                if (left > right)
                    swap(left, right);
                int v1 = segt.query(left, right);
                int v2 = tr.query(left, right);
                if (v1 != v2) {
                    cerr << "Mismatched! : " << v1 << ", " << v2 << endl;
                    tr.query(left, right);
                }
                assert(v1 == v2);
                break;
            }
            }
        }
    }
    cout << "OK!" << endl;

    {
        const int N = 10000;

        auto tr = makeTreapRangeQuery([](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);
        vector<int> vec;

        vector<int> possible;
        for (int i = 0; i < N; i++)
            possible.push_back(i);
        random_shuffle(possible.begin(), possible.end());

        for (int i = 0; i < N * 4; i++) {
            switch (rand() % 4) {
            case 0: // insert
                if (!possible.empty()) {
                    int x = possible.front();
                    possible.erase(possible.begin());
                    int idx = lower_bound(vec.begin(), vec.end(), x) - vec.begin();
                    vec.insert(vec.begin() + idx, x);
                    tr.insert(idx, x);
                    assert(accumulate(vec.begin(), vec.end(), 0) == tr.query(0, tr.size() - 1));
                }
                break;
            case 1: // erase
                if (!vec.empty()) {
                    int idx = rand() % (int)vec.size();
                    int x = vec[idx];
                    possible.push_back(x);
                    vec.erase(vec.begin() + idx);
                    tr.erase(idx);

                    int v1 = accumulate(vec.begin(), vec.end(), 0);
                    int v2 = tr.query(0, tr.size() - 1);
                    if (v1 != v2) {
                        accumulate(vec.begin(), vec.end(), 0);
                        tr.query(0, tr.size() - 1);
                    }
                    assert(accumulate(vec.begin(), vec.end(), 0) == tr.query(0, tr.size() - 1));
                }
                break;
            case 2: // query
                if (!vec.empty()) {
                    int idx = rand() % (int)vec.size();
                    assert(vec[idx] == tr.query(idx));
                    assert(accumulate(vec.begin(), vec.end(), 0) == tr.query(0, tr.size() - 1));
                }
                break;
            case 3: // query
                if (!vec.empty()) {
                    int left = rand() % (int)vec.size();
                    int right = rand() % (int)vec.size();;
                    if (left > right)
                        swap(left, right);

                    int v1 = accumulate(vec.begin() + left, vec.begin() + right + 1, 0);
                    int v2 = tr.query(left, right);
                    if (v1 != v2) {
                        accumulate(vec.begin() + left, vec.begin() + right + 1, 0);
                        tr.query(left, right);
                    }
                    assert(v1 == v2);
                    assert(accumulate(vec.begin(), vec.end(), 0) == tr.query(0, tr.size() - 1));
                }
                break;
            }
        }
    }
    cout << "OK!" << endl;

    cout << "*** Speed test SegmentTreeLazy vs SplayTreeRangeQuery vs TreapRangeQuery ***" << endl;
    {
        int N = 1000000;
        auto segt = makeSegmentTreeLazy(N, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);
        auto spt = makeSplayTreeRangeQuery([](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);
        auto tr = makeTreapRangeQuery([](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);

        spt.build(N);
        for (int i = 0; i < N; i++)
            tr.insert(i, 0);

        for (int i = 0; i < N; i++) {
            int x = rand() % 100;
            segt.update(i, x);
            spt.update(i, x);
            tr.update(i, x);
        }

        int tN = 1000000;
        vector<tuple<int, int, int, int>> in;
        in.reserve(tN);

        for (int i = 0; i < tN; i++) {
            int l = rand() % N;
            int r = rand() % N;
            if (l > r)
                swap(l, r);
            in.emplace_back(rand() % 2, l, r, rand() % 100);
        }

        PROFILE_START(0);
        for (int i = 0; i < (int)in.size(); i++) {
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
        for (int i = 0; i < (int)in.size(); i++) {
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

        PROFILE_START(2);
        for (int i = 0; i < (int)in.size(); i++) {
            if (!get<0>(in[i])) {
                tr.update(get<1>(in[i]), get<2>(in[i]), get<3>(in[i]));
                if (tr.count <= 0)
                    cerr << "It'll never be shown!" << endl;
            } else {
                if (tr.query(get<1>(in[i]), get<2>(in[i])) < 0)
                    cerr << "It'll never be shown!" << endl;
            }
        }
        PROFILE_STOP(2);
    }
    cout << "OK!" << endl;
}
