#pragma once

#include "../set/unionFind.h"

struct BasicUndirectedGraph {
    int N;
    vector<vector<int>> edges;

    BasicUndirectedGraph() : N(0) {
    }

    explicit BasicUndirectedGraph(int n) : N(n), edges(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<int>>(N);
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    bool isConnectedGraph() const {
        vector<bool> visited(N);

        for (int u = 0; u < N; u++) {
            if (!edges[u].empty()) {
                dfs(visited, u);
                break;
            }
        }

        for (int u = 0; u < N; u++) {
            if (!visited[u] && !edges[u].empty())
                return false;
        }

        return true;
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
    bool isCyclicGraphDFS(vector<bool>& visited, int u, int parent) const {
        visited[u] = true;
        for (int v : edges[u]) {
            if (!visited[v]) {
                if (isCyclicGraphDFS(visited, v, u))
                    return true;
            } else if (v != parent)
                return true;
        }

        return false;
    }

    bool isCyclicGraphDFS() const {
        vector<bool> visited(N);
        for (int u = 0; u < N; u++) {
            if (!visited[u] && isCyclicGraphDFS(visited, u, -1))
                return true;
        }

        return false;
    }

    //--- Cycle detection with Union Find ---
    bool isCyclicGraphWithUnionFind() const {
        UnionFind uf(N);

        for (int u = 0; u < N; u++) {
            for (int v : edges[u]) {
                if (u < v) {
                    int x = uf.find(u);
                    int y = uf.find(v);
                    if (x == y)
                        return true;
                    uf.merge(u, v);
                }
            }
        }

        return false;
    }

    //--- Eulerian path & circuit
    int getOddDegreeCount() const {
        int odd = 0;
        for (int i = 0; i < N; i++)
            if (edges[i].size() & 1)
                odd++;
        return odd;
    }

    bool existEulerPath() const {
        if (!isConnectedGraph())
            return false;
        return (getOddDegreeCount() & ~2) == 0; // 0 or 2
    }

    bool existEulerCircuit() const {
        if (!isConnectedGraph())
            return false;
        return getOddDegreeCount() == 0;    // 0
    }

    vector<int> getEulerPath() const {
        vector<int> path;
        vector<vector<int>> residualEdges(edges);

        int u = 0;
        int odd = 0;
        for (int i = 0; i < N; i++) {
            if (residualEdges[i].size() & 1) {
                if (odd++ == 0)
                    u = i;
            }
        }
        if (odd != 0 && odd != 2)
            return path;

        stack<int> st;
        st.push(u);
        while (!st.empty()) {
            int v = st.top();
            if (!residualEdges[v].empty()) {
                u = residualEdges[v][0];
                st.push(u);
                //TODO: find() can be removed if each edge has opposite edge's index
                residualEdges[u].erase(find(residualEdges[u].begin(), residualEdges[u].end(), v));
                residualEdges[v].erase(residualEdges[v].begin());
            } else {
                path.push_back(v);
                st.pop();
            }
        }
        reverse(path.begin(), path.end());

        return path;
    }

    //---

    // Lexicographic Breadth First Search (Lex-BFS), O(V + E)
    vector<int> findLexicographicBFS() {
        // initialize a sequence ¥Ò of sets, to contain a single set containing all vertices.
        struct ListT {
            list<int> set;
            int time;
        };

        list<ListT> sigma;
        {
            sigma.resize(1);
            for (int i = 0; i < N; i++)
                sigma.back().set.push_back(i);
            sigma.back().time = 0;
        }

        vector<bool> visit(N);
        vector<list<ListT>::iterator> setInSigma(N, sigma.begin());
        vector<list<int>::iterator> nodeInSet;
        nodeInSet.reserve(N);
        for (auto it = sigma.front().set.begin(); it != sigma.front().set.end(); ++it)
            nodeInSet.push_back(it);

        int time = 0;
        vector<int> res;

        while (!sigma.empty()) {
            if (sigma.front().set.empty()) {
                sigma.pop_front();
                continue;
            }

            // find and remove a vertex u from the first set in ¥Ò
            int u = sigma.front().set.front();
            sigma.front().set.pop_front();

            // if the first set in ¥Ò is now empty, remove it from ¥Ò
            if (sigma.front().set.empty())
                sigma.pop_front();

            // add u to the end of the output sequence.
            res.push_back(u);
            visit[u] = true;
            ++time;

            // for each edge u-v such that v still belongs to a set S in ¥Ò :
            for (auto v : edges[u]) {
                if (visit[v])
                    continue;

                // If the set S containing v has not yet been replaced while processing u,
                //   create a new empty replacement set T and place it prior to S in the sequence;
                // otherwise, let T be the set prior to S.

                auto iter = setInSigma[v];

                setInSigma[v]->set.erase(nodeInSet[v]);
                if (setInSigma[v]->time < time) {
                    setInSigma[v]->time = time;
                    setInSigma[v] = sigma.insert(setInSigma[v], ListT{ {}, 0 });
                } else {
                    --setInSigma[v];
                }
                nodeInSet[v] = setInSigma[v]->set.insert(setInSigma[v]->set.end(), v);

                if (iter->set.empty())
                    sigma.erase(iter);
            }
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
