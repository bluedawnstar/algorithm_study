#include <queue>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

#include "maxFlowDinic.h"
#include "gomoryHuTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testGomoryHuTree() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- GomoryHu Tree ---------" << endl;

    {
        GomoryHuTreeBuilder<int> gomory(6);
        MaxFlowDinic<int> dinic(6);

        /*
        int in[][6] = {
        { 0, 1, 7, 0, 0, 0 },
        { 1, 0, 1, 3, 2, 0 },
        { 7, 1, 0, 0, 4, 0 },
        { 0, 3, 0, 0, 1, 6 },
        { 0, 2, 4, 1, 0, 2 },
        { 0, 0, 0, 6, 2, 0 }
        };
        */

        gomory.addEdge(0, 1, 1); dinic.addEdge(0, 1, 1, 1);
        gomory.addEdge(0, 2, 7); dinic.addEdge(0, 2, 7, 7);
        gomory.addEdge(1, 2, 3); dinic.addEdge(1, 2, 3, 3);
        gomory.addEdge(1, 4, 2); dinic.addEdge(1, 4, 2, 2);
        gomory.addEdge(2, 4, 4); dinic.addEdge(2, 4, 4, 4);
        gomory.addEdge(3, 4, 1); dinic.addEdge(3, 4, 1, 1);
        gomory.addEdge(3, 5, 6); dinic.addEdge(3, 5, 6, 6);
        gomory.addEdge(4, 5, 2); dinic.addEdge(4, 5, 2, 2);

        auto tree = gomory.build();

        for (int s = 0; s < 6; s++) {
            for (int t = 0; t < 6; t++) {
                if (s == t)
                    continue;

                dinic.clearFlow();
                auto gt = dinic.calcMaxFlow(s, t);

                auto flow = tree.calcMaxFlow(s, t);
                if (gt != flow)
                    cout << "gt = " << gt << ", ans = " << flow << endl;
                assert(flow == gt);
            }
        }
    }
    {
        int N = 10;
        int E = 100;
        int T = 100;
        GomoryHuTreeBuilder<int> gomory(N);
        MaxFlowDinic<int> dinic(N);

        for (int i = 0; i < E; i++) {
            int u = RandInt32::get() % N;
            int v = RandInt32::get() % N;
            if (u == v)
                continue;

            int flow = RandInt32::get() % N + 1;

            gomory.addEdge(u, v, flow);
            dinic.addEdge(u, v, flow, flow);
        }

        auto tree = gomory.build();

        for (int i = 0; i < T; i++) {
            int s = RandInt32::get() % N;
            int t = RandInt32::get() % N;
            if (s == t)
                continue;

            dinic.clearFlow();
            auto gt = dinic.calcMaxFlow(s, t);

            auto flow = tree.calcMaxFlow(s, t);
            if (gt != flow)
                cout << "gt = " << gt << ", ans = " << flow << endl;
            assert(flow == gt);
        }
    }
    cout << "*** Speed Test ***" << endl;
    {
        int N = 10;
        int E = 100;
        int T = 100;
        GomoryHuTreeBuilder<int> gomory(N);
        MaxFlowDinic<int> dinic(N);

        for (int i = 0; i < E; i++) {
            int u = RandInt32().get() % N;
            int v = RandInt32().get() % N;
            if (u == v)
                continue;

            int flow = RandInt32::get() % N + 1;

            gomory.addEdge(u, v, flow);
            dinic.addEdge(u, v, flow, flow);
        }
        auto tree = gomory.build();

        vector<pair<int, int>> qry;
        for (int i = 0; i < T; i++) {
            int s = RandInt32::get() % N;
            int t = RandInt32::get() % N;
            if (s == t)
                continue;
            qry.emplace_back(s, t);
        }

        for (auto& q : qry) {
            dinic.clearFlow();
            auto ans0 = dinic.calcMaxFlow(q.first, q.second);
            auto ans1 = tree.calcMaxFlow(q.first, q.second);
            if (ans0 != ans1)
                cout << "It'll never be shown" << endl;
        }

        PROFILE_START(0);
        for (auto& q : qry) {
            dinic.clearFlow();
            auto ans = dinic.calcMaxFlow(q.first, q.second);
            if (ans < 0)
                cout << "It'll never be shown" << endl;
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        for (auto& q : qry) {
            auto ans = tree.calcMaxFlow(q.first, q.second);
            if (ans < 0)
                cout << "It'll never be shown" << endl;
        }
        PROFILE_STOP(1);
    }

    cout << "OK!" << endl;
}
