#pragma once

struct CentroidDecomposition {
    int N;
    vector<vector<int>> edges;
    vector<int> parent;
    vector<int> treeSize;
    int root;

    int decomposeTree(Tree& tree, int start) {
        N = (int)tree.N;
        edges = vector<vector<int>>(N);
        parent = vector<int>(N);
        treeSize = vector<int>(N);
        visited = vector<bool>(N);

        dfsSize(tree, start, -1);

        root = decomposeTreeSub(tree, start);
        parent[root] = -1;

        return root;
    }

protected:
    vector<bool> visited;

    int decomposeTreeSub(Tree& tree, int u) {
        int size = treeSize[u];
        int centroid = findCentroid(tree, u, -1, size);

        visited[centroid] = true;
        if (parent[centroid] >= 0 && !visited[parent[centroid]]) {
            //dfsSize(tree, parent[centroid], centroid);    // update all nodes of a updated sub-tree
            updateSize(parent[centroid], centroid);         // update only nodes of a path from centroid to old root
        }
        treeSize[centroid] = size;

        for (int v : tree.edges[centroid]) {
            if (visited[v])
                continue;

            int subRoot = decomposeTreeSub(tree, v);
            parent[subRoot] = centroid;

            edges[centroid].push_back(subRoot);
            edges[subRoot].push_back(centroid);
        }

        return centroid;
    }

    int findCentroid(Tree& tree, int u, int from, int size) {
        bool isMajor = true;
        for (int v : tree.edges[u]) {
            if (v == from || visited[v])
                continue;

            int res = findCentroid(tree, v, u, size);
            if (res != -1)
                return res;

            if (treeSize[v] + treeSize[v] > size)
                isMajor = false;
        }

        if (isMajor && (size - treeSize[u]) * 2 <= size)
            return u;

        return -1;
    }

    int updateSize(int u, int from) {
        if (u < 0 || visited[u])
            return 0;

        treeSize[u] = treeSize[u] - treeSize[from] + updateSize(parent[u], u);
        parent[u] = from;

        return treeSize[u];
    }

    void dfsSize(Tree& tree, int u, int from) {
        treeSize[u] = 1;
        parent[u] = from;
        for (int v : tree.edges[u]) {
            if (v == from || visited[v])
                continue;

            dfsSize(tree, v, u);
            treeSize[u] += treeSize[v];
        }
    }
};
