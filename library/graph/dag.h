#pragma once

// Directed Acyclic Graph
template <typename T>
struct DAG {
    static const T INF = T(0x3f3f3f3f);

    int N;
    vector<vector<int>> edges;

    DAG() : N(0) {
    }

    DAG(int n) : N(n), edges(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<pair<int, T>>>(N);
    }

    // add edges to undirected graph
    void addEdge(int u, int v) {
        edges[u].push_back(v);
    }

    //--- topological sort with DFS

    // return if cycle detected
    bool topologicalSortDFS(int u, vector<int>& res) {
        res.clear();
        res.reserve(N);

        vector<bool> visited(N);

        bool cycleDetected = topologicalSortDFS(u, visited, res);
        reverse(res.begin(), res.end());

        return cycleDetected;
    }

    // return if cycle detected
    bool topologicalSortDFS(vector<int>& res) {
        res.clear();
        res.reserve(N);

        vector<bool> visited(N);

        bool cycleDetected = false;
        for (int i = 0; i < N; i++) {
            if (!visited[i]) {
                if (topologicalSortDFS(i, visited, res))
                    cycleDetected = true;
            }
        }
        reverse(res.begin(), res.end());

        return cycleDetected;
    }

    //--- topological sort with BFS (Kahn's algorithm)

    // return if cycle detected
    bool topologicalSortBFS(vector<int>& res) {
        res.clear();
        res.reserve(N);

        vector<int> inDegree(N, 0);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < (int)edges[i].size(); j++)
                inDegree[edges[i][j]]++;
        }

        queue<int> Q;
        for (int i = 0; i < N; i++) {
            if (inDegree[i] == 0)
                Q.push(i);
        }
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            res.push_back(u);
            for (int i = 0; i < (int)edges[u].size(); i++) {
                int v = edges[u][i];
                if (--inDegree[v] == 0)
                    Q.push(v);
            }
        }

        return res.size() != N;
    }

private:
    // return if cycle detected
    bool topologicalSortDFS(int u, vector<bool>& visited, vector<int>& res) {
        visited[u] = true;
        for (int i = 0; i < (int)edges[u].size(); i++) {
            int v = edges[u][i];
            if (!visited[v]) {
                if (topologicalSortDFS(v, visited, res))
                    return true;
            }
            //TODO: uncomment if cycle detection is necessary
            //else if (find(res.begin(), res.end(), v) == res.end())
            //    return true;
        }
        res.push_back(u);
        return false;
    }
};
