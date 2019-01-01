#pragma once

// https://tanujkhattar.wordpress.com/2016/01/11/dominator-tree-of-a-directed-graph/
// https://www.cs.princeton.edu/courses/archive/fall03/cs528/handouts/a%20fast%20algorithm%20for%20finding.pdf

#if 1
// Implementation #1 from https://ideone.com/aO6cH4
struct DominatorTree {
    int N;
    vector<vector<int>> edges;  // original graph

    int treeRoot;
    vector<vector<int>> tree;   // dominator tree
    vector<int> treeParent;     // treeParent[i] = idom of node i

    DominatorTree() : N(0) {
    }

    explicit DominatorTree(int N) : N(N), edges(N) {
    }

    DominatorTree(const vector<vector<int>>& edges, int N, int start) {
        buildFromGraph(edges, N, start);
    }

    //--- build from another graph

    void buildFromGraph(const vector<vector<int>>& edges, int N, int start) {
        this->N = N;
        this->edges = edges;
        build(start);
    }

    //--- build from this graph

    void add(int u, int v) {
        edges[u].push_back(v);
    }

    // O((N + M)logN)
    void build(int start) {
        treeRoot = start;
        tree = vector<vector<int>>(N);
        treeParent = vector<int>(N, -1);

        idom = vector<int>(N, -1);
        sdom = vector<int>(N);
        bucket = vector<vector<int>>(N);

        currTime = 0;
        revEdges = vector<vector<int>>(N);
        nodeToTime = vector<int>(N, -1);
        timeToNode = vector<int>(N);
        parent = vector<int>(N, -1);

        ufParent = vector<int>(N);
        ufUp = vector<int>(N);

        dfs(start);
        iota(ufParent.begin(), ufParent.end(), 0);
        iota(ufUp.begin(), ufUp.end(), 0);
        iota(sdom.begin(), sdom.end(), 0);

        for (int t = currTime - 1; t >= 0; t--) {
            for (int ut : revEdges[t])
                sdom[t] = min(sdom[t], sdom[find(ut)]);
            if (t > 0)
                bucket[sdom[t]].push_back(t);
            for (int ut : bucket[t]) {
                int p = find(ut);
                if (sdom[p] == t)
                    idom[ut] = t;
                else
                    idom[ut] = p;
            }
            //bucket[t].clear();
            if (t > 0)
                merge(t, parent[t]);
        }
        for (int t = 1; t < currTime; t++) {
            if (sdom[t] != idom[t])
                idom[t] = idom[idom[t]];

            int u = timeToNode[idom[t]];
            int v = timeToNode[t];
            treeParent[v] = u;
            tree[u].push_back(v);
            tree[v].push_back(u);
        }
    }

private:
    vector<int> idom;               // nodeToTime[node] to idom
    vector<int> sdom;               // semi-dominator
    vector<vector<int>> bucket;     // semi-dominator to node (bucket[i] is a list of vertices for which i is the semi-dominator)

    int currTime;
    vector<vector<int>> revEdges;   // reverse edges based on time
    vector<int> nodeToTime;         // node to time
    vector<int> timeToNode;         // time to node
    vector<int> parent;             // parent based on time

    void dfs(int u) {
        nodeToTime[u] = currTime;
        timeToNode[currTime++] = u;
        for (int v : edges[u]) {
            if (nodeToTime[v] == -1) {
                dfs(v);
                parent[nodeToTime[v]] = nodeToTime[u];
            }
            revEdges[nodeToTime[v]].push_back(nodeToTime[u]);
        }
    }

    //--- union-find

    vector<int> ufParent;
    vector<int> ufUp;             // min sdom's vertex

    void merge(int x, int y) {
        ufParent[x] = y;
    }

