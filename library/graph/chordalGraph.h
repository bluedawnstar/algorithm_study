#pragma once

/*
    https://en.wikipedia.org/wiki/Chordal_graph
    http://www.secmem.org/blog/2019/03/10/Finding-perfect-elimination-ordering-in-choral-graphs/

  <Chordal Graph>
    - one in which all cycles of four or more vertices have a chord,
      which is an edge that is not part of the cycle but connects two vertices of the cycle
    - every induced cycle in the graph should have exactly three vertices

  1. Perfect Elimination Ordering
    - an ordering of the vertices of the graph such that, for each vertex v,
      v and the neighbors of v that occur after v in the order form a clique
*/

// undirected graph
struct ChordalGraph {
    int N;
    vector<vector<pair<int, int>>> edges;
    int edgeCount;

    ChordalGraph() : N(0), edgeCount(0) {
    }

    explicit ChordalGraph(int n) : N(n), edges(N), edgeCount(0) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<pair<int, int>>>(N);
        edgeCount = 0;
    }

    void addEdge(int u, int v) {
        edges[u].emplace_back(v, edgeCount);
        edges[v].emplace_back(u, edgeCount);
        edgeCount++;
    }


    // O(E*sqrt(E))
    vector<int> findPerfectEliminationOrdering() {
        vector<long long> C(N);
        // C[u] = the number of all pairs among neighbors - the number of 3-cycles
        {
            for (int u = 0; u < N; u++) {
                if (!edges[u].empty())
                    sort(edges[u].begin(), edges[u].end());
                int sz = int(edges[u].size());
                C[u] = 1ll * sz * (sz - 1) / 2;
            }

            vector<bool> neighbor(N);
            for (int u = 0; u < N; u++) {
                for (auto& x : edges[u])
                    neighbor[x.first] = true;

                for (auto& x : edges[u]) {
                    if ((edges[x.first].size() < edges[u].size()) ||
                        (edges[x.first].size() == edges[u].size() && x.first < u)) {
                        for (auto& y : edges[x.first]) {
                            if (neighbor[y.first])
                                C[y.first]--;
                        }
                    }
                }

                for (auto& x : edges[u])
                    neighbor[x.first] = false;
            }
        }

        queue<int> Q;
        vector<bool> vis(N);

        vector<bool> edgeErased(edgeCount);
        vector<int> erasedCnt(N);

        for (int u = 0; u < N; u++) {
            if (C[u] == 0) {
                Q.push(u);
                vis[u] = true;
            }
        }

        vector<int> res;

        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            res.push_back(u);

            int sz = int(edges[u].size()) - erasedCnt[u];
            for (auto& it : edges[u]) {
                if (edgeErased[it.second])
                    continue;
                edgeErased[it.second] = true;

                int v = it.first;
                C[v] += (sz - 1) - (int(edges[v].size()) - ++erasedCnt[v]);
                if (C[v] == 0 && !vis[v]) {
                    vis[v] = true;
                    Q.push(v);
                }
            }
        }

        if (int(res.size()) != N)
            res.clear();

        return res;
    }
};
