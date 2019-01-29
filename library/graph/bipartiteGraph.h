#pragma once

struct BipartiteGraph {
    int srcN;
    int dstN;
    vector<vector<int>> edges;

    BipartiteGraph() : srcN(0), dstN(0) {
    }

    BipartiteGraph(int _srcN, int _dstN) : srcN(_srcN), dstN(_dstN), edges(srcN) {
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
            if (dfsKuhn(u, visited))
                res++;
        }
        return res;
    }

    // maximum number of matching from srcN to dstN
    // It's faster than Kuhn's algorithm about 3x
    // Hopcroft-Karp : O(E * sqrt(V))
    int calcMaxMatchingHopcroftKarp() {
        match.assign(srcN, -1);
        matchRev.assign(dstN, -1);

        int res = 0;
        while (true) {
            bfsHopcroftKarp();

            vector<bool> visited(srcN);

            int cnt = 0;
            for (int u = 0; u < srcN; u++) {
                if (match[u] < 0 && dfsHopcroftKarp(u, visited))
                    ++cnt;
            }
            if (!cnt)
                break;

            res += cnt;
        }

        return res;
    }

    vector<int>& getLastMaxMatchingEdges() {
        return match;
    }

    //--- Min Vertex Cover ----------------------------------------------------

    // Must call this function after calcMaxMatchingXXX()
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

    //--- Max Independent Set -------------------------------------------------

    // Must call this function after calcMaxMatchingXXX()
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

private:
    vector<int> match;
    vector<int> matchRev;
    vector<int> dist;

    // return true if a matching for vertex u is possible
    bool dfsKuhn(int u, vector<bool>& visited) {
        if (visited[u])
            return false;
        visited[u] = true;

        for (int v : edges[u]) {
            if (matchRev[v] < 0 || dfsKuhn(matchRev[v], visited)) {
                matchRev[v] = u;
                match[u] = v;
                return true;
            }
        }
        return false;
    }

    //-------------------------------------------

    void bfsHopcroftKarp() {
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

    bool dfsHopcroftKarp(int u, vector<bool>& visited) {
        visited[u] = true;

        for (int v : edges[u]) {
            int u2 = matchRev[v];
            if (u2 < 0 || !visited[u2] && dist[u2] == dist[u] + 1 && dfsHopcroftKarp(u2, visited)) {
                matchRev[v] = u;
                match[u] = v;
                return true;
            }
        }
        return false;
    }
};