    int find(int x, bool toGetParent = false) {
        if (ufParent[x] == x)
            return toGetParent ? -1 : x;
        int p = find(ufParent[x], true);
        if (p != -1) {
            if (sdom[ufUp[x]] > sdom[ufUp[ufParent[x]]])
                ufUp[x] = ufUp[ufParent[x]];
            ufParent[x] = p;
        }
        return toGetParent ? ufParent[x] : ufUp[x];
    }
};
#else
// Implementation #2 from https://github.com/yosupo06/Algorithm/blob/master/src/graph/dominator.h
struct DominatorTree {
    int N;
    vector<vector<int>> edges;      // original graph
    vector<vector<int>> revEdges;   // reverse graph

    int                 treeRoot;
    vector<int>&        treeParent = idom;
    vector<vector<int>> tree;       // dominator tree

    DominatorTree() : N(0) {
    }

    explicit DominatorTree(int N) : N(N), edges(N) {
    }

    DominatorTree(const vector<vector<int>>& edges, int N, int start) {
        buildFromGraph(edges, N, start);
    }

    //--- build from another graph

    void buildFromGraph(const vector<vector<int>>& edges, int N, int start) {
        this->N = N;
        this->edges = edges;
        build(start);
    }

    //--- build from this graph

    void add(int u, int v) {
        edges[u].push_back(v);
    }

    // O((N + M)logN)
    void build(int start) {
        revEdges = vector<vector<int>>(N);

        treeRoot = start;
        tree = vector<vector<int>>(N);

        idom = vector<int>(N, -1);
        sdom = vector<int>(N);

        bucket = vector<vector<int>>(N);
        up = vector<int>(N);
        currTime = 0;
        nodeToTime = vector<int>(N, -1);
        timeToNode = vector<int>(N);
        parent = vector<int>(N, -1);

        // unionfind
        ufParent = vector<int>(N, -1);
        ufUp = vector<int>(N);
        iota(ufUp.begin(), ufUp.end(), 0);

        dfs(start, -1);
        iota(sdom.begin(), sdom.end(), 0);

        for (int t = N - 1; t > 0; t--) {
            int u = timeToNode[t];
            for (int v : revEdges[u])
                sdom[u] = timeToNode[min(nodeToTime[sdom[u]], nodeToTime[sdom[find(v)]])];
            bucket[sdom[u]].push_back(u);
            for (int v : bucket[parent[u]])
                up[v] = find(v);
            bucket[parent[u]].clear();
            merge(u, parent[u]);
        }

        // build idom & tree
        for (int t = 1; t < N; t++) {
            int u = timeToNode[t];
            int v = up[u];
            if (sdom[u] == sdom[v])
                idom[u] = sdom[u];
            else
                idom[u] = idom[v];

            tree[idom[u]].push_back(u);
            tree[u].push_back(idom[u]);
        }
    }

private:
    vector<int>         idom;       // node to idom
    vector<int>         sdom;       // semi-dominator
    vector<vector<int>> bucket;     // semi-dominator to node (bucket[i] is a list of vertices for which i is the semi-dominator)
    vector<int>         up;

    int                 currTime;
    vector<int>         nodeToTime; // node to time
    vector<int>         timeToNode; // time to node
    vector<int>         parent;

    void dfs(int u, int par) {
        parent[u] = par;
        nodeToTime[u] = currTime;
        timeToNode[currTime++] = u;
        for (int v : edges[u]) {
            if (nodeToTime[v] == -1)
                dfs(v, u);
            revEdges[v].push_back(u);
        }
    }

    //--- union-find

    vector<int> ufParent;
    vector<int> ufUp;             // min sdom's vertex

    void merge(int a, int b) {
        ufParent[a] = b;
    }

    int find(int a) {
        if (ufParent[a] != -1) {
            find(ufParent[a]);
            if (nodeToTime[sdom[ufUp[a]]] > nodeToTime[sdom[ufUp[ufParent[a]]]]) {
                ufUp[a] = ufUp[ufParent[a]];
            }
            if (ufParent[ufParent[a]] != -1)
                ufParent[a] = ufParent[ufParent[a]];
        }
        return ufUp[a];
    }
};
#endif
