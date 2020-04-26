#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

#include "treeDP_sameDistanceTripleCounter.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

struct SameDistanceTripleCounterNaive {
    SameDistanceTripleCounterOnTree& tree;

    SameDistanceTripleCounterNaive(SameDistanceTripleCounterOnTree& tree) : tree(tree) {
    }

    //---

    vector<tuple<int, int, int>> vec[2]; // (id, u, parent)

    long long bfsSolve(int u) {
        vec[0].clear();
        int from = 0, to = 1;

        for (int i = 0; i < int(tree.edges[u].size()); i++) {
            int v = tree.edges[u][i];
            vec[from].emplace_back(i, v, u);
        }

        long long res = 0;
        while (int(vec[from].size()) >= 3) {
            vec[to].clear();

            long long one = 0, two = 0, three = 0;
            long long okLastID = -1, okCnt = 0;

            int lastID = -1;
            int idCount = 0;

            int id, u, parent;
            for (int i = 0; i < int(vec[from].size()); i++) {
                tie(id, u, parent) = vec[from][i];

                if (tree.marks[u]) {
                    if (okLastID != id) {
                        one += okCnt;
                        two += okCnt * okCnt;
                        three += okCnt * okCnt * okCnt;

                        okLastID = id;
                        okCnt = 1;
                    } else {
                        okCnt++;
                    }
                }

                if (lastID != id) {
                    lastID = id;
                    idCount++;
                }

                for (auto v : tree.edges[u]) {
                    if (v != parent)
                        vec[to].emplace_back(id, v, u);
                }
            }

            if (idCount < 3)
                break;

            one += okCnt;
            two += okCnt * okCnt;
            three += okCnt * okCnt * okCnt;

            res += one * one * one - three - 3 * (two * one - three);

            swap(from, to);
        }

        return res;
    }

    long long solve() {
        long long res = 0;
        for (int u = 0; u < tree.N; u++) {
            if (int(tree.edges[u].size()) >= 3)
                res += bfsSolve(u);
        }

        return res;
    }
};

void testSameDistanceTripleCounterOnTree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Same Distance Triple Counter on a Tree ---------------------" << endl;
    {
        int T = 100;
        int N = 100;
        for (int i = 0; i < T; i++) {
            SameDistanceTripleCounterOnTree counter(N);
            for (int j = 1; j < N; j++)
                counter.addEdge(RandInt32::get() % j, j);
            for (int j = 0; j < N; j++) {
                if (RandInt32::get() & 1)
                    counter.setMark(j);
            }

            SameDistanceTripleCounterNaive counterNaive(counter);

            long long ans1 = counter.solve();
            long long ans2 = counterNaive.solve();

            if (ans1 != ans2) {
                cout << "Mismatched : " << ans1 << ", " << ans2 << endl;
            }
            assert(ans1 == ans2);
        }
    }

    cout << "OK!" << endl;
}
