#pragma once

/*
<How to use>
1. initialize : init()
2. add edges : addEdge()
3. prepare searching : build()
*/
template <typename T>
struct KthValueInDistance {
    struct SimplePersistentSegmentTree {
        int N;
        vector<T> value;
        vector<int> L;
        vector<int> R;
        int root;       // first root;

        SimplePersistentSegmentTree() {
        }

        explicit SimplePersistentSegmentTree(int n) {
            build(n);
        }

        // return root node index
        int build(int n) {
            N = n;
            value.reserve(N * 4);
            L.reserve(N * 4);
            R.reserve(N * 4);
            return root = dfsBuild(0, N - 1);
        }

        // return root node index
        int add(int root, int index, T val) {
            return dfsAdd(root, 0, N - 1, index, val);
        }

    private:
        int dfsBuild(int left, int right) {
            int l = -1, r = -1;
            if (left < right) {
                int mid = (left + right) >> 1;
                l = dfsBuild(left, mid);
                r = dfsBuild(mid + 1, right);
            }
            value.push_back(0);
            L.push_back(l);
            R.push_back(r);
            return int(value.size()) - 1;
        }

        int dfsAdd(int node, int left, int right, int index, T val) {
            T newValue;
            int l = -1, r = -1;
            if (left == right) {
                newValue = value[node] + val;           //TODO: merge operation
            } else {
                int mid = (left + right) >> 1;
                if (index <= mid) {
                    l = dfsAdd(L[node], left, mid, index, val);
                    r = R[node];
                } else {
                    l = L[node];
                    r = dfsAdd(R[node], mid + 1, right, index, val);
                }
                newValue = value[l] + value[r];         //TODO: merge operation
            }
            value.push_back(newValue);
            L.push_back(l);
            R.push_back(r);
            return int(value.size()) - 1;
        }
    };

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

                                    //---

    vector<vector<pair<int, T>>> addNodes; // addNodes[i] = all descendants of i to add, addNodes[i][j] = { distance, value }
    vector<vector<pair<int, T>>> subNodes; // subNodes[i] = all descendants of i to subtract, subNodes[i][j] = { distance, value }

    SimplePersistentSegmentTree distSegTree;
    vector<vector<pair<int, int>>> addRootByDist; // roots of persistent segment tree, addRootByDist[i][j] = { distance, root }
    vector<vector<pair<int, int>>> subRootByDist; // roots of persistent segment tree, subRootByDist[i][j] = { distance, root }

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

        addRootByDist.resize(N);
        subRootByDist.resize(N);

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

    // O(N*(logN)^2)
    void build() {
        prepareSearch();
    }

    //--- query

    // 0 <= node < N, 0 <= d < N, 1 <= kth < N
    // O((logN)^2)
    T query(int u, int maxDist, int kth) {
        vector<pair<int, int>> roots = findSegmentTreeRoots(u, maxDist);
        auto ans = findKthValue(roots, kth);
        return (ans >= 0) ? uniqueValues[ans] : -1;  // uncompress
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

    void prepareSearch() {
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

        distSegTree.build(M);

        for (int v = 0; v < N; v++) {
            sort(begin(addNodes[v]), end(addNodes[v]));   // sort all descendant nodes by distance in a centroid tree
            sort(begin(subNodes[v]), end(subNodes[v]));   // sort all descendant nodes by distance in a centroid tree

            if (!addNodes[v].empty()) {
                auto& nodes = addNodes[v];
                int root = distSegTree.add(distSegTree.root, nodes[0].second, 1);
                for (int i = 1; i < int(nodes.size()); i++) {
                    if (nodes[i - 1].first != nodes[i].first)
                        addRootByDist[v].emplace_back(nodes[i - 1].first, root);
                    root = distSegTree.add(root, nodes[i].second, 1);
                }
                addRootByDist[v].emplace_back(nodes.back().first, root);
            }
            if (!subNodes[v].empty()) {
                auto& nodes = subNodes[v];
                int root = distSegTree.add(distSegTree.root, nodes[0].second, 1);
                for (int i = 1; i < int(nodes.size()); i++) {
                    if (nodes[i - 1].first != nodes[i].first)
                        subRootByDist[v].emplace_back(nodes[i - 1].first, root);
                    root = distSegTree.add(root, nodes[i].second, 1);
                }
                subRootByDist[v].emplace_back(nodes.back().first, root);
            }
        }
    }

    // d = max distance, 0 <= u < N, 0 <= d < N
    // return the roots in the segment tree, for all ancestors in the centroid tree
    vector<pair<int, int>> findSegmentTreeRoots(int u, int maxDist) {
        vector<pair<int, int>> segTrees;

        int v = u, last = -1, at = 0;
        while (v >= 0) {
            int h = maxDist - ctDist[u][at];
            auto it = lower_bound(begin(addRootByDist[v]), end(addRootByDist[v]), make_pair(h + 1, 0));
            if (it != begin(addRootByDist[v])) {
                it--;
                segTrees.emplace_back(it->second, 1);
            }

            if (last >= 0) {
                it = lower_bound(begin(subRootByDist[last]), end(subRootByDist[last]), make_pair(h + 1, 0));
                if (it != begin(subRootByDist[last])) {
                    it--;
                    segTrees.emplace_back(it->second, -1);
                }
            }

            last = v;
            v = ctParent[v];
            at++;
        }

        return segTrees;
    }

    // 1 <= kth
    int findKthValue(vector<pair<int, int>>& segTreeRoots, int kth) {
        int lo = 0, hi = M - 1;
        while (lo < hi) {
            int qt = 0;
            for (auto& r : segTreeRoots)
                qt += r.second * distSegTree.value[distSegTree.L[r.first]];

            int mid = (lo + hi) >> 1;
            if (kth <= qt) {
                for (auto& r : segTreeRoots)
                    r.first = distSegTree.L[r.first];
                hi = mid;
            } else {
                kth -= qt;
                for (auto& r : segTreeRoots)
                    r.first = distSegTree.R[r.first];
                lo = mid + 1;
            }
        }

        int qt = 0;
        for (auto& r : segTreeRoots)
            qt += r.second * distSegTree.value[r.first];

        return kth <= qt ? lo : -1;
    }
};
