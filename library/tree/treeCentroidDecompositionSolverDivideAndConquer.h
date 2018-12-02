#pragma once

/*
<How to use>
1. initialize : init()
2. add edges : addEdge()
3. prepare searching : build()
*/
template <typename T>
struct TreeCentroidDecompositionSolverDivideAndConquer {
    int N;
    vector<vector<int>> edges;
    vector<T> values;               // compressed values (0 <= values[i] < M)

    int M;                          // sizeof uniqueValues
    vector<T> uniqueValues;         // unique & sorted values of values's original values

    // find a center node in a tree
    vector<int> treeSize;
    vector<bool> ctMarked;

    void init(const T v[], int N) {
        this->N = N;
        edges.resize(N);
        values.assign(v, v + N);

        treeSize.resize(N);
        ctMarked.resize(N);

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
    // void addOp(int u);
    // void subOp(int u, int parent);
    void solve(const function<void()>& initOp,
               const function<void(int)>& addOp,
               const function<void(int,int)>& subOp) {
        initOp();
        solve(0, -1, addOp, subOp);
    }


private:
    void dfsSize(int u, int parent) {
        treeSize[u] = 1;
        for (int v : edges[u]) {
            if (v != parent && !ctMarked[v]) {
                dfsSize(v, u);
                treeSize[u] += treeSize[v];
            }
        }
    }

    int findCentroid(int u, int parent, int N) {
        bool isMajor = true;
        for (int v : edges[u]) {
            if (v == parent || ctMarked[v])
                continue;

            int res = findCentroid(v, u, N);
            if (res != -1)
                return res;

            if (treeSize[v] + treeSize[v] > N)
                isMajor = false;
        }

        if (isMajor && 2 * (N - treeSize[u]) <= N)
            return u;

        return -1;
    }

    int findCentroid(int start, int parent) {
        dfsSize(start, parent);
        return findCentroid(start, parent, treeSize[start]);
    }


    void solve(int u, int parent, const function<void(int)>& addOp, const function<void(int,int)>& subOp) {
        int root = findCentroid(u, parent);

        if (parent >= 0)
            subOp(u, parent);

        u = root;
        addOp(u);

        ctMarked[u] = true;
        for (int v : edges[u]) {
            if (!ctMarked[v])
                solve(v, u, addOp, subOp);
        }
    }
};
