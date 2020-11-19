#pragma once

/*
  Maximum Weighted Matching in General Undirected Graphs (vertex and edge weighted)
  - time complexity: O((VE + V^2) * K * REPEAT)

  1. algorithm
     1) make random bipartite graph
     2) calculate MCMF
     3) repeat 1), 2)
*/
struct RandomizedGeneralMaxMatchingWithVertexAndEdgeWeight {
    template <typename T, const T INF = 0x3f3f3f3f>
    struct MinCostMaxFlow {
        struct Edge {
            int to;                 // v
            int revIndex;           // for (v -> u)

            T   flow;
            T   cost;

            T   capacity;           // capacity for flow
        };

        int N;                      // the number of vertices
        vector<vector<Edge>> edges;
        vector<bool> visited;
        vector<T> cost;

        MinCostMaxFlow() : N(0) {
        }

        explicit MinCostMaxFlow(int n) : N(n), edges(N), visited(N), cost(N) {
        }

        void init(int n) {
            N = n;
            edges = vector<vector<Edge>>(N);
            visited = vector<bool>(N);
            cost = vector<T>(N);
        }

        // add edges to a directed graph
        void addEdge(int u, int v, T capacity, T cost) {
            int uN = int(edges[u].size());
            int vN = int(edges[v].size());
            edges[u].push_back(Edge{ v, vN, 0, cost, capacity });
            edges[v].push_back(Edge{ u, uN, 0, -cost, 0 });
        }

        void clearFlow() {
            for (auto& vec : edges) {
                for (auto& e : vec)
                    e.flow = 0;
            }
        }

        // return (flow, cost), O((VE + V^2) * flow)
        pair<T, T> calcMinCostMaxFlow(int s, int t) {
            //clearFlow();
            pair<T, T> res;
            while (bfsUpdateCost(s, t)) {
                auto r = dfsUpdateFlow(s, t, INF);
                res.first += r.first;
                res.second += r.second;
            }

            return res;
        }

        // return (flow, cost), O((VE + V^2) * flow)
        pair<T, T> calcMinCostMaxFlow(int s, int t, T maxFlow) {
            //clearFlow();
            pair<T, T> res;
            while (res.first < maxFlow && bfsUpdateCost(s, t)) {
                auto r = dfsUpdateFlow(s, t, maxFlow - res.first);
                res.first += r.first;
                res.second += r.second;
            }

            return res;
        }

    private:
        // update cost
        bool bfsUpdateCost(int s, int t) {
            fill(cost.begin(), cost.end(), INF);
            fill(visited.begin(), visited.end(), false);

            cost[s] = 0;

            queue<int> Q;
            Q.push(s);
            visited[s] = true;

            while (!Q.empty()) {
                int u = Q.front();
                Q.pop();

                for (auto& e : edges[u]) {
                    if (e.flow < e.capacity && cost[e.to] > cost[u] + e.cost) {
                        cost[e.to] = cost[u] + e.cost;
                        if (!visited[e.to]) {
                            Q.push(e.to);
                            visited[e.to] = true;
                        }
                    }
                }

                visited[u] = false;
            }

            return cost[t] < INF;
        }

        // update flow and return (flow, cost)
        pair<T, T> dfsUpdateFlow(int u, int t, T flow) {
            if (u == t) {
                return make_pair(flow, flow * cost[u]);
            }

            visited[u] = true;

            pair<T, T> res;
            for (auto& e : edges[u]) {
                if (!visited[e.to] && e.flow < e.capacity && cost[e.to] == cost[u] + e.cost) {
                    auto r = dfsUpdateFlow(e.to, t, min(flow - res.first, e.capacity - e.flow));

                    res.first += r.first;
                    res.second += r.second;

                    e.flow += r.first;
                    edges[e.to][e.revIndex].flow -= r.first;

                    if (res.first == flow)
                        break;
                }
            }
            return res;
        }
    };

