#pragma once

template <typename T>
struct BipartiteGraphArray {
    static const T INF = T(0x3f3f3f3f);

    int srcN;
    int dstN;
    vector<vector<bool>> edges;

    BipartiteGraphArray() : srcN(0), dstN(0) {
    }

    BipartiteGraphArray(int _srcN, int _dstN) : srcN(_srcN), dstN(_dstN), edges(srcN, vector<bool>(dstN)) {
    }

    void init(int _srcN, int _dstN) {
        srcN = _srcN;
        dstN = _dstN;
        edges = vector<vector<bool>>(srcN, vector<bool>(dstN));
    }

    void addEdge(int u, int v) {
        edges[u][v] = true;
    }

    // maximum number of matching from srcN to dstN
    // Kuhn's algorithm : O(V^3)
    int calcMaxMatching() {
        match.assign(srcN, -1);
        matchRev.assign(dstN, -1);

        int res = 0;
        for (int u = 0; u < srcN; u++) {
            vector<bool> visited(dstN);
            if (dfsBPM(u, visited))
                res++;
        }
        return res;
    }
    vector<int>& getLastMaxMatchingEdges() {
        return match;
    }

private:
    vector<int> match;
    vector<int> matchRev;

    // return true if a matching for vertex u is possible
    bool dfsBPM(int u, vector<bool>& visited) {
        if (visited[u])
            return false;
        visited[u] = true;

        for (int v = 0; v < dstN; v++) {
            if (edges[u][v]) {
                if (matchRev[v] < 0 || dfsBPM(matchRev[v], visited)) {
                    matchRev[v] = u;
                    match[u] = v;
                    return true;
                }
            }
        }
        return false;
    }
/*
    bool dfsBPM(int u, vector<bool>& visited) {
        for (int v = 0; v < dstN; v++) {
            if (edges[u][v] && !visited[v]) {
                visited[v] = true;

                if (matchRev[v] < 0 || dfsBPM(matchRev[v], visited)) {
                    matchRev[v] = u;
                    match[u] = v;
                    return true;
                }
            }
        }
        return false;
    }
*/
};

template <typename T>
struct BipartiteGraph {
    static const T INF = T(0x3f3f3f3f);

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
        edges = vector<vector<bool>>(srcN);
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
            vector<bool> visited(dstN);
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

            vector<bool> visited(dstN);

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
