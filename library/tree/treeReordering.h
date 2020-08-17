#pragma once

struct TreeReordering {
    struct FenwickTree {
        vector<int> tree;

        FenwickTree() {
        }

        explicit FenwickTree(int n) : tree(n + 1) {
        }

        void init(int n) {
            tree = vector<int>(n + 1);
        }

        int sum(int pos) const {
            pos++;

            int res = 0;
            while (pos > 0) {
                res += tree[pos];
                pos &= pos - 1;         // clear lowest bit
            }

            return res;
        }

        int sumRange(int left, int right) const {
            int res = sum(right);
            if (left > 0)
                res -= sum(left - 1);
            return res;
        }

        void add(int pos, int val) {
            pos++;

            while (pos < int(tree.size())) {
                tree[pos] += val;
                pos += pos & -pos;      // add lowest bit
            }
        }

        void addRange(int left, int right, int val) {
            add(left, val);
            if (right + 1 < int(tree.size()) - 1)
                add(right + 1, -val);
        }
    };

    struct RangeSet : public map<int, int> {
        RangeSet() {
        }

        void insert(int left, int right) {
            (*this)[left] = right;
        }

        void remove(int left, int right) {
            auto itL = this->upper_bound(left);
            auto itR = this->upper_bound(right);

            if (itL != this->begin()) {
                --itL;
                if (itL->second < left)
                    ++itL;
            }

            if (itL == itR)
                return;

            int tL = min(left, itL->first);
            int tR = max(right, prev(itR)->second);

            this->erase(itL, itR);
            if (tL < left)
                (*this)[tL] = left - 1;

            if (right < tR)
                (*this)[right + 1] = tR;
        }

        void copyTo(int left, int right, vector<pair<int, int>>& to) {
            auto itL = this->upper_bound(left);

            if (itL != this->begin()) {
                --itL;
                if (itL->second < left)
                    ++itL;
            }

            for (auto it = itL; it != this->end() && it->first <= right; ++it)
                to.emplace_back(max(left, it->first), min(it->second, right));
        }
    };

    //---

    int N;
    vector<vector<int>> edges;
    vector<int> P;

    int currTime;
    vector<pair<int, int>> visitTime;
    vector<int> timeToNode;

    void init(int n) {
        N = n;
        edges = vector<vector<int>>(n);
        P = vector<int>(n);

        currTime = 0;
        visitTime = vector<pair<int, int>>(n);
        timeToNode = vector<int>(n);
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    void build() {
        currTime = 0;
        dfs(0, -1);
    }

    //---

    vector<vector<int>> edges2;
    vector<int> P2;
    vector<int> visitTime2; // first visit time
    vector<int> timeToNode2;

    // reconstruct tree with depth order (first is new root)
    // depthOrder = { new root, depth1-node, depth1-node, ..., depth2-node, depth2-node, ... }
    void reorderTree(const vector<int>& reorderedVertexesByDepth) {
        edges2 = vector<vector<int>>(N);
        P2 = vector<int>(N);
        visitTime2 = vector<int>(N);
        timeToNode2 = vector<int>(N);

        //---

        FenwickTree rootFinder;         // rootFinder.sum(visit time of u) = new root of u
        rootFinder.init(N + 1);

        vector<RangeSet> segments(N);   // segments[u] = { (timeL1, timeR1), (timeL2, timeR2), ... }, segments including u
        segments[0].insert(0, N - 1);

        vector<int> currP(N, -1);
        vector<bool> visited(N);

        vector<pair<int, int>> vec;
        for (int t = 0; t < N; t++) {
            int u = reorderedVertexesByDepth[t];
            visitTime2[u] = t;
            timeToNode2[t] = u;

            visited[u] = true;

            int uL = visitTime[u].first;
            int uR = visitTime[u].second - 1;
            int rootU = rootFinder.sum(uL);

            if (currP[rootU] >= 0)
                edges2[currP[rootU]].push_back(u);
            P2[u] = currP[rootU];

            for (int v : edges[u]) {
                if (v == P[u] || visited[v])
                    continue;

                int vL = visitTime[v].first;
                int vR = visitTime[v].second - 1;

                vec.clear();
                segments[rootU].copyTo(vL, vR, vec);
                for (auto& it : vec) {
                    segments[v].insert(it.first, it.second);
                    rootFinder.addRange(it.first, it.second, v - rootU);
                    currP[v] = u;
                }
            }
            segments[rootU].remove(uL, uR);
            currP[rootU] = u;
        }
    }

private:
    void dfs(int u, int parent) {
        P[u] = parent;
        visitTime[u].first = currTime;
        timeToNode[currTime++] = u;

        for (auto v : edges[u]) {
            if (v != parent)
                dfs(v, u);
        }

        visitTime[u].second = currTime;
    }
};
