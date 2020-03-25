#pragma once

/*
    Convert a undirected graph to a component tree
      - components become nodes of output tree
      - bridges become edges of output tree

    <How to use>
      1. init()
         - initialize graph and tree
      2. addEdge(), ...
         - build a graph
      3. build()
         - build a tree from a graph
*/
struct BridgeBlockTree {
    struct UndirectedGraph {
        int N;                                  // the number of vertex
        int M;                                  // the number of edge
        vector<vector<pair<int, int>>> edges;   // (v, index in edgeList)
        vector<pair<int, int>> edgeList;        // (u, v)

        vector<bool> bridge;                    // if edgeList[i] is bridge
        vector<int> bridgeList;                 // { edge index, ... }

        void init(int n, int m) {
            N = n;
            M = m;
            edges = vector<vector<pair<int, int>>>(N);
            edgeList.clear();
            bridge = vector<bool>(M);
            bridgeList.clear();
        }

        void addEdge(int u, int v) {
            int i = int(edgeList.size());
            edges[u].emplace_back(v, i);
            edges[v].emplace_back(u, i);
            edgeList.emplace_back(u, v);
        }

        //---

        // O(V + E), Tarjan's algorithm
        void findBridge() {
            visited = vector<bool>(N);

            discoverCount = 0;
            discover = vector<int>(N);
            low = vector<int>(N);

            for (int u = 0; u < N; u++) {
                if (!visited[u])
                    findBridge(u, -1);
            }
        }

    private:
        //--- for bridge
        vector<bool> visited;

        int discoverCount;
        vector<int> discover;
        vector<int> low;

        void findBridge(int u, int parent) {
            visited[u] = true;
            discover[u] = low[u] = discoverCount++;

            for (auto& it : edges[u]) {
                int v = it.first;
                if (!visited[v]) {
                    findBridge(v, u);
                    if (low[v] > discover[u]) {
                        bridge[it.second] = true;
                        bridgeList.emplace_back(it.second);
                    }
                    low[u] = min(low[u], low[v]);
                } else if (v != parent) {
                    low[u] = min(low[u], discover[v]);
                }
            }
        }
    };

    struct DSU {
        vector<int> parent;
        vector<int> rank;

        DSU() {
        }

        explicit DSU(int N) {
            init(N);
        }

        void init(int N) {
            parent.resize(N);
            rank.assign(N, 0);
            for (int i = 0; i < N; i++)
                parent[i] = i;
        }

        int find(int x) {
            if (parent[x] == x)
                return x;
            return parent[x] = find(parent[x]);
        }

        int merge(int x, int y) {
            int xset = find(x);
            int yset = find(y);
            if (xset == yset)
                return xset;

            if (rank[xset] < rank[yset]) {
                parent[xset] = yset;
                return yset;
            } else {
                parent[yset] = xset;
                if (rank[xset] == rank[yset])
                    rank[xset]++;
                return xset;
            }
        }
    };

    int N;
    vector<vector<int>> edges;
    vector<vector<int>> components;
    vector<int> nodeToComp;         // tree node id to component id

    UndirectedGraph graph;          // original graph and bridge information

    //--- build a graph

    void init(int vertexN, int edgeN) {
        graph.init(vertexN, edgeN);
    }

    void addEdge(int u, int v) {
        graph.addEdge(u, v);
    }

    //--- build a tree

    // O(n)
    void build() {
        graph.findBridge();

        components.clear();
        nodeToComp = vector<int>(graph.N);

        // grouping vertexes
        DSU dsu(graph.N);
        for (int i = 0; i < graph.M; i++) {
            if (!graph.bridge[i]) {
                auto& it = graph.edgeList[i];
                dsu.merge(it.first, it.second);
            }
        }

        // grouping vertexes into components
        unordered_map<int, int> convMap;
        for (int u = 0; u < graph.N; u++) {
            int group = dsu.find(u);
            int compId;
            auto it = convMap.find(group);
            if (it != convMap.end()) {
                compId = it->second;
                components[compId].push_back(u);
            } else {
                compId = int(convMap.size());
                convMap[group] = compId;
                components.emplace_back(vector<int>{ u });
            }
            nodeToComp[u] = compId;
        }

        // construct a tree from components
        N = int(convMap.size());
        edges = vector<vector<int>>(N);
        for (int i = 0; i < graph.M; i++) {
            if (graph.bridge[i]) {
                int u = nodeToComp[graph.edgeList[i].first];
                int v = nodeToComp[graph.edgeList[i].second];
                if (u != v) {
                    edges[u].push_back(v);
                    edges[v].push_back(u);
                }
            }
        }
    }
};
