#pragma once

/*
    |V| = [MVC] + |MIS|
    |MVC| = |MC|

        |V| : the number of vertex
        MC  : Maximum Matching
        MVC : Minimum Vertex Cover
        MIS : Maximum Independent Set 
*/
struct BipartiteMatchingHopcroftKarp {
    int srcN;
    int dstN;
    vector<vector<int>> edges;

    vector<int> match;              // left to right
    vector<int> matchRev;           // right to left

    BipartiteMatchingHopcroftKarp()
        : srcN(0), dstN(0) {
    }

    BipartiteMatchingHopcroftKarp(int _srcN, int _dstN)
        : srcN(_srcN), dstN(_dstN), edges(srcN) {
    }

    void init(int _srcN, int _dstN) {
        srcN = _srcN;
        dstN = _dstN;
        edges = vector<vector<int>>(srcN);
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
    }

    // maximum number of matching from srcN to dstN
    // It's faster than Kuhn's algorithm about 3x
    // Hopcroft-Karp : O(E * sqrt(V))
    int calcMaxMatching() {
        match.assign(srcN, -1);
        matchRev.assign(dstN, -1);

        int res = 0;
        while (true) {
            bfs();

            vector<bool> visited(srcN);

            int cnt = 0;
            for (int u = 0; u < srcN; u++) {
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
        match.assign(srcN, -1);
        matchRev.assign(dstN, -1);

        int res = 0;
        while (true) {
            bfs();

            vector<int> start(srcN, -1);

            int cnt = 0;
            for (int u = 0; u < srcN; u++) {
                if (match[u] < 0 && dfsFast(u, start))
                    ++cnt;
            }
            if (!cnt)
                break;

            res += cnt;
        }

        return res;
    }

    //--- Min Vertex Cover ----------------------------------------------------

    // Must call this function after calcMaxMatching()
    // return (chosen A, chosen B), O(V*E)
    pair<vector<bool>, vector<bool>> minVertexCover() {
        pair<vector<bool>, vector<bool>> res;
        res.first.assign(srcN, false);
        res.second.assign(dstN, true);

        for (int v = 0; v < dstN; v++)
            if (matchRev[v] < 0)
                res.second[v] = false;

        while (true) {
            bool changed = false;
            for (int u = 0; u < srcN; u++) {
                if (res.first[u])
                    continue;
                for (int v : edges[u]) {
                    if (!res.second[v]) {
                        res.first[u] = true;
                        res.second[match[u]] = false;
                        changed = true;
                    }
                }
            }
            if (!changed)
                break;
        }

        return res;
    }

    pair<vector<int>, vector<int>> minVertexCoverIndexes() {
        pair<vector<int>, vector<int>> res;

        auto r = minVertexCover();
        for (int i = 0; i < int(r.first.size()); i++) {
            if (r.first[i])
                res.first.push_back(i);
        }
        for (int i = 0; i < int(r.second.size()); i++) {
            if (r.second[i])
                res.second.push_back(i);
        }

        return res;
    }

    //--- Max Independent Set -------------------------------------------------

    // Must call this function after calcMaxMatching()
    // return (chosen A, chosen B), O(V*E)
    pair<vector<bool>, vector<bool>> maxIndependentSet() {
        pair<vector<bool>, vector<bool>> res;
        res.first.assign(srcN, true);
        res.second.assign(dstN, false);

        for (int v = 0; v < dstN; v++)
            if (matchRev[v] < 0)
                res.second[v] = true;

        while (true) {
            bool changed = false;
            for (int u = 0; u < srcN; u++) {
                if (!res.first[u])
                    continue;
                for (int v : edges[u]) {
                    if (res.second[v]) {
                        res.first[u] = false;
                        res.second[match[u]] = true;
                        changed = true;
                    }
                }
            }
            if (!changed)
                break;
        }

        return res;
    }

    pair<vector<int>, vector<int>> maxIndependentSetIndexes() {
        pair<vector<int>, vector<int>> res;

        auto r = maxIndependentSet();
        for (int i = 0; i < int(r.first.size()); i++) {
            if (r.first[i])
                res.first.push_back(i);
        }
        for (int i = 0; i < int(r.second.size()); i++) {
            if (r.second[i])
                res.second.push_back(i);
        }

        return res;
    }

private:
    vector<int> dist;

    void bfs() {
        dist.assign(srcN, -1);

        queue<int> Q;
        for (int u = 0; u < srcN; u++) {
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
