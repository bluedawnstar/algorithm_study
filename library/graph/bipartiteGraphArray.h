#pragma once

// Simple, but slow
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

    //--- Max Independent Set -------------------------------------------------

    // Must call this function after calcMaxMatching()
    // return (chosen A, chosen B), O(V^3)
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
                for (int v = 0; v < dstN; v++) {
                    if (res.second[v] && edges[u][v]) {
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
