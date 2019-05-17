#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

#include "bipartiteMatchingKuhn.h"
#include "bipartiteMatchingHopcroftKarp.h"
#include "bipartiteMatchingKuhnArray.h"

// min vertex cover
// https://www.codechef.com/MAY19A/problems/ADAPWN

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <unordered_set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static BipartiteMatchingKuhnArray buildGraphKuhnArray() {
    BipartiteMatchingKuhnArray graph(6, 6);
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

static BipartiteMatchingKuhn buildGraphKuhn() {
    BipartiteMatchingKuhn graph(6, 6);
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

static BipartiteMatchingHopcroftKarp buildGraphHopcroftKarp() {
    BipartiteMatchingHopcroftKarp graph(6, 6);
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
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Bipartite Maching -------------" << endl;
    {
        auto graph = buildGraphKuhnArray();

        int ans = graph.calcMaxMatching();
        cout << "bipartite matching : " << ans << endl;
        assert(ans == 5);
        cout << graph.match << endl;

        auto mis = graph.maxIndependentSet();
        cout << "max independent set : " << endl
            << "    " << mis.first << endl
            << "    " << mis.second << endl;
        vector<bool> misGT1{ true, true, true, true, true, true };
        vector<bool> misGT2{ false, false, false, false, true, false };
        assert(mis.first == misGT1 && mis.second == misGT2);
    }
    {
        auto graph = buildGraphKuhn();

        int ans = graph.calcMaxMatching();
        cout << "bipartite matching : " << ans << endl;
        assert(ans == 5);
        cout << graph.match << endl;

        auto mis = graph.maxIndependentSet();
        cout << "max independent set : " << endl
            << "    " << mis.first << endl
            << "    " << mis.second << endl;
        vector<bool> misGT1{ true, true, true, true, true, true };
        vector<bool> misGT2{ false, false, false, false, true, false };
        assert(mis.first == misGT1 && mis.second == misGT2);
    }
    {
        auto graph = buildGraphHopcroftKarp();

        int ans = graph.calcMaxMatching();
        cout << "bipartite matching : " << ans << endl;
        assert(ans == 5);
        cout << graph.match << endl;

        ans = graph.calcMaxMatching();
        cout << "bipartite matching : " << ans << endl;
        assert(ans == 5);
        cout << graph.match << endl;

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
        int N = 10000;
        int E = N * int(sqrt(N));

#ifdef _DEBUG
        N = 500;
#endif
        vector<pair<int, int>> edges;
        unordered_set<long long> S;
        for (int i = 0; i < E; i++) {
            int u = RandInt32::get() % N;
            int v = RandInt32::get() % N;
            if (u == v)
                continue;

            if (S.find((1ll * u << 32) + v) != S.end())
                continue;
            S.insert((1ll * u << 32) + v);

            edges.emplace_back(u, v);
        }

        long long ansKuhn = 0;
        long long ansHopcroftKarp = 0;
        long long ansHopcroftKarpFast = 0;
        {
            BipartiteMatchingKuhn graph(N, N);
            for (auto& e : edges)
                graph.addEdge(e.first, e.second);

            PROFILE_START(0);
            ansKuhn += graph.calcMaxMatching();
            PROFILE_STOP(0);
        }
        {
            BipartiteMatchingHopcroftKarp graph(N, N);
            for (auto& e : edges)
                graph.addEdge(e.first, e.second);

            PROFILE_START(1);
            ansHopcroftKarp += graph.calcMaxMatching();
            PROFILE_STOP(1);

            PROFILE_START(2);
            ansHopcroftKarpFast += graph.calcMaxMatchingFast();
            PROFILE_STOP(2);
        }

        assert(ansKuhn == ansHopcroftKarp);
        assert(ansKuhn == ansHopcroftKarpFast);
        if (ansKuhn != ansHopcroftKarp || ansKuhn != ansHopcroftKarpFast)
            cout << "Answers are mismatched!" << endl;
    }

    cout << "OK" << endl;
}
