#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

#include "bipartiteGraph.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <unordered_set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

static BipartiteGraphArray<int> buildGraphArray() {
    BipartiteGraphArray<int> graph(6, 6);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 0);
    graph.addEdge(1, 3);
    graph.addEdge(2, 2);
    graph.addEdge(3, 2);
    graph.addEdge(3, 3);
    graph.addEdge(5, 5);
    return graph;
}

static BipartiteGraph<int> buildGraph() {
    BipartiteGraph<int> graph(6, 6);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 0);
    graph.addEdge(1, 3);
    graph.addEdge(2, 2);
    graph.addEdge(3, 2);
    graph.addEdge(3, 3);
    graph.addEdge(5, 5);
    return graph;
}

void testBipartiteMaching() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Bipartite Maching -------------" << endl;
    {
        auto graph = buildGraphArray();

        int ans = graph.calcMaxMatching();
        cout << "bipartite matching : " << ans << endl;
        assert(ans == 5);
        cout << graph.getLastMaxMatchingEdges() << endl;

        auto mis = graph.maxIndependentSet();
        cout << "max independent set : " << endl
             << "    " << mis.first << endl
             << "    " << mis.second << endl;
        vector<bool> misGT1{ true, true, true, true, true, true };
        vector<bool> misGT2{ false, false, false, false, true, false };
        assert(mis.first == misGT1 && mis.second == misGT2);
    }
    {
        auto graph = buildGraph();

        int ans = graph.calcMaxMatching();
        cout << "bipartite matching : " << ans << endl;
        assert(ans == 5);
        cout << graph.getLastMaxMatchingEdges() << endl;

        ans = graph.calcMaxMatchingHopcroftKarp();
        cout << "bipartite matching : " << ans << endl;
        assert(ans == 5);
        cout << graph.getLastMaxMatchingEdges() << endl;

        auto mis = graph.maxIndependentSet();
        cout << "max independent set : " << endl
             << "    " << mis.first << endl
             << "    " << mis.second << endl;
        vector<bool> misGT1{ true, true, true, true, true, true };
        vector<bool> misGT2{ false, false, false, false, true, false };
        assert(mis.first == misGT1 && mis.second == misGT2);
    }
    cout << "*** Speed Test ***" << endl;
    {
        int N = 100;
        int E = N * int(sqrt(N));
        int T = 1000;

        vector<pair<int, int>> qry;
        unordered_set<long long> S;
        for (int i = 0; i < E; i++) {
            int u = rand() % N;
            int v = rand() % N;
            if (u == v)
                continue;

            if (S.find((1ll * u << 32) + v) != S.end())
                continue;
            S.insert((1ll * u << 32) + v);

            qry.emplace_back(u, v);
        }

        BipartiteGraph<int> graph(N, N);
        for (auto& q : qry) {
            auto ans0 = graph.calcMaxMatching();
            auto ans1 = graph.calcMaxMatchingHopcroftKarp();
            if (ans0 != ans1)
                cout << "It'll never be shown" << endl;
        }

        PROFILE_START(0);
        for (auto& q : qry) {
            auto ans = graph.calcMaxMatching();
            if (ans < 0)
                cout << "It'll never be shown" << endl;
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        for (auto& q : qry) {
            auto ans = graph.calcMaxMatchingHopcroftKarp();
            if (ans < 0)
                cout << "It'll never be shown" << endl;
        }
        PROFILE_STOP(1);
    }


    cout << "OK" << endl;
}
