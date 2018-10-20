#pragma once

struct BasicDigraph {
    int N;
    vector<vector<int>> edges;

    BasicDigraph() : N(0) {
    }

    explicit BasicDigraph(int n) : N(n), edges(N) {
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
