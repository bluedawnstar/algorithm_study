#include <map>
#include <vector>

using namespace std;

#include "unionFindPersistent.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "unionFind.h"

void testPersistentUnionFind() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Persistent Union-Find ---------------------" << endl;
    {
        const int T = 100;
        const int N = 1000;
        const int UPT = 100;
        for (int tt = 0; tt < T; tt++) {
            vector<DSU> history;

            DSU dsu(N);
            history.push_back(dsu);

            PersistentUnionFind puf(N);

            for (int i = 0; i < UPT; i++) {
                int a = RandInt32::get() % N;
                int b = RandInt32::get() % N;
                while (a == b) {
                    b = RandInt32::get() % N;
                }

                dsu.merge(a, b);
                history.push_back(dsu);

                puf.merge(a, b);
                puf.upgradeTime();
            }
            
            for (int t = 0; t < int(history.size()); t++) {
                for (int i = 0; i < N; i++) {
                    int g1 = history[t].find(i);
                    int g2 = puf.find(t, i);
                    if (g1 != g2) {
                        cout << "Mismatched at " << __LINE__ << " : " << g1 << ", " << g2 << endl;
                    }
                    assert(g1 == g2);
                }
            }

            //---

            for (int i = 0; i < UPT; i++) {
                int a = RandInt32::get() % N;
                int b = RandInt32::get() % N;
                while (a == b) {
                    b = RandInt32::get() % N;
                }

                dsu.merge(a, b);
                history.push_back(dsu);

                puf.merge(a, b);
                puf.upgradeTime();
            }

            for (int t = 0; t < int(history.size()); t++) {
                for (int i = 0; i < N; i++) {
                    int g1 = history[t].find(i);
                    int g2 = puf.find(t, i);
                    if (g1 != g2) {
                        cout << "Mismatched at " << __LINE__ << " : " << g1 << ", " << g2 << endl;
                    }
                    assert(g1 == g2);
                }
            }
        }
    }

    cout << "OK!" << endl;
}
