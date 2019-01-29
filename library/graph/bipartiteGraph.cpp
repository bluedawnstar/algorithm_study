#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

#include "bipartiteGraph.h"
#include "bipartiteGraphArray.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <unordered_set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static BipartiteGraphArray buildGraphArray() {
    BipartiteGraphArray graph(6, 6);
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

static BipartiteGraph buildGraph() {
    BipartiteGraph graph(6, 6);
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
    return; //TODO: if you want to test, make this line a comment.

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

        auto mvc = graph.minVertexCover();
        auto mis = graph.maxIndependentSet();

        assert(mvc.first.size() == mis.first.size());
        assert(mvc.second.size() == mis.second.size());
        for (int i = 0; i < int(mvc.first.size()); i++)
            assert(mvc.first[i] != mis.first[i]);
        for (int i = 0; i < int(mvc.second.size()); i++)
            assert(mvc.second[i] != mis.second[i]);

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
            int u = RandInt32::get() % N;
            int v = RandInt32::get() % N;
            if (u == v)
                continue;

            if (S.find((1ll * u << 32) + v) != S.end())
                continue;
            S.insert((1ll * u << 32) + v);

            qry.emplace_back(u, v);
        }

        BipartiteGraph graph(N, N);
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
