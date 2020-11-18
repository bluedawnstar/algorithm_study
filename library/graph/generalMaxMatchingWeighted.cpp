#include <assert.h>
#include <tuple>
#include <numeric>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#include "generalMaxMatchingWeighted.h"
#include "generalMaxMatchingWeighted2.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <unordered_set>
#include <map>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

/*
  <Related problems>
  - https://www.codechef.com/problems/SELEDGE
*/

static long long solve1(int N, const vector<int>& vertexWeight, const vector<tuple<int,int,int>>& uv) {
    int u, v, w;

    vector<int> vertexW(N);
    map<pair<int, int>, long long> edgeM;
    for (int i = 0; i < int(uv.size()); i++) {
        tie(u, v, w) = uv[i];
        if (u > v)
            swap(u, v);

        long long cost = vertexWeight[u] + vertexWeight[v] - w;
        edgeM[make_pair(u, v)] = max(cost, edgeM[make_pair(u, v)]);

        vertexW[u] = max(vertexW[u], vertexWeight[u] - w);
        vertexW[v] = max(vertexW[v], vertexWeight[v] - w);
    }

    vector<tuple<int, int, long long>> edges;
    for (auto& it : edgeM) {
        edges.emplace_back(it.first.first, it.first.second, it.second);
    }

    for (int u = 0; u < N; u++) {
        if (vertexW[u] > 0) {
            edges.emplace_back(u, u + N, vertexW[u]);
        }
    }

    auto res = WeightedGeneralMaxMatching2<long long>(N * 2, edges).calcMaxWeightedMatching();
    return res.first;
}

static long long solve2(int N, const vector<int>& vertexWeight, const vector<tuple<int, int, int>>& uv) {
    WeightedGeneralMaxMatching<long long, 0x3f3f3f3f3f3f3f3fll> flow;
    flow.init(N * 2);

    vector<int> vertexW(N);

    int u, v, w;
    for (int i = 0; i < int(uv.size()); i++) {
        tie(u, v, w) = uv[i];

        long long cost = vertexWeight[u] + vertexWeight[v] - w;
        flow.addEdge(u, v, max(cost, flow.getEdge(u, v).w));

        vertexW[u] = max(vertexW[u], vertexWeight[u] - w);
        vertexW[v] = max(vertexW[v], vertexWeight[v] - w);
    }

    for (int u = 0; u < N; u++) {
        if (vertexW[u] > 0)
            flow.addEdge(u, u + N, vertexW[u]);
    }

    auto res = flow.calcMaxMatching();
    return res.first;
}

static long long solve2(int N, const vector<int>& vertexWeight, const vector<tuple<int, int, int>>& uv, int K) {
    WeightedGeneralMaxMatching<long long, 0x3f3f3f3f3f3f3f3fll> flow;
    flow.init(N * 2);

    vector<int> vertexW(N);

    int u, v, w;
    for (int i = 0; i < int(uv.size()); i++) {
        tie(u, v, w) = uv[i];

        long long cost = vertexWeight[u] + vertexWeight[v] - w;
        flow.addEdge(u, v, max(cost, flow.getEdge(u, v).w));

        vertexW[u] = max(vertexW[u], vertexWeight[u] - w);
        vertexW[v] = max(vertexW[v], vertexWeight[v] - w);
    }

    for (int u = 0; u < N; u++) {
        if (vertexW[u] > 0)
            flow.addEdge(u, u + N, vertexW[u]);
    }

    auto res = flow.calcMaxMatching(K);
    return res.first;
}

void testGeneralMaxMatchingWeighted() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Blossom Algorithm for Weighted General Matching -------------" << endl;
    {
        // https://www.codechef.com/problems/SELEDGE
        int N = 5;
        int K = 2;
        vector<int> vertexWeight{
            1, 2, 4, 8, 16
        };
        vector<tuple<int, int, int>> edges{
            { 0, 1, 1 },
            { 0, 2, 2 },
            { 2, 3, 2 },
            { 3, 4, 2 },
            { 1, 4, 1 }
        };

        auto ans1 = solve1(N, vertexWeight, edges);
        auto ans2 = solve2(N, vertexWeight, edges);
        auto ans2k = solve2(N, vertexWeight, edges, K);
        assert(ans1 == 27 && ans2 == 27 && ans2k == 27);
    }

    cout << "OK" << endl;
}
