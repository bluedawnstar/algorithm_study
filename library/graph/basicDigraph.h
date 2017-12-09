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

    //--- shortest path without weight ---

    vector<int> searchShortestPathBFS(int start) {
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

        return move(parent);
    }

    vector<int> getShortestPath(int u, int v) {
        vector<int> parent = searchShortestPathBFS(u);

        vector<int> res;
        do {
            res.push_back(v);
            v = parent[v];
        } while (v >= 0);
        reverse(res.begin(), res.end());

        return res;
    }

    //--- cycle detection with DFS ---

    bool isCyclicGraphDFS(vector<bool>& visited, int u, vector<bool>& visStack) {
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

    bool isCyclicGraphDFS() {
        vector<bool> visited(N);
        vector<bool> visStack(N);

        for (int u = 0; u < N; u++) {
            if (!visited[u] && isCyclicGraphDFS(visited, u, visStack))
                return true;
        }

        return false;
    }

    //--- strongly connected components (Tarjan's algorithm) ---
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
    void findSCC(SCCContext& ctx, int u) {
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

    vector<vector<int>> findSCC() {
        SCCContext ctx(N);

        for (int u = 0; u < N; u++) {
            if (!ctx.visited[u])
                findSCC(ctx, u);
        }

        return move(ctx.scc);
    }

    //--- strongly connected graph test (Kosaraju's algorithm) ---
    static void dfsSCGraph(const vector<vector<int>>& edges, vector<bool>& visited, int u) {
        visited[u] = true;
        for (int v : edges[u]) {
            if (!visited[v])
                dfsSCGraph(edges, visited, v);
        }
    }

    bool isSCGraph() {
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

protected:
    void dfs(int u, vector<bool>& visited) {
        //cout << "dfs(" << u << ")" << endl;

        visited[u] = true;
        for (int v : edges[u]) {
            if (!visited[v])
                dfs(v, visited);
        }
    }

    void dfsAll() {
        vector<bool> visited(N);
        for (int u = 0; u < N; u++) {
            if (!visited[u])
                dfs(u, visited);
        }
    }

    void bfs(int start, vector<bool>& visited) {
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

    void bfsAll() {
        vector<bool> visited(N);
        for (int u = 0; u < N; u++) {
            if (!visited[u])
                bfs(u, visited);
        }
    }
};
