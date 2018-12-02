#pragma once

/*
<How to use>
1. initialize : init()
2. add edges : addEdge()
3. prepare searching : build()
*/
template <typename T>
struct TreeCentroidDecompositionSolver {
    int N;
    vector<vector<int>> edges;
    vector<T> values;               // compressed values (0 <= values[i] < M)

    int M;                          // sizeof uniqueValues
    vector<T> uniqueValues;         // unique & sorted values of values's original values

    vector<int> treeSize;
    vector<int> bigChild;

    int ctRoot;
    vector<int> ctParent;
    vector<bool> ctMark;
    vector<vector<int>> ctDist;     // ctDist[u] = { distance from u to the nearest ancestor in centroid tree, next centroid, ..., the farthest ancestor }

    vector<vector<pair<int, T>>> addNodes; // addNodes[i] = all descendants of i to add, addNodes[i][j] = { distance, value }
    vector<vector<pair<int, T>>> subNodes; // subNodes[i] = all descendants of i to subtract, subNodes[i][j] = { distance, value }

    void init(const T v[], int N) {
        this->N = N;
        edges.resize(N);
        values.assign(v, v + N);
        treeSize.resize(N);
        bigChild.resize(N);

        ctParent.resize(N);
        ctMark.resize(N);
        ctDist.resize(N);

        addNodes.resize(N);
        subNodes.resize(N);

        //---

        // must compress values
        uniqueValues.resize(N);
        for (int i = 0; i < N; i++)
            uniqueValues[i] = values[i];
        sort(uniqueValues.begin(), uniqueValues.end());
        uniqueValues.erase(unique(uniqueValues.begin(), uniqueValues.end()), uniqueValues.end());
        M = int(uniqueValues.size());

        for (int i = 0; i < N; i++)
            values[i] = int(lower_bound(uniqueValues.begin(), uniqueValues.end(), values[i]) - uniqueValues.begin());
    }

    void init(const vector<T>& v) {
        init(v.data(), int(v.size()));
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    // void initOp();
    // void addOp(int node, const vector<pair<int, T>>& addNodes);
    // void subOp(int node, const vector<pair<int, T>>& subNodes);
    void build(const function<void()>& initOp,
               const function<void(int, const vector<pair<int, T>>&)>& addOp,
               const function<void(int, const vector<pair<int, T>>&)>& subOp) {
        prepareSearch(initOp, addOp, subOp);
    }

private:
    int dfsSize(int u, int parent = -1) {
        treeSize[u] = 1;
        bigChild[u] = -1;
        for (int v : edges[u]) {
            if (v == parent || ctMark[v])
                continue;

            treeSize[u] += dfsSize(v, u);
            if (bigChild[u] < 0 || treeSize[bigChild[u]] < treeSize[v])
                bigChild[u] = v;
        }
        return treeSize[u];
    }

    void dfsDistance(int u, int parent = -1, int d = 0) {
        ctDist[u].push_back(d);
        for (int v : edges[u]) {
            if (v == parent || ctMark[v])
                continue;
            dfsDistance(v, u, d + 1);
        }
    }

    int dfsCentroidDecomposition(int u) {
        int n = dfsSize(u);
        while (bigChild[u] >= 0 && 2 * treeSize[bigChild[u]] > n)
            u = bigChild[u];

        dfsDistance(u, u);
        ctMark[u] = true;
        for (int v : edges[u]) {
            if (ctMark[v])
                continue;
            ctParent[dfsCentroidDecomposition(v)] = u;
        }

        return u;
    }

    void doCentroidDecomposition() {
        ctRoot = dfsCentroidDecomposition(0);
        ctParent[ctRoot] = -1;

        for (int i = 0; i < N; i++)
            reverse(begin(ctDist[i]), end(ctDist[i]));    // sort ancestors of centroid tree by distance
    }

    //-----------------------------------------------------------------------------

    // void initOp();
    // void addOp(int node, const vector<pair<int, T>>& addNodes);
    // void subOp(int node, const vector<pair<int, T>>& subNodes);
    void prepareSearch(const function<void()>& initOp,
                       const function<void(int, const vector<pair<int, T>>&)>& addOp,
                       const function<void(int, const vector<pair<int, T>>&)>& subOp) {
        doCentroidDecomposition();

        // prepare search by distance

        for (int i = 0; i < N; i++) {
            int v = i, last = -1, at = 0;
            while (v >= 0) {
                addNodes[v].emplace_back(ctDist[i][at], values[i]);
                if (last >= 0)
                    subNodes[last].emplace_back(ctDist[i][at], values[i]);

                last = v;
                v = ctParent[v];
                at++;
            }
        }

        initOp();

        for (int v = 0; v < N; v++) {
            sort(begin(addNodes[v]), end(addNodes[v]));   // sort all descendant nodes by distance in a centroid tree
            sort(begin(subNodes[v]), end(subNodes[v]));   // sort all descendant nodes by distance in a centroid tree

            if (!addNodes[v].empty())
                addOp(v, addNodes[v]);

            if (!subNodes[v].empty())
                subOp(v, subNodes[v]);
        }
    }
};
