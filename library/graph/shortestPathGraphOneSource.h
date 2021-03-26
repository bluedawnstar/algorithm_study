#pragma once

// for undirected graph
template <typename T, const T INF = 0x3f3f3f3f>
struct ShortestPathGraph {
    int N;

    vector<tuple<int, int, T>> edges;       // { (u, v, w), ... }
    vector<vector<pair<int, int>>> graph;   // undirected graph, edges[u] = { (v, edge-index), ... }
    vector<T> dist;                         // distances from the source node
    vector<vector<pair<int, int>>> shortestPathDAG; // shortest path graph that the source node is the sink, shortestPathDAG[v] = { (u, edge-index), ... }

    ShortestPathGraph() : N(0) {
    }

    ShortestPathGraph(int n) : N(n), graph(n), dist(n), shortestPathDAG(n) {
    }

    void init(int n) {
        N = n;
        edges.clear();
        graph = vector<vector<pair<int, int>>>(N);
        dist = vector<T>(N);
        shortestPathDAG = vector<vector<pair<int, int>>>(N);
    }

    void addEdge(int u, int v, T w) {
        int edgeIndex = int(edges.size());
        graph[u].emplace_back(v, edgeIndex);
        graph[v].emplace_back(u, edgeIndex);
        edges.emplace_back(u, v, w);
    }

    // O(E*logV)
    void dijkstra(int start) {
        fill(dist.begin(), dist.end(), INF);

        priority_queue<pair<T, int>> pq;    // (-weight, vertex)

        pq.emplace(0, start);
        dist[start] = 0;
        shortestPathDAG[start].clear();
        while (!pq.empty()) {
            T w = -pq.top().first;          // weight
            int u = pq.top().second;        // vertex u

            pq.pop();
            if (dist[u] < w)
                continue;

            for (int i = 0; i < int(graph[u].size()); i++) {
                int v = graph[u][i].first;
                int ei = graph[u][i].second;
                T vDist = w + get<2>(edges[ei]);
                if (dist[v] == vDist) {
                    shortestPathDAG[v].emplace_back(u, ei);
                } else if (dist[v] > vDist) {
                    pq.emplace(-vDist, v);
                    dist[v] = vDist;
                    shortestPathDAG[v].clear();
                    shortestPathDAG[v].emplace_back(u, ei);
                }
            }
        }
    }

    //---

    struct UndirectedGraph {
        int N;

        vector<int> originalVertex; // [vertex id of current graph] = vertex id of original graph
        vector<vector<int>> edges;  // undirected graph

        UndirectedGraph() : N(0) {
        }

        explicit UndirectedGraph(int n) {
            init(n);
        }

        void init(int n) {
            N = n;
            originalVertex = vector<int>(N);
            edges = vector<vector<int>>(N);
        }

        void addEdge(int u, int v) {
            edges[u].push_back(v);
            edges[v].push_back(u);
        }


        //--- for cut-vertex

        vector<bool> visited;
        int discoverCount;
        vector<int> discover;
        vector<int> low;
        vector<bool> cutVertex;

        // O(V + E), Tarjan's algorithm
        const vector<bool>& findCutVertex() {
            visited = vector<bool>(N);

            discoverCount = 0;
            discover = vector<int>(N);
            low = vector<int>(N);

            cutVertex = vector<bool>(N);

            for (int u = 0; u < N; u++) {
                if (!visited[u])
                    findCutVertex(u, -1);
            }
            return cutVertex;
        }

    private:
        void findCutVertex(int u, int parent) {
            visited[u] = true;
            discover[u] = low[u] = discoverCount++;

            int childCount = 0;
            for (int v : edges[u]) {
                if (!visited[v]) {
                    findCutVertex(v, u);
                    childCount++;
                    if (low[v] >= discover[u])
                        cutVertex[u] = true;
                    low[u] = min(low[u], low[v]);
                } else if (v != parent)
                    low[u] = min(low[u], discover[v]);
            }
            if (parent < 0)
                cutVertex[u] = (childCount > 1);
        }
    };

    UndirectedGraph buildUndirectedGraphFromShortestPaths(int dst) {
        vector<int> vertexes;       // vertexes
        vector<int> selectedEdges;  // edge indexes
        {
            vector<bool> vis(N);
            queue<int> Q;
            Q.push(dst);
            vis[dst] = true;
            vertexes.push_back(dst);
            while (!Q.empty()) {
                int u = Q.front();
                Q.pop();

                for (auto& it : shortestPathDAG[u]) {
                    selectedEdges.push_back(it.second);
                    int v = it.first;
                    if (vis[v])
                        continue;
                    Q.push(v);
                    vis[v] = true;
                    vertexes.push_back(v);
                }
            }
        }
        sort(vertexes.begin(), vertexes.end());
        vertexes.erase(unique(vertexes.begin(), vertexes.end()), vertexes.end());

        //-- build a graph
        int N = int(vertexes.size());
        UndirectedGraph graph(N);
        for (int eidx : selectedEdges) {
            int u = get<0>(edges[eidx]);
            int v = get<1>(edges[eidx]);
            u = int(lower_bound(vertexes.begin(), vertexes.end(), u) - vertexes.begin());
            v = int(lower_bound(vertexes.begin(), vertexes.end(), v) - vertexes.begin());
            graph.addEdge(u, v);
        }
        swap(graph.originalVertex, vertexes);

        return graph;
    }
};
