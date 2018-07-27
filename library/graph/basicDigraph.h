#pragma once

struct BasicDigraph {
    int N;
    vector<vector<int>> edges;

    BasicDigraph() : N(0) {
    }

    BasicDigraph(int n) : N(n), edges(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<int>>(N);
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
    }

    //--- Shortest path without weight ---
    vector<int> searchShortestPathBFS(int start) const {
        vector<int> dist(N, -1);
        vector<int> parent(N, -1);

        queue<int> Q;
        Q.push(start);
        dist[start] = 0;
        parent[start] = -1;
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (int v : edges[u]) {
                if (dist[v] < 0) {
                    Q.push(v);
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                }
            }
        }

        return parent;
    }

    vector<int> getShortestPath(int u, int v) const {
        vector<int> parent = searchShortestPathBFS(u);

        vector<int> res;
        do {
            res.push_back(v);
            v = parent[v];
        } while (v >= 0);
        reverse(res.begin(), res.end());

        return res;
    }

    //--- Cycle detection with DFS ---
    bool isCyclicGraphDFS(vector<bool>& visited, int u, vector<bool>& visStack) const {
        visited[u] = true;
        visStack[u] = true;

        for (int v : edges[u]) {
            if (!visited[v]) {
                if (isCyclicGraphDFS(visited, v, visStack))
                    return true;
            } else if (visStack[v])
                return true;
        }
        visStack[u] = false;

        return false;
    }

    bool isCyclicGraphDFS() const {
        vector<bool> visited(N);
        vector<bool> visStack(N);

        for (int u = 0; u < N; u++) {
            if (!visited[u] && isCyclicGraphDFS(visited, u, visStack))
                return true;
        }

        return false;
    }

    //--- Strongly connected components (Tarjan's algorithm) ---
    struct SCCContext {
        vector<vector<int>> scc;

        vector<bool> visited;

        int discoverCount;
        vector<int> discover;
        vector<int> low;

        vector<bool> stacked;
        vector<int> stack;

        SCCContext(int n)
            : scc(), visited(n), discoverCount(0), discover(n), low(n), stacked(n) {
        }
    };
    void findSCC(SCCContext& ctx, int u) const {
        ctx.visited[u] = true;
        ctx.discover[u] = ctx.low[u] = ctx.discoverCount++;

        ctx.stack.push_back(u);
        ctx.stacked[u] = true;
        for (int v : edges[u]) {
            if (!ctx.visited[v]) {
                findSCC(ctx, v);
                ctx.low[u] = min(ctx.low[u], ctx.low[v]);
            } else if (ctx.stacked[v]) // back edge
                ctx.low[u] = min(ctx.low[u], ctx.discover[v]);
        }

        // u is a root of an SCC
        if (ctx.low[u] == ctx.discover[u]) {
            vector<int> scc;
            while (!ctx.stack.empty() && ctx.stack.back() != u) {
                int w = ctx.stack.back();
                scc.push_back(w);
                ctx.stack.pop_back();
                ctx.stacked[w] = false;
            }
            scc.push_back(u);
            ctx.stack.pop_back();
            ctx.stacked[u] = false;

            ctx.scc.push_back(scc);
        }
    }

    vector<vector<int>> findSCC() const {
        SCCContext ctx(N);

        for (int u = 0; u < N; u++) {
            if (!ctx.visited[u])
                findSCC(ctx, u);
        }

        return ctx.scc;
    }

    //--- Strongly connected graph test (Kosaraju's algorithm), O(E + V) ---
    static void dfsSCGraph(const vector<vector<int>>& edges, vector<bool>& visited, int u) {
        visited[u] = true;
        for (int v : edges[u]) {
            if (!visited[v])
                dfsSCGraph(edges, visited, v);
        }
    }

    bool isSCGraph() const {
        vector<bool> visited(N);

        // step 1 : check connectivity of the current graph
        dfsSCGraph(edges, visited, 0);
        for (int u = 0; u < N; u++) {
            if (!visited[u])
                return false;
        }

        // step 2 : make a reversed graph
        vector<vector<int>> revEdge(N);
        for (int u = 0; u < N; u++) {
            for (int v : edges[u]) {
                revEdge[v].push_back(u);
            }
        }

        // step 3 : check connectivity of the reversed graph
        visited.assign(N, false);
        dfsSCGraph(revEdge, visited, 0);
        for (int i = 0; i < N; i++) {
            if (!visited[i])
                return false;
        }

        return true;
    }

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


    //--- Eulerian path & circuit
    // return (indegree, edge count)
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

    // precondition: all vertices are connected
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

    // precondition: all vertices are connected
    bool existEulerCircuitNaive() {
        auto indeg = calcInDegree();

        for (int i = 0; i < N; i++) {
            if (int(edges[i].size()) != indeg.first[i])
                return false;
        }
        return true;
    }

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

    bool existEulerCircuit() {
        auto indeg = calcInDegree();

        for (int i = 0; i < N; i++) {
            if (edges[i].size() != indeg.first[i])
                return false;
        }
        return isSC(indeg.first, 0, 0);
    }

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
                    u = residualEdges[v][0];
                    st.push(u);
                    residualEdges[v].erase(residualEdges[v].begin());
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

protected:
    void dfs(vector<bool>& visited, int u) const {
        //cout << "dfs(" << u << ")" << endl;

        visited[u] = true;
        for (int v : edges[u]) {
            if (!visited[v])
                dfs(visited, v);
        }
    }

    void dfsAll() const {
        vector<bool> visited(N);
        for (int u = 0; u < N; u++) {
            if (!visited[u])
                dfs(visited, u);
        }
    }

    void bfs(int start, vector<bool>& visited) const {
        queue<int> Q;
        Q.push(start);
        visited[start] = true;
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            //cout << "bfs(" << start << ") : " << u << endl;

            for (int v : edges[u]) {
                if (!visited[v]) {
                    Q.push(v);
                    visited[v] = true;
                }
            }
        }
    }

    void bfsAll() const {
        vector<bool> visited(N);
        for (int u = 0; u < N; u++) {
            if (!visited[u])
                bfs(u, visited);
        }
    }
};
