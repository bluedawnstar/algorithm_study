#include <climits>
#include <numeric>
#include <queue>
#include <algorithm>
#include <vector>

using namespace std;

#include "shortestPathAllPair.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static const int INF = ShortestAllPairs<int>::INF;

void testShortestPathAllPairs() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Shortest Path - All Pairs ---------" << endl;
    {
        vector<vector<int>> D;
        ShortestAllPairs<int> graph(3);
        graph.addEdge(0, 1, 3);
        graph.addEdge(0, 2, 2);
        graph.addEdge(1, 0, 0);
        graph.addEdge(2, 0, ShortestAllPairs<int>::INF);
        graph.addEdge(2, 1, 0);
        
        //--- Floyd-Warshal
        vector<vector<int>> parent;
        graph.doFloydWarshal(D, parent, 3);

        assert(D[0][0] == 0);
        assert(D[0][1] == 2);
        assert(D[0][2] == 2);
        assert(parent[0][0] == -1);
        assert(parent[0][1] == 2);
        assert(parent[0][2] == 0);
        
        vector<int> pathGT{ 0, 2, 1 };
        auto pathAns = ShortestAllPairs<int>::getPathFloydWarshal(parent, 3, 0, 1);
        assert(pathGT == pathAns);

        //--- Johnson
        vector<vector<int>> D2;
        vector<vector<int>> parent2;
        graph.findAllPathJohnson(D2, parent2, 3);
        assert(D == D2);
        assert(parent == parent2);

        auto pathAns2 = ShortestAllPairs<int>::getShortestPath(parent2, 0, 1);
        assert(pathAns == pathAns2);

        //--- SPFA
        vector<vector<int>> D3;
        vector<vector<int>> parent3;
        graph.findAllPathSpfa(D3, parent3, 3);
        assert(D == D3);
        assert(parent == parent3);

        auto pathAns3 = ShortestAllPairs<int>::getShortestPath(parent3, 0, 1);
        assert(pathAns == pathAns3);
    }
    {
        vector<vector<int>> D;
        ShortestAllPairs<int> graph(4);
        graph.addEdge(0, 1, -5);
        graph.addEdge(0, 2, 2);
        graph.addEdge(0, 3, 3);
        graph.addEdge(1, 2, 4);
        graph.addEdge(2, 3, 1);

        //--- Floyd-Warshal
        vector<vector<int>> parent;
        graph.doFloydWarshal(D, parent, 4);

        assert(D[0][0] == 0);
        assert(D[1][1] == 0);
        assert(D[2][2] == 0);
        assert(D[3][3] == 0);
        assert(D[0][1] == -5);
        assert(D[0][2] == -1);
        assert(D[0][3] == 0);
        assert(D[1][2] == 4);
        assert(D[1][3] == 5);
        assert(D[2][3] == 1);

        //--- Johnson
        vector<vector<int>> D2;
        vector<vector<int>> parent2;
        graph.findAllPathJohnson(D2, parent2, 4);
        if (D != D2) {
            cout << "D:" << endl;
            for (int i = 0; i < 4; i++)
                cout << D[i] << endl;
            cout << "D2:" << endl;
            for (int i = 0; i < 4; i++)
                cout << D2[i] << endl;
        }
        assert(D == D2);

        //--- SPFA
        vector<vector<int>> D3;
        vector<vector<int>> parent3;
        graph.findAllPathSpfa(D3, parent3, 4);
        assert(D == D3);
    }
    cout << "*** Speed Test ***" << endl;
    {
        int N = 100;
        int E = 1000;

        //srand(time(nullptr));

        for (int T = 0; T < 10; T++) {
            vector<pair<int, int>> edges;
            for (int i = 0; i < E; i++) {
                int u = RandInt32::get() % N;
                int v = RandInt32::get() % N;
                if (u == v)
                    continue;
                edges.emplace_back(u, v);
            }
            sort(edges.begin(), edges.end());
            edges.erase(unique(edges.begin(), edges.end()), edges.end());
            random_shuffle(edges.begin(), edges.end());

            ShortestAllPairs<int> graph(N);
            for (auto& e : edges)
                graph.addEdge(e.first, e.second, (RandInt32::get() % N) + 1);
            for (int u = 0; u < N; u += 2) {
                if (graph.edges[u].empty())
                    continue;
                graph.edges[u][0].second = -2;
                //break;
            }

            //--- Floyd-Warshal
            PROFILE_START(0);
            vector<vector<int>> ans1;
            vector<vector<int>> par1;
            graph.doFloydWarshal(ans1, par1, N);
            PROFILE_STOP(0);

            //--- Johnson
            PROFILE_START(1);
            vector<vector<int>> ans2;
            vector<vector<int>> par2;
            auto ok2 = graph.findAllPathJohnson(ans2, par2, N);
            PROFILE_STOP(1);
            if (!ok2)
                cout << "There are negative cycles" << endl;

            //--- SPFA
            PROFILE_START(2);
            vector<vector<int>> ans3;
            vector<vector<int>> par3;
            auto ok3 = graph.findAllPathSpfa(ans3, par3, N);
            PROFILE_STOP(2);
            if (!ok3)
                cout << "There are negative cycles" << endl;

            if (ok2 == false) {
                if (ok2 != ok3)
                    cerr << "Mismatched : ok2 <-> ok3" << endl;
                assert(ok2 == ok3);
            } else {
                if (ans1 != ans2)
                    cerr << "Mismatched : ans1 <-> ans2" << endl;
                if (ans1 != ans3)
                    cerr << "Mismatched : ans1 <-> ans3" << endl;
                if (ans2 != ans3)
                    cerr << "Mismatched : ans2 <-> ans3" << endl;
                assert(ans1 == ans2);
                assert(ans1 == ans3);
            }
        }
    }

    cout << "OK" << endl;
}