    /*
        <Graph type #1>

                  a
          --- (U) -- (V) ---
                \   /
                b\ /c
                  .
                 / \
                /   \
          -- (V')    (U') --

              a : (1, -(vW[u] + vW[v] - eW[u][v]))
              b : (K - 1, -(vW[u] - eW[u][v]))
              c : (K - 1, -(vW[v] - eW[u][v]))
    */
    template <typename T>
    static long long calcMaxMatching1(int N, const vector<T>& vertexWeight,
            vector<tuple<int, int, T>> edges, int K, int tryCount = 2'000) {
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

        int u, v;
        T w;
        vector<T> vertexW(N);
        for (int i = 0; i < int(edges.size()); i++) {
            tie(u, v, w) = edges[i];
            vertexW[u] = max(vertexW[u], vertexWeight[u] - w);
            vertexW[v] = max(vertexW[v], vertexWeight[v] - w);
            get<2>(edges[i]) = vertexWeight[u] + vertexWeight[v] - w;
        }
        for (int u = 0; u < N; u++)
            edges.emplace_back(u, u + N, vertexW[u]);

        //static mt19937 eng(0xCD0F);
        static mt19937 eng(chrono::steady_clock::now().time_since_epoch().count());

        long long res = 0;
        for (int t = 0; t < tryCount; t++) {
            vector<int> vertexColor(N * 2);
            for (int i = 0; i < N; i++) {
                vertexColor[i] = eng() & 1;
                vertexColor[i + N] = vertexColor[i] ^ 1;
            }
            res = max(res, calcMinCost1(N * 2, vertexColor, edges, K));
        }

        return res;
    }


    /*
        <Graph type #2>

           a      c       b
          --- (U) -- (V) ---
                \   /
                d\ /e
                  .
                 / \
           a'   /   \      b'
          -- (U')    (V') --

              a  : (1, -vW[u])
              a' : (K - 1, 0)
              b  : (1, -vW[v])
              b' : (K - 1, 0)
              c  : (1, eW[u][v])
              d  : (1, eW[u][v])
              e  : (1, eW[u][v])

              a-c-b  : -(vW[u] + vW[u] - eW[u][v])
              a-d-b' : -(vW[u] - eW[u][v])
             a'-e-b  : -(vW[v] - eW[u][v])
    */
    template <typename T>
    static long long calcMaxMatching2(int N, const vector<T>& vertexWeight,
            vector<tuple<int, int, T>> edges, int K, int tryCount = 4'000) {
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

        //static mt19937 eng(0xCD0F);
        static mt19937 eng(chrono::steady_clock::now().time_since_epoch().count());

        long long res = 0;
        for (int t = 0; t < tryCount; t++) {
            vector<int> vertexColor(N);
            for (int i = 0; i < N; i++)
                vertexColor[i] = eng() & 1;
            res = max(res, calcMinCost2(N, vertexWeight, vertexColor, edges, K));
        }

        return res;
    }

private:
    template <typename T>
    static long long calcMinCost1(int N, const vector<T>& vertexColor,
            const vector<tuple<int, int, T>>& edges, int K) {
        MinCostMaxFlow<long long, 0x3f3f3f3f3f3f3f3fll> flow;

        vector<int> A, B;
        for (int u = 0; u < N; u++) {
            if (vertexColor[u] == 0)
                A.push_back(u);
            else
                B.push_back(u);
        }

        flow.init(N + 2);
        int vS = N;
        int vT = N + 1;

        int u, v;
        T w;
        for (auto& it : edges) {
            tie(u, v, w) = it;
            if (vertexColor[u] != vertexColor[v]) {
                if (vertexColor[u] > vertexColor[v])
                    swap(u, v);

                if (u < N / 2 && v < N / 2)
                    flow.addEdge(u, v, 1, -w);
                else
                    flow.addEdge(u, v, K - 1, -w);
            }
        }

        for (auto u : A)
            flow.addEdge(vS, u, 1, 0);
        for (auto u : B)
            flow.addEdge(u, vT, 1, 0);

        return -flow.calcMinCostMaxFlow(vS, vT, K).second;
    }

    template <typename T>
    static long long calcMinCost2(int N, const vector<T>& vertexWeight, const vector<int>& vertexColor,
            const vector<tuple<int, int, T>>& edges, int K) {
        MinCostMaxFlow<long long, 0x3f3f3f3f3f3f3f3fll> flow;

        flow.init(N * 2 + 2);
        int vS = N * 2;
        int vT = N * 2 + 1;

        for (int u = 0; u < N; u++) {
            if (vertexColor[u] == 0) {
                flow.addEdge(vS, u, 1, -vertexWeight[u]);
                flow.addEdge(vS, u + N, K - 1, 0);
            } else {
                flow.addEdge(u, vT, 1, -vertexWeight[u]);
                flow.addEdge(u + N, vT, K - 1, 0);
            }
        }

        int u, v;
        T w;
        for (auto& it : edges) {
            tie(u, v, w) = it;
            if (vertexColor[u] != vertexColor[v]) {
                if (vertexColor[u] > vertexColor[v])
                    swap(u, v);

                flow.addEdge(u, v, 1, w);
                flow.addEdge(u, v + N, 1, w);
                flow.addEdge(u + N, v, 1, w);
            }
        }

        return -flow.calcMinCostMaxFlow(vS, vT, K).second;
    }
};
