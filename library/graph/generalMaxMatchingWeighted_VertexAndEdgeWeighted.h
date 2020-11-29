#pragma once

#include "generalMaxMatchingWeighted.h"
#include "../math/simplex.h"

// https://www.codechef.com/problems/SELEDGE

/*
  Maximum Weighted Matching in General Undirected Graphs (vertex and edge weighted)
*/
template <typename T, T INF = 0x3f3f3f3f>
struct GeneralMaxMatchingWithVertexAndEdgeWeight {
    // Blossom Algorithm, O(N^3)
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

    // Linear Programming (Simplex), O(N^3)
    // - from https://www.codechef.com/viewsolution/39445510
    static long long calcMaxMatchingWithSimplex(int N, const vector<T>& vertexWeight,
        vector<tuple<int, int, T>> edges, int K = numeric_limits<int>::max()) {
        sort(edges.begin(), edges.end());
        {
            int j = 0;
            for (int i = 1; i < int(edges.size()); i++) {
                if (get<0>(edges[j]) == get<0>(edges[i]) && get<1>(edges[j]) == get<1>(edges[i]))
                    continue;
                if (++j < i)
                    edges[j] = edges[i];
            }
            edges.resize(j + 1);
        }

        /*
                   0  1  2  ... M-1 M           M+N-1
            A = |  1  0  0  ...  0  0  0 ...  0  0 |        b = | 1 |
                |  0  1  0  ...  0  0  0 ...  0  0 |            | 1 |
                |  0  0  1  ...  0  0  0 ...  0  0 |            | 1 |
                |   ...     ...     ...     ...    |            |...|
                |  0  0  0  ...  1  0  0 ...  0  0 |            | 1 |
                |  0  0  0  ...  0  1  0 ...  0  0 |            | 1 |
                |  0  0  0  ...  0  0  1 ...  0  0 |            | 1 |
                |   ...     ...     ...     ...    |            |...|
                |  0  0  0  ...  0  0  0 ...  1  0 |            | 1 |
                |  0  0  0  ...  0  0  0 ...  0  1 |M+N-1       | 1 |
                ------------------------------------            -----
                |  1  0  0  ...  0 -M  0 ...  0  0 |            | 0 |
                |  0  1  0  ...  0  0 -M ...  0  0 |            | 0 |
                |   ...     ...     ...     ...    |            |...|
                |  0  1  0  ...  0  0  0 ... -M  0 |            | 0 |
                |  1  0  0  ...  0  0  0 ...  0 -M |M+2*N-1     | 0 |
                ------------------------------------            -----
                | -1  0  0  ...  0  1  0 ...  0  0 |            | 0 |
                |  0 -1  0  ...  0  0  1 ...  0  0 |            | 0 |
                |   ...     ...     ...     ...    |            |...|
                |  0 -1  0  ...  0  0  0 ...  1  0 |            | 0 |
                | -1  0  0  ...  0  0  0 ...  0  1 |M+3*N-1     | 0 |
                ------------------------------------            -----
                |  1  1  1  ...  1  0  0 ...  0  0 |            | K |

                A[M + N + u][edgeIndex] = 1
                A[M + N + v][edgeIndex] = 1
                A[M + 2*N + u][edgeIndex] = -1
                A[M + 2*N + v][edgeIndex] = -1

                     0      1      2  ...     M-1     M                      M+N-1
            c = |-eW[0] -eW[1] -eW[2] ... -eW[M-1] vW[0] vW[1] ... vW[N-2] vW[N-1] |
        */

        int M = int(edges.size());
        vector<vector<double>> A(M + 3 * N + 1, vector<double>(M + N));
        vector<double> b(M + 3 * N + 1);
        vector<double> c(M + N);
        vector<double> x;
        for (int i = 0; i < M; i++)
            c[i] = -get<2>(edges[i]);
        for (int u = 0; u < N; u++)
            c[M + u] = vertexWeight[u];
        for (int i = 0; i < M + N; i++) {
            A[i][i] = 1;
            b[i] = 1;
        }
        for (int i = 0; i < N; i++) {
            A[M + N + i][M + i] = -M;
            A[M + 2 * N + i][M + i] = 1;
        }
        int u, v, w;
        for (int i = 0; i < M; i++) {
            tie(u, v, w) = edges[i];
            A[M + N + u][i] = 1;
            A[M + N + v][i] = 1;
            A[M + 2 * N + u][i] = -1;
            A[M + 2 * N + v][i] = -1;
        }
        for (int i = 0; i < M; i++)
            A[M + 3 * N][i] = 1;
        b[M + 3 * N] = min(K, M);

        double val = Simplex(A, b, c).solve(x);

        auto fractionalIndex = [](const vector<double>& x, int M) -> int {
            for (int i = M - 1; i >= 0; i--) {
                if (0.01 <= x[i] && x[i] <= 0.99)
                    return i;
            }
            return -1;
        };
        int start = fractionalIndex(x, M);
        if (start == -1)
            return static_cast<long long>(val + 0.1);

        function<long long(int, long long)> search = [&](int start, long long best) -> long long {
            // set edge i to 0
            A.push_back(vector<double>(M + N));
            A.back()[start] = 1;
            b.push_back(0);
            vector<double> x;
            long long r = static_cast<long long>(Simplex(A, b, c).solve(x) + 0.1);
            if (best < 0 || r >= best) {
                int j = fractionalIndex(x, M);
                if (j == -1)
                    best = r;
                else
                    best = search(j, best);
            }
            // set edge i to 1
            A.back()[start] = -1;
            b.back() = -1;
            x.clear();
            r = static_cast<long long>(Simplex(A, b, c).solve(x) + 0.1);
            if (best < 0 || r >= best) {
                int j = fractionalIndex(x, M);
                if (j == -1)
                    best = r;
                else
                    best = search(j, best);
            }
            A.pop_back();
            b.pop_back();
            return best;
        };
        return search(start, 0ll);
    }

    // Blossom Algorithm, O(N^3)
    static long long calcMaxPerfectMatching(int N, const vector<T>& vertexWeight, vector<tuple<int, int, T>> edges) {
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
