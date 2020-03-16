#pragma once

// Minimum path cover in DAG
//  https://en.wikipedia.org/wiki/Maximum_flow_problem#Minimum_path_cover_in_directed_acyclic_graph

struct MinPathCoverDAG {
    int N;
    vector<vector<int>> edges;

    vector<int> match;              // left to right
    vector<int> matchRev;           // right to left

    MinPathCoverDAG()
        : N(0) {
    }

    explicit MinPathCoverDAG(int n)
        : N(n), edges(n) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<int>>(n);
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
    }


    // return edge list
    vector<pair<int, int>> solve() {
        calcMaxMatching();

        vector<pair<int, int>> res;
        res.reserve(N);
        for (int u = 0; u < N; u++) {
            int v = match[u];
            if (v >= 0)
                res.emplace_back(u, v);
        }

        return res;
    }

    // return edge list
    vector<pair<int, int>> solveFast() {
        calcMaxMatchingFast();

        vector<pair<int, int>> res;
        res.reserve(N);
        for (int u = 0; u < N; u++) {
            int v = match[u];
            if (v >= 0)
                res.emplace_back(u, v);
        }

        return res;
    }

private:
    // maximum number of matching from srcN to dstN
    // It's faster than Kuhn's algorithm about 3x
    // Hopcroft-Karp : O(E * sqrt(V))
    int calcMaxMatching() {
        match.assign(N, -1);
        matchRev.assign(N, -1);

        int res = 0;
        while (true) {
            bfs();

            vector<bool> visited(N);

            int cnt = 0;
            for (int u = 0; u < N; u++) {
                if (match[u] < 0 && dfs(u, visited))
                    ++cnt;
            }
            if (!cnt)
                break;

            res += cnt;
        }

        return res;
    }

    int calcMaxMatchingFast() {
        match.assign(N, -1);
        matchRev.assign(N, -1);

        int res = 0;
        while (true) {
            bfs();

            vector<int> start(N, -1);

            int cnt = 0;
            for (int u = 0; u < N; u++) {
                if (match[u] < 0 && dfsFast(u, start))
                    ++cnt;
            }
            if (!cnt)
                break;

            res += cnt;
        }

        return res;
    }

    //---

    vector<int> dist;

    void bfs() {
        dist.assign(N, -1);

        queue<int> Q;
        for (int u = 0; u < N; u++) {
            if (match[u] < 0) {
                Q.push(u);
                dist[u] = 0;
            }
        }

        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (int v : edges[u]) {
                int u2 = matchRev[v];
                if (u2 >= 0 && dist[u2] < 0) {
                    dist[u2] = dist[u] + 1;
                    Q.push(u2);
                }
            }
        }
    }

    bool dfs(int u, vector<bool>& visited) {
        visited[u] = true;

        for (int v : edges[u]) {
            int u2 = matchRev[v];
            if (u2 < 0 || !visited[u2] && dist[u2] == dist[u] + 1 && dfs(u2, visited)) {
                matchRev[v] = u;
                match[u] = v;
                return true;
            }
        }
        return false;
    }

    bool dfsFast(int u, vector<int>& start) {
        for (start[u] = 0; start[u] < int(edges[u].size()); ++start[u]) {
            int v = edges[u][start[u]];
            int u2 = matchRev[v];
            if (u2 < 0 || (start[u2] < 0 && dist[u2] == dist[u] + 1 && dfsFast(u2, start))) {
                matchRev[v] = u;
                match[u] = v;
                return true;
            }
        }
        return false;
    }
};
