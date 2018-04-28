#include <climits>
#include <numeric>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

#include "globalMinCut.h"
#include "globalMinCutNaive.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// https://www.hackerrank.com/contests/w37/challenges/two-efficient-teams/problem

static void makeData(int N, int M, int maxCost, vector<int>& inCost, vector<vector<int>>& inGroup) {
    inCost.assign(M, 0);
    inGroup.assign(M, vector<int>());

    for (int i = 0; i < M; i++) {
        inCost[i] = RandInt32::get() % maxCost + 1;

        int k = 2 + (RandInt32::get() % 2);
        inGroup[i].reserve(k);

        int a = -1, b = -1;
        for (int j = 0; j < k; j++) {
            int c;
            do {
                c = RandInt32::get() % N;
            } while (c == i || c == a || c == b);
            inGroup[i].push_back(c);

            b = a;
            a = c;
        }
    }
}


namespace GlobalMinCut_Naive {
    vector<unsigned long long> groupMask;
    
    int total = 0;
    int ans = 0;

    void dfsSolve(int N, int M, const vector<int>& inCost, unsigned long long setA, unsigned long long setB, int level) {
        int c = total;
        for (int g = 0; g < M; g++) {
            if ((groupMask[g] & setA) && (groupMask[g] & setB))
                c -= inCost[g];
        }

        if (c <= ans)
            return;

        if (level >= N) {
            if (setA && setB)
                ans = c;
        } else {
            dfsSolve(N, M, inCost, setA | (1ull << level), setB, level + 1);
            dfsSolve(N, M, inCost, setA, setB | (1ull << level), level + 1);
        }
    }

    int solve(int N, int M, const vector<int>& inCost, const vector<vector<int>>& inGroup) {
        groupMask.assign(M, 0ull);

        vector<int> ecost(N, 0);

        ans = 0;
        total = 0;

        for (int i = 0; i < M; i++) {
            total += inCost[i];
            for (int e : inGroup[i]) {
                groupMask[i] |= 1ull << e;
                ecost[e] += inCost[i];
            }
        }

        ans = total - *min_element(ecost.begin(), ecost.end());
        dfsSolve(N, M, inCost, 1ull, 0ull, 1);

        return ans;
    }
}

static int solve(int N, int M, const vector<int>& inCost, const vector<vector<int>>& inGroup) {
    GlobalMinCut<int> graph(N);

    int total = 0;
    for (int i = 0; i < M; i++) {
        auto& v = inGroup[i];

        int k = (int)v.size();
        total += inCost[i];

        if (k == 2)
            graph.addEdge(v[0], v[1], inCost[i] * 2);
        else {
            graph.addEdge(v[0], v[1], inCost[i]);
            graph.addEdge(v[1], v[2], inCost[i]);
            graph.addEdge(v[2], v[0], inCost[i]);
        }
    }

    vector<int> cutGroup;
    int cut = graph.doStoerWagner(cutGroup);
    return total - cut / 2;
}

void testGlobalMinCut() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Global Minimum Cut -----------------------" << endl;

    // https://www.hackerrank.com/contests/w37/challenges/two-efficient-teams/problem
    cout << "Step 1 : solve HackerRank solution" << endl;
    {
        int T = 50;
        int N = 40;
        int M = 80;
        int MaxCost = 1000;

#ifdef _DEBUG
        T = 10;
#endif

        for (int i = 0; i < T; i++) {
            vector<int> inCost;
            vector<vector<int>> inGroup;
            makeData(N, M, MaxCost, inCost, inGroup);

            auto ans1 = solve(N, M, inCost, inGroup);
            auto ans2 = GlobalMinCut_Naive::solve(N, M, inCost, inGroup);
            if (ans1 != ans2)
                cout << "Mismatch: " << ans1 << ", " << ans2 << endl;
            assert(ans1 == ans2);
        }
    }
    cout << "Step 2 : solve random graphs" << endl;
    {
        int T = 100;
        int N = 50;
        int E = N * 10;
        int MaxCost = 1000;

        for (int i = 0; i < T; i++) {
            GlobalMinCut<int> graph1(N);
            GlobalMinCutNaive64<int> graph2(N);
            for (int j = 0; j < E; j++) {
                int w = rand() % MaxCost + 1;
                int u = rand() % N;
                int v = rand() % N;
                while (u == v)
                    v = rand() % N;

                graph1.addEdge(u, v, w);
                graph2.addEdge(u, v, w);
            }

            vector<int> group1;
            auto ans1 = graph1.doStoerWagner(group1);
            auto ans2 = graph2.solve();
            if (ans1 != ans2.first)
                cout << "Mismatch: " << ans1 << ", " << ans2.first << endl;
            assert(ans1 == ans2.first);
        }
    }
    cout << "Step 3 : speed test" << endl;
    {
        //int T = 100;
        int T = 1;
        int N = 64;
        int E = N * N;
        int MaxCost = 10000;

        {
            GlobalMinCut<int> graph1(N);
            GlobalMinCutNaive64<int> graph2(N);
            for (int j = 0; j < E; j++) {
                int w = rand() % MaxCost + 1;
                int u = rand() % N;
                int v = rand() % N;
                while (u == v)
                    v = rand() % N;

                graph1.addEdge(u, v, w);
                graph2.addEdge(u, v, w);
            }

            vector<int> group1;
            int ans1 = 0;
            PROFILE_HI_START(0);
            for (int i = 0; i < T; i++)
                ans1 += graph1.doStoerWagner(group1);
            PROFILE_HI_STOP(0);

            int ans2 = 0;
            PROFILE_HI_START(1);
            graph2.mergeEdges();
            for (int i = 0; i < T; i++)
                ans2 += graph2.solve().first;
            PROFILE_HI_STOP(1);

            if (ans1 != ans2)
                cout << "Mismatch: " << ans1 << ", " << ans2 << endl;
            assert(ans1 == ans2);
        }
    }
    cout << "Step 4 : speed test" << endl;
    {
        //int T = 100;
        int T = 1;
        int N = 160;
        int E = N * N;
        int MaxCost = 10000;

        {
            GlobalMinCut<int> graph1(N);
            GlobalMinCutNaive<int> graph2(N);
            for (int j = 0; j < E; j++) {
                int w = rand() % MaxCost + 1;
                int u = rand() % N;
                int v = rand() % N;
                while (u == v)
                    v = rand() % N;

                graph1.addEdge(u, v, w);
                graph2.addEdge(u, v, w);
            }

            vector<int> group1;
            int ans1 = 0;
            PROFILE_HI_START(0);
            for (int i = 0; i < T; i++)
                ans1 += graph1.doStoerWagner(group1);
            PROFILE_HI_STOP(0);

            int ans2 = 0;
            PROFILE_HI_START(1);
            graph2.mergeEdges();
            for (int i = 0; i < T; i++)
                ans2 += graph2.solve();
            PROFILE_HI_STOP(1);

            if (ans1 != ans2)
                cout << "Mismatch: " << ans1 << ", " << ans2 << endl;
            assert(ans1 == ans2);
        }
    }

    cout << "OK" << endl;
}
