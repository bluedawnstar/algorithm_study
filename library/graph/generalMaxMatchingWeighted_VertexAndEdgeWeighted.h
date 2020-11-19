#pragma once

#include "generalMaxMatchingWeighted.h"

// https://www.codechef.com/problems/SELEDGE

/*
  Maximum Weighted Matching in General Undirected Graphs (vertex and edge weighted)
  - Blossom Algorithm, O(N^3)
*/
template <typename T, T INF = 0x3f3f3f3f>
struct GeneralMaxMatchingWithVertexAndEdgeWeight {
    static long long calcMaxMatching(int N, const vector<T>& vertexWeight,
            const vector<tuple<int, int, T>>& edges, int K = numeric_limits<int>::max()) {
        /*
                   a
          --- (U) --- (V) ---
               |       |
               |b      |c
               |       |
              (U')    (V')

              a : vW[u] + vW[v] - eW[u][v]
              b : vW[u] - eW[u][v]
              c : vW[v] - eW[u][v]
        */

        WeightedGeneralMaxMatching<T, INF> flow;
        flow.init(N * 2);

        vector<int> vertexW(N);
        int u, v;
        T w;
        for (int i = 0; i < int(edges.size()); i++) {
            tie(u, v, w) = edges[i];

            T cost = vertexWeight[u] + vertexWeight[v] - w;
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
};

template <typename T, T INF = 0x3f3f3f3f>
struct GeneralMaxMatchingWithVertexAndEdgeWeight2 {
    static long long calcMaxMatching(int N, const vector<T>& vertexWeight, vector<tuple<int, int, T>> edges) {
        /*
                   a
          --- (U) --- (V) ---
               |       |
               |b      |c
               |       |
              (U')    (V')

              a : vW[u] + vW[v] - eW[u][v]
              b : vW[u] - eW[u][v]
              c : vW[v] - eW[u][v]
        */

        vector<T> vertexW(N);
        map<pair<int, int>, T> edgeM;

        int u, v;
        T w;
        for (int i = 0; i < int(edges.size()); i++) {
            tie(u, v, w) = edges[i];
            if (u > v)
                swap(u, v);

            T cost = vertexWeight[u] + vertexWeight[v] - w;
            edgeM[make_pair(u, v)] = max(cost, edgeM[make_pair(u, v)]);

            vertexW[u] = max(vertexW[u], vertexWeight[u] - w);
            vertexW[v] = max(vertexW[v], vertexWeight[v] - w);
        }

        edges.clear();
        for (auto& it : edgeM) {
            edges.emplace_back(it.first.first, it.first.second, it.second);
        }
        for (int u = 0; u < N; u++) {
            if (vertexW[u] > 0) {
                edges.emplace_back(u, u + N, vertexW[u]);
            }
        }

        auto res = WeightedGeneralMaxMatching2<T>(N * 2, edges).calcMaxWeightedMatching();
        return res.first;
    }
};
