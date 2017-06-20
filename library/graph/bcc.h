#pragma once

// Undirected Graph
struct BiconnectedComponents {
    int                 N;

    // output
    vector<int>         graphID;
    vector<bool>        cutV;
    vector<vector<int>> comps;

    void findBCC(vector<vector<int>>& edges, int N) {
        this->N = N;
        visited = vector<bool>(N);
        graphID = vector<int>(N);
        cutV = vector<bool>(N);
        discover = vector<int>(N);
        low = vector<int>(N);

        comps.clear();

        discoverCount = 0;
        graphCnt = 0;

        for (int u = 0; u < N; ++u) {
            if (!discover[u]) {
                discoverCount = 0;
                stk.clear();

                dfsConn(edges, u, -1);

                graphCnt++;
            }
        }
    }

private:
    vector<bool> visited;
    int          graphCnt;
    int          discoverCount;
    vector<int>  discover;
    vector<int>  low;
    vector<int>  stk;

    void dfsConn(vector<vector<int>>& edges, int u, int parent) {
        discover[u] = low[u] = ++discoverCount;
        stk.push_back(u);

        graphID[u] = graphCnt;

        int childCount = 0;
        for (int v : edges[u]) {
            if (v == parent)
                continue;

            if (!discover[v]) {
                dfsConn(edges, v, u);
                childCount++;

                low[u] = min(low[u], low[v]);
                if (low[v] >= discover[u]) {
                    cutV[u] = (discover[u] > 1 || discover[v] > 2);

                    comps.push_back({ u });
                    while (comps.back().back() != v) {
                        comps.back().push_back(stk.back());
                        stk.pop_back();
                    }
                }
            } else {
                low[u] = min(low[u], discover[v]);
            }
        }
    }
};


// Construct block-cut tree from biconnected components
struct BlockCutTree {
    int                 N;
    vector<vector<int>> edges;
    vector<bool>        cutV;
    vector<int>         graphToTreeID;

    void makeFromBCC(const BiconnectedComponents& bcc) {
        edges = vector<vector<int>>(bcc.N * 2);
        cutV = vector<bool>(bcc.N * 2);
        graphToTreeID = vector<int>(bcc.N * 2);

        N = 0;
        for (int u = 0; u < bcc.N; ++u) {
            if (bcc.cutV[u]) {
                graphToTreeID[u] = N++;
                cutV[graphToTreeID[u]] = true;
            }
        }
        for (auto& comp : bcc.comps) {
            int node = N++;
            for (int u : comp) {
                if (!bcc.cutV[u])
                    graphToTreeID[u] = node;
                else {
                    edges[node].push_back(graphToTreeID[u]);
                    edges[graphToTreeID[u]].push_back(node);
                }
            }
        }
    }
};
