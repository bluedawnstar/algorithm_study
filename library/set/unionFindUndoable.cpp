#include <map>
#include <vector>

using namespace std;

#include "unionFindUndoable.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "unionFind.h"
#include "unionFindPersistent.h"

void testUndoableUnionFind() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Undoable Union-Find ---------------------" << endl;
    {
#ifdef _DEBUG
        const int T = 10;
        const int N = 100;
        const int UPT = 20;
#else
        const int T = 100;
        const int N = 1000;
        const int UPT = 100;
#endif
        for (int tt = 0; tt < T; tt++) {
            vector<DSU> history;

            DSU dsu(N);
            history.push_back(dsu);

            UndoableUnionFind uuf(N);

            for (int i = 0; i < UPT; i++) {
                int a = RandInt32::get() % N;
                int b = RandInt32::get() % N;
                while (a == b) {
                    b = RandInt32::get() % N;
                }

                dsu.merge(a, b);
                uuf.merge(a, b);

                a = RandInt32::get() % N;
                b = RandInt32::get() % N;
                while (a == b) {
                    b = RandInt32::get() % N;
                }

                dsu.merge(a, b);
                history.push_back(dsu);

                uuf.merge(a, b);
                uuf.commit();
            }

            for (int i = 0; i < N; i++) {
                int g1 = dsu.find(i);
                int g2 = uuf.find(i);
                if (g1 != g2) {
                    cout << "Mismatched at " << __LINE__ << " : " << g1 << ", " << g2 << endl;
                }
                assert(g1 == g2);
            }

            //---

            for (int t = int(history.size()) - 1; t >= 0; t--) {
                for (int i = 0; i < N; i++) {
                    int g1 = history[t].find(i);
                    int g2 = uuf.find(i);
                    if (g1 != g2) {
                        cout << "Mismatched at " << __LINE__ << " : " << g1 << ", " << g2 << endl;
                    }
                    assert(g1 == g2);
                }
                uuf.undo();
            }
        }
    }
    {
#ifdef _DEBUG
        const int T = 1;
        const int N = 2000;
        const int UPT = 1000;
#else
        const int T = 1;
        const int N = 200'000;
        const int UPT = 100'000;
#endif
        vector<pair<int, int>> qry;
        qry.reserve(UPT * 2);

        for (int i = 0; i < UPT * 2; i++) {
            int a = RandInt32::get() % N;
            int b = RandInt32::get() % N;
            while (a == b)
                b = RandInt32::get() % N;

            qry.emplace_back(a, b);
        }

        PROFILE_START(0);
        long long dummy1 = 0;
        for (int tt = 0; tt < T; tt++) {
            DSU dsu(N);
            for (int i = 0; i < UPT; i++)
                dummy1 += dsu.merge(qry[i].first, qry[i].second);
            for (int i = 0; i < N; i++)
                dummy1 += dsu.find(i);

            for (int i = 0; i < UPT; i++)
                dummy1 += dsu.merge(qry[UPT + i].first, qry[UPT + i].second);
            for (int i = 0; i < N; i++)
                dummy1 += dsu.find(i);
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        long long dummy2 = 0;
        for (int tt = 0; tt < T; tt++) {
            UndoableUnionFind puf(N);
            for (int i = 0; i < UPT; i++) {
                dummy2 += puf.merge(qry[i].first, qry[i].second);
                puf.commit();
            }
            for (int i = 0; i < N; i++)
                dummy2 += puf.find(i);

            for (int i = 0; i < UPT; i++) {
                dummy2 += puf.merge(qry[UPT + i].first, qry[UPT + i].second);
                puf.commit();
            }
            for (int i = 0; i < N; i++)
                dummy2 += puf.find(i);
        }
        PROFILE_STOP(1);

        if (dummy1 != dummy2)
            cout << "Mismatched : " << dummy1 << ", " << dummy2 << endl;
        assert(dummy1 == dummy2);
    }

    cout << "OK!" << endl;
}
