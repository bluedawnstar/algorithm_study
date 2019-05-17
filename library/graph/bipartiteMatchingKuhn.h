#pragma once

/*
    |V| = [MVC] + |MIS|

       |V| = the number of vertex
       MVC = Min Vertex Cover
       MIS = Maximum 
*/
struct BipartiteMatchingKuhn {
    int srcN;
    int dstN;
    vector<vector<int>> edges;

    vector<int> match;              // left to right
    vector<int> matchRev;           // right to left

    BipartiteMatchingKuhn()
        : srcN(0), dstN(0) {
    }

    BipartiteMatchingKuhn(int _srcN, int _dstN)
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
    // Kuhn's algorithm : O(V * E)
    int calcMaxMatching() {
        match.assign(srcN, -1);
        matchRev.assign(dstN, -1);

        int res = 0;
        for (int u = 0; u < srcN; u++) {
            vector<bool> visited(srcN);
            if (dfs(u, visited))
                res++;
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
    // return true if a matching for vertex u is possible
    bool dfs(int u, vector<bool>& visited) {
        if (visited[u])
            return false;
        visited[u] = true;

        for (int v : edges[u]) {
            if (matchRev[v] < 0 || dfs(matchRev[v], visited)) {
                matchRev[v] = u;
                match[u] = v;
                return true;
            }
        }
        return false;
    }
};
