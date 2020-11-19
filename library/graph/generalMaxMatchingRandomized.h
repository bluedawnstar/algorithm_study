#pragma once

/*
  Randomized Algorithm - general matching for undirected graph
  - O(V^3)

  <Ref>
  - https://uoj.ac/submission/233938
*/
struct RandomizedGeneralMaxMatching {
    int N;
    vector<vector<int>> edges;

    RandomizedGeneralMaxMatching() : N(0) {
    }

    RandomizedGeneralMaxMatching(int n) : N(n), edges(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<int>>(N);
    }

    // add edges to a undirected graph
    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    // Randomized Algorithm : O(N * tryCount)
    int calcMaxMatching(int tryCount = 10) {
        link = vector<int>(N, -1);
        visitTime = vector<int>(N);
        currTime = 0;

        int res = 0;
        for (int t = 0; t < tryCount; t++) {
            for (int i = 0; i < N; i++) {
                if (link[i] < 0) {
                    currTime++;
                    res += dfs(i);
                }
            }
        }

        return res;
    }

    vector<pair<int, int>> getLastMaxMatchingEdges() {
        vector<pair<int, int>> res;
        for (int u = 0; u < N; u++) {
            if (u < link[u])
                res.emplace_back(u, link[u]);
        }
        return res;
    }

private:
    vector<int> link;
    vector<int> visitTime;
    int currTime;

    bool dfs(int x) {
        visitTime[x] = currTime;

        random_shuffle(edges[x].begin(), edges[x].end());
        for (int u : edges[x]) {
            int v = link[u];
            if (v < 0) {
                link[x] = u; link[u] = x;
                return true;
            } else if (visitTime[v] < currTime) {
                link[x] = u; link[u] = x; link[v] = -1;
                if (dfs(v))
                    return true;
                link[u] = v; link[v] = u; link[x] = -1;
            }
        }
        return false;
    }
};
