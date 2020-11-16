#pragma once

#include "../flowNetwork/maxFlowDinic.h"

/*
    SUM VertexWeight = MVC + MIS
    MVC = MC

        MC  : Maximum Matching
        MVC : Minimum Vertex Cover
        MIS : Maximum Independent Set
*/
template <typename T, const T INF = 0x3f3f3f3f>
struct BipartiteWeightedMatching {
    // return minimum vertex cover (or maximum matching)
    // - edges is an undirected graph
    // - color[u] is in { 0, 1 } , 0 : red, 1 : blue
    // O(min(V^2 * E, E * MAX_WEIGHT))
    static T solve(const vector<vector<int>>& edges, const vector<T>& vertexWeight, const vector<bool>& vertexColor) {
        int N = int(edges.size());

        MaxFlowDinic<T,INF> flow;

        vector<int> A, B;
        for (int u = 0; u < N; u++) {
            if (!vertexColor[u])
                A.push_back(u);
            else
                B.push_back(u);
        }

        flow.init(N + 2);
        int vS = N;
        int vT = vS + 1;

        for (int u = 0; u < N; u++) {
            for (int v : edges[u]) {
                if (u > v)
                    continue;
                int a = u;
                int b = v;
                if (vertexColor[a])
                    swap(a, b);
                flow.addEdge(a, b, INF, 0);
            }
        }

        for (auto u : A)
            flow.addEdge(vS, u, vertexWeight[u], 0);

        for (auto u : B)
            flow.addEdge(u, vT, vertexWeight[u], 0);

        return flow.calcMaxFlow(vS, vT);
    }

    // return minimum vertex cover (or maximum matching)
    // - edges : an undirected graph
    // - color : color[u] is in { 0, 1 } , 0 : red, 1 : blue
    // - oppositePairs : pairs to have opposite colors and not to be included in edges
    // O(min(V^2 * E, E * MAX_WEIGHT))
    static T solveWithOppositePairs(const vector<vector<int>>& edges, const vector<T>& vertexWeight, const vector<bool>& vertexColor,
            const vector<pair<int,int>>& oppositePairs) {
        int N = int(edges.size());

        MaxFlowDinic<T, INF> flow;

        vector<int> A, B;
        for (int u = 0; u < N; u++) {
            if (!vertexColor[u])
                A.push_back(u);
            else
                B.push_back(u);
        }

        flow.init(N + 2);
        int vS = N;
        int vT = vS + 1;

        for (int u = 0; u < N; u++) {
            for (int v : edges[u]) {
                if (u > v)
                    continue;
                int a = u;
                int b = v;
                if (vertexColor[a])
                    swap(a, b);
                flow.addEdge(a, b, INF, 0);
            }
        }

        // opposite pairs
        for (auto& it : oppositePairs) {
            int u = it.first;
            int v = it.second;
            if (vertexColor[u])
                swap(u, v);

            flow.addEdge(v, u, INF, 0);
        }

        for (auto u : A)
            flow.addEdge(vS, u, vertexWeight[u], 0);

        for (auto u : B)
            flow.addEdge(u, vT, vertexWeight[u], 0);

        return flow.calcMaxFlow(vS, vT);
    }
};
