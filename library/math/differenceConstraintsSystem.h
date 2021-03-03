#pragma once

/*
    1. Systems of Inequalities (system of difference constraints)

       Path p = (v1, v2, ..., vn)

          x[2] - x[1] <= w[1,2]
          x[3] - x[2] <= w[2,3]
          ...
          x[n] - x[n-1] <= w[n-1,n]

      <=>
                          n
          x[n] - x[1] <= SUM { w[i-1,i] } = w(p)
                         i=2

      ==>

          x[n] - x[1] <= (the weight of the shortest path from v1 to vn)
*/
// Constraint Graph
struct DifferenceConstraintsSystem {
    const int INF = 0x7f7f7f7f;

    int N;
    vector<vector<pair<int, int>>> edges;   // edges[u] = { (v, w), ... }

    DifferenceConstraintsSystem() {
    }

    explicit DifferenceConstraintsSystem(int n) {
        init(n);
    }

    void init(int n) {
        N = n;
        edges = vector<vector<pair<int, int>>>(n);
    }

    // adds a difference constraint
    //  :  v - u <= maxW  (u --> v)
    void add(int u, int v, int maxW) {
        edges[u].emplace_back(v, maxW);
    }

    // adds a difference constraint
    //  :  minW <= v - u <= maxW  (u --> v)
    void add(int u, int v, int minW, int maxW) {
        edges[u].emplace_back(v, maxW);
        edges[v].emplace_back(u, -minW);
    }

    // returns 1) the solution to the constraint system
    //         2) or an empty vector if there is none
    // 
    // - result[i] = X[i] - X[start]
    vector<int> solve(int start) {
        return bellmanFord(start);
    }

private:
    vector<int> bellmanFord(int start) {
        vector<int> dist(N, INF);
        dist[start] = 0;

        for (bool updated = true; updated; ) {
            if (dist[start] != 0)
                return {};  // bad case (negative cycle)

            updated = false;
            for (int u = 0; u < N; u++) {
                for (auto it : edges[u]) {
                    int v = it.first;
                    int w = it.second;
                    if (dist[v] > dist[u] + w) {
                        updated = true;
                        dist[v] = dist[u] + w;
                    }
                }
            }
        }

        return dist;
    }
};
