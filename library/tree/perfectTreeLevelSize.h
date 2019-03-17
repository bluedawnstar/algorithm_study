#pragma once

struct PerfectTreeLevelSize {
    int N;
    vector<vector<int>> edges;

    vector<int> parent;
    vector<int> size;
    vector<int> depth;
    vector<int> subDepth;
    int maxDepth;

    void build(const vector<vector<int>>& edges) {
        N = int(edges.size());
        this->edges = edges;

        parent = vector<int>(N);
        size = vector<int>(N);
        depth = vector<int>(N);
        subDepth = vector<int>(N);
        visited = vector<bool>(N);

        maxDepth = 0;
        dfsBuild(0, -1, 0);

        levelSize = vector<vector<pair<int, int>>>(N);
        levelDelta = vector<int>(N + 1);

        mapN = 1;
        maps = vector<map<int,int>>(N);
    }

    // { (size, depth_height), ... }, except u
    const vector<pair<int,int>>& calcSubLevelSize(int u) {
        if (size[u] == 1)
            return levelSize[u];

        if (!levelSize[u].empty())
            return levelSize[u];

        dfsD(u, parent[u]);

        int d = depth[u] + 1;
        int n = (u == 0) ? int(edges[u].size()) : int(edges[u].size()) - 1;

        auto& M = maps[levelDelta[u]];
        levelSize[u].reserve(M.size() + 1);

        for (auto it : M) {
            levelSize[u].emplace_back(n, it.first - d);
            d = it.first;
            n += it.second;
        }

        levelSize[u].emplace_back(n, subDepth[u] + 1 - d);

        return levelSize[u];
    }

private:
    void dfsBuild(int u, int p, int d) {
        depth[u] = d;
        parent[u] = p;
        size[u] = 1;

        subDepth[u] = d;
        for (int v : edges[u]) {
            if (v != p) {
                dfsBuild(v, u, d + 1);
                size[u] += size[v];
                subDepth[u] = max(subDepth[u], subDepth[v]);
            }
        }

        maxDepth = max(maxDepth, d);
    }

    //----

    vector<vector<pair<int,int>>> levelSize;
    vector<int> levelDelta;

    vector<bool> visited;

    int mapN;
    vector<map<int,int>> maps;

    void dfsD(int u, int parent) {
        if (visited[u])
            return;
        visited[u] = true;

        int childN = (u == 0) ? int(edges[u].size()) : int(edges[u].size()) - 1;

        if (childN == 0) {
            levelDelta[u] = 0;
            return;
        } else if (childN == 1) {
            int v = (edges[u][0] != parent) ? edges[u][0] : edges[u][1];
            if (edges[v].size() <= 2) {
                dfsD(v, u);
                levelDelta[u] = levelDelta[v];
                return;
            }
        }

        levelDelta[u] = mapN++;
        auto& M = maps[levelDelta[u]];

        for (auto v : edges[u]) {
            if (v != parent) {
                dfsD(v, u);

                if (M.empty()) {
                    M = maps[levelDelta[v]];
                } else {
                    for (auto it : maps[levelDelta[v]])
                        M[it.first] += it.second;
                }

                if (edges[v].size() > 2)
                    M[depth[v] + 1] += int(edges[v].size()) - 2;
            }
        }
    }
};
