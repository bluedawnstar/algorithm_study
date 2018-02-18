#pragma once

// Heavy Light Decomposition
struct HeavyLightDecomposition {
    Tree& mTree;

    int mRoot;
    vector<vector<int>> mHeavyPaths;        // heavy paths
    vector<int>         mHeavyPathIndex;    // convert node number(0 ~ N - 1) to index of heavy paths (mHeavyPaths)

    HeavyLightDecomposition(Tree& tree) : mTree(tree), mRoot(0) {
        // no action
    }

    void doHLD(int root) {
        mRoot = root;
        mHeavyPaths.clear();
        mHeavyPathIndex.resize(mTree.mN, -1);

        vector<bool> visited(mTree.mN);

        queue<int> Q;
        Q.push(root);
        visited[root] = true;
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (int v : mTree.mE[u]) {
                if (visited[v])
                    continue;
                Q.push(v);
                visited[v] = true;
            }

            if (u == root)
                continue;

            int p = mTree.mP[0][u];

            if (mTree.mTreeSize[u] * 2 >= mTree.mTreeSize[p] && p != root) {
                // heavy path -> add u to parent's heavy path
                int parentPath = mHeavyPathIndex[p];
                mHeavyPaths[parentPath].push_back(u);
                mHeavyPathIndex[u] = parentPath;
            } else {
                // light path -> make new heavy path
                mHeavyPathIndex[u] = mHeavyPaths.size();
                mHeavyPaths.push_back(vector<int>(2));
                mHeavyPaths.back()[0] = p;
                mHeavyPaths.back()[1] = u;
            }
        }
    }

    // PRECONDITION: v is not the root
    int indexInPath(int v) {
        assert(v != mRoot);
        int topOfPath = mHeavyPaths[mHeavyPathIndex[v]][0];
        return mTree.mLevel[mTree.mP[0][v]] - mTree.mLevel[topOfPath];
    }

    // PRECONDITION: path is the index of v's heavy path
    // PRECONDITION: v is not the root
    int indexInPath(int path, int v) {
        assert(v != mRoot);
        int topOfPath = mHeavyPaths[path][0];
        return mTree.mLevel[mTree.mP[0][v]] - mTree.mLevel[topOfPath];
    }
};
