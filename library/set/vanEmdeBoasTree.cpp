#include <cassert>
#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

#include "vanEmdeBoasTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include <set>
#include "bitSetSimple64.h"

void testVanEmdeBoasTree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- van Emde Boas Tree ------------------------" << endl;
    {
        int N = 100000;
        int T = 1000000;

        VanEmdeBoasTree<int> veb(N);
        BitSetSimple64 bs(N);

        for (int i = 0; i < T; i++) {
            int x = RandInt32::get() % N;
            int cmd = RandInt32::get() % 2;
            if (cmd) {
                veb.insert(x);
                bs.set(x);
            }
            else {
                veb.erase(x);
                bs.reset(x);
            }
#ifndef _DEBUG
            if (veb.min() != bs.first())
                cout << "min() error!" << endl;
            if (veb.max() != bs.last())
                cout << "max() error!" << endl;
            if (veb.prev(x) != bs.prev(x))
                cout << "prev() error!" << endl;
            if (veb.next(x) != bs.next(x))
                cout << "next() error!" << endl;
#else
            assert(veb.min() == bs.first());
            assert(veb.max() == bs.last());
            assert(veb.prev(x) == bs.prev(x));
            assert(veb.next(x) == bs.next(x));
#endif
        }

        for (int i = 0; i < N; i++) {
#ifndef _DEBUG
            if (veb.exist(i) != bs.test(i))
                cout << "exist() error!" << endl;
#else
            assert(veb.exist(i) == bs.test(i));
#endif
        }

        int pos = bs.prev(N);
        while (pos) {
#ifndef _DEBUG
            if (veb.popMax() != pos)
                cout << "popMin() error!" << endl;
#else
            assert(veb.popMax() == pos);
#endif
            pos = bs.prev(pos);
        }
    }
    cout << "Speed test: VanEmdeBoasTree vs BitSetSimple" << endl;
    {
        int N = 100000;
        int T = 1000000;
        int C = 10000;

        VanEmdeBoasTree<int> veb(N);
        BitSetSimple64 bs(N);

        vector<pair<int, int>> cmds(C);

        for (int i = 0; i < C; i++) {
            cmds[i].first = RandInt32::get() % N;
            cmds[i].second = RandInt32::get() % 2;
        }

        PROFILE_START(0);
        for (int i = 0; i < C; i++) {
            int x = cmds[i].first;
            if (cmds[i].second)
                veb.insert(x);
            else
                veb.erase(x);
        }
        PROFILE_STOP(0);
        if (veb.empty())
            cout << "ERROR!" << endl;

        PROFILE_START(1);
        for (int i = 0; i < C; i++) {
            int x = cmds[i].first;
            if (cmds[i].second)
                bs.set(x);
            else
                bs.reset(x);
        }
        PROFILE_STOP(1);
        if (bs.count() == 0)
            cout << "ERROR!" << endl;

        PROFILE_START(2);
        for (int i = 0; i < T; i++) {
            int x = veb.prev(cmds[i % C].first);
            if (x >= N)
                cout << "ERROR!" << endl;
        }
        PROFILE_STOP(2);

        PROFILE_START(3);
        for (int i = 0; i < T; i++) {
            int x = bs.prev(cmds[i % C].first);
            if (x >= N)
                cout << "ERROR!" << endl;
        }
        PROFILE_STOP(3);

        PROFILE_START(4);
        for (int i = 0; i < T; i++) {
            int x = veb.popMin();
            if (x >= N)
                cout << "ERROR!" << endl;
        }
        PROFILE_STOP(4);

        PROFILE_START(5);
        for (int i = 0; i < T; i++) {
            int x = bs.next(-1);
            if (x >= N)
                cout << "ERROR!" << endl;
        }
        PROFILE_STOP(5);
    }
    cout << "Speed test: VanEmdeBoasTree vs set" << endl;
    {
        int N = 100000;
        int T = 1000000;
        int C = 10000;

        VanEmdeBoasTree<int> veb(N);
        set<int> st;

        vector<pair<int, int>> cmds(C);

        for (int i = 0; i < C; i++) {
            cmds[i].first = RandInt32::get() % N;
            cmds[i].second = RandInt32::get() % 2;
        }

        PROFILE_START(0);
        for (int i = 0; i < C; i++) {
            int x = cmds[i].first;
            if (cmds[i].second)
                veb.insert(x);
            else
                veb.erase(x);
        }
        PROFILE_STOP(0);
        if (veb.empty())
            cout << "ERROR!" << endl;

        PROFILE_START(1);
        for (int i = 0; i < C; i++) {
            int x = cmds[i].first;
            if (cmds[i].second)
                st.insert(x);
            else
                st.erase(x);
        }
        PROFILE_STOP(1);
        if (st.empty())
            cout << "ERROR!" << endl;

        PROFILE_START(2);
        for (int i = 0; i < T; i++) {
            int x = veb.prev(cmds[i % C].first);
            if (x >= N)
                cout << "ERROR!" << endl;
        }
        PROFILE_STOP(2);

        PROFILE_START(3);
        for (int i = 0; i < T; i++) {
            auto it = st.upper_bound(cmds[i % C].first);
            if (it != st.end() && *it >= N)
                cout << "ERROR!" << endl;
        }
        PROFILE_STOP(3);

        PROFILE_START(4);
        for (int i = 0; i < T; i++) {
            int x = veb.popMin();
            if (x >= N)
                cout << "ERROR!" << endl;
        }
        PROFILE_STOP(4);

        PROFILE_START(5);
        for (int i = 0; i < T; i++) {
            if (!st.empty()) {
                int x = *st.begin();
                st.erase(st.begin());
                if (x >= N)
                    cout << "ERROR!" << endl;
            }
        }
        PROFILE_STOP(5);
    }

    cout << "OK!" << endl;
}
