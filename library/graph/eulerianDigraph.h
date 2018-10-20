#pragma once

struct EulerianDigraph {
    int N;
    vector<vector<int>> edges;

    EulerianDigraph() : N(0) {
    }

    explicit EulerianDigraph(int n) : N(n), edges(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<int>>(N);
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
    }


    // return (indegree, edge count)
    // O(E)
    pair<vector<int>, int> calcInDegree() const {
        pair<vector<int>, int> res;
        res.second = 0;;
        res.first.assign(N, 0);

        for (int i = 0; i < N; i++) {
            res.second += int(edges[i].size());
            for (int v : edges[i])
                res.first[v]++;
        }

        return res;
    }

    //--- Euler circuit

    // precondition: all vertices are connected
    // O(V + E)
    bool existEulerCircuitNaive() {
        auto indeg = calcInDegree();

        for (int i = 0; i < N; i++) {
            if (int(edges[i].size()) != indeg.first[i])
                return false;
        }
        return true;
    }

    // O(V + E)
    bool existEulerCircuit() {
        auto indeg = calcInDegree();

        for (int i = 0; i < N; i++) {
            if (edges[i].size() != indeg.first[i])
                return false;
        }
        return isSC(indeg.first, 0, 0);
    }

    // Hierholzer¡¯s Algorithm, O(V + E)
    vector<int> getEulerCircuit() {
        vector<int> path;

        int edgeCount = 0;
        for (int u = 0; u < N; u++)
            edgeCount += int(edges[u].size());

        path.reserve(edgeCount + 1);

        vector<vector<int>> residualEdges(edges);
        {
            stack<int> st;
            st.push(0);
            while (!st.empty()) {
                int v = st.top();
                if (!residualEdges[v].empty()) {
                    int u = residualEdges[v].back();
                    st.push(u);
                    residualEdges[v].pop_back();
                } else {
                    path.push_back(v);
                    st.pop();
                }
            }
        }
        if (path.size() != edgeCount + 1 || path.front() != path.back())
            return vector<int>{};

        reverse(path.begin(), path.end());

        return path;
    }

    //--- Euler path

    // precondition: all vertices are connected
    // O(V + E)
    bool existEulerPathNaive() const {
        auto indeg = calcInDegree();

        int in = 0;
        int out = 0;
        for (int i = 0; i < N; i++) {
            int deg = int(edges[i].size()) - indeg.first[i];    // out_deg - in_deg
            if (deg == 1)
                out++;
            else if (deg == -1)
                in++;
            else if (deg != 0)
                return false;
        }
        return in == out && (in <= 1);  // (in == 0 && out == 0) || (in == 1 && out == 1)
    }

    // O(V + E)
    bool existEulerPath() {
        auto indeg = calcInDegree();

        int in = 0, out = 0;
        int start = 0, finish = 0;
        for (int i = 0; i < N; i++) {
            int deg = int(edges[i].size()) - indeg.first[i];
            if (deg == 1) {
                out++;
                start = i;
            } else if (deg == -1) {
                in++;
                finish = i;
            } else if (deg != 0)
                return false;
        }
        if (in != out || (in > 1))
            return false;

        return isSC(indeg.first, start, finish);
    }

    // O(V)
    int findEulerPathStart(const vector<int>& indeg) const {
        int in = 0;
        int out = 0;
        int start = 0;
        for (int i = 0; i < N; i++) {
            int deg = int(edges[i].size()) - indeg[i];
            if (deg == 1) {
                out++;
                start = i;
            } else if (deg == -1)
                in++;
            else if (deg != 0)
                return -1;
        }
        if (in != out || (in > 1))
            return -1;

        return start;
    }

    // Hierholzer¡¯s Algorithm, O(V + E)
    vector<int> getEulerPath() {
        vector<int> path;
        auto indeg = calcInDegree();

        int u = findEulerPathStart(indeg.first);
        if (u < 0)
            return path;

        path.reserve(indeg.second + 1);

        vector<vector<int>> residualEdges(edges);
        {
            stack<int> st;
            st.push(u);
            while (!st.empty()) {
                int v = st.top();
                if (!residualEdges[v].empty()) {
                    u = residualEdges[v].back();
                    st.push(u);
                    residualEdges[v].pop_back();
                } else {
                    path.push_back(v);
                    st.pop();
                }
            }
        }
        if (path.size() != indeg.second + 1)
            return vector<int>{};

        reverse(path.begin(), path.end());

        return path;
    }

private:
    //--- Strongly connected graph test (Kosaraju's algorithm), O(E + V) ---

    static void dfsSCGraph(const vector<vector<int>>& edges, vector<bool>& visited, int u) {
        visited[u] = true;
        for (int v : edges[u]) {
            if (!visited[v])
                dfsSCGraph(edges, visited, v);
        }
    }

    // O(V + E)
    bool isSC(const vector<int>& indeg, int start, int finish) {
        // step 1 : check connectivity of the current graph
        vector<bool> visited(N);

        dfsSCGraph(edges, visited, start);
        for (int i = 0; i < N; i++) {
            if (!visited[i] && (!edges[i].empty() || indeg[i] > 0))
                return false;
        }

        // step 2 : make a reversed graph
        vector<vector<int>> revEdge(N);
        for (int u = 0; u < N; u++) {
            for (int v : edges[u])
                revEdge[v].push_back(u);
        }

        // step 3 : check connectivity of the reversed graph
        fill(visited.begin(), visited.end(), false);

        dfsSCGraph(revEdge, visited, finish);
        for (int i = 0; i < N; i++) {
            if (!visited[i] && (!edges[i].empty() || indeg[i] > 0))
                return false;
        }

        return true;
    }
};
