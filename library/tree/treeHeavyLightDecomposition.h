#pragma once

// Heavy Light Decomposition
struct HeavyLightDecomposition {
    Tree& tree;

    int root;
    vector<vector<int>> heavyPaths;         // heavy paths
    vector<int>         heavyPathIndex;     // convert node number(0 ~ N - 1) to index of heavy paths (heavyPaths)

    explicit HeavyLightDecomposition(Tree& tree) : tree(tree), root(0) {
        // no action
    }

    void doHLD(int root) {
        root = root;
        heavyPaths.clear();
        heavyPathIndex.resize(tree.N, -1);

        vector<bool> visited(tree.N);

        queue<int> Q;
        Q.push(root);
        visited[root] = true;
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (int v : tree.edges[u]) {
                if (visited[v])
                    continue;
                Q.push(v);
                visited[v] = true;
            }

            if (u == root)
                continue;

            int p = tree.P[0][u];

            if (tree.treeSize[u] * 2 >= tree.treeSize[p] && p != root) {
                // heavy path -> add u to parent's heavy path
                int parentPath = heavyPathIndex[p];
                heavyPaths[parentPath].push_back(u);
                heavyPathIndex[u] = parentPath;
            } else {
                // light path -> make new heavy path
                heavyPathIndex[u] = (int)heavyPaths.size();
                heavyPaths.push_back(vector<int>(2));
                heavyPaths.back()[0] = p;
                heavyPaths.back()[1] = u;
            }
        }
    }

    // PRECONDITION: v is not the root
    int indexInPath(int v) {
        assert(v != root);
        int topOfPath = heavyPaths[heavyPathIndex[v]][0];
        return tree.level[tree.P[0][v]] - tree.level[topOfPath];
    }

    // PRECONDITION: path is the index of v's heavy path
    // PRECONDITION: v is not the root
    int indexInPath(int path, int v) {
        assert(v != root);
        int topOfPath = heavyPaths[path][0];
        return tree.level[tree.P[0][v]] - tree.level[topOfPath];
    }
};
