#pragma once

/*
  https://www.codechef.com/problems/DENSEGRP
  https://discuss.codechef.com/t/densegrp-editorial/86514

    ex)

              1
            /    \
           v      v
          2        3
        /   \    /   \
       v     v  v     v
      4      5  6     7
       \    /    \   /
        v  v      v v
         2'       3'   ^
          \      /  |  |
           v    v   |  |
             1'     |  |
                    v  |
               (from[0],to[0]), ..., (from[i], to[i]), ...
*/
struct RangeBasedDenseGraph {
    const int INF = 0x3f3f3f3f;

    int N;
    vector<vector<pair<int, bool>>> segTreeGraph;    // [u] = { (v, one more step?), ... }

    // - 0 <= x < n
    // - edges : from[u] -> to[u]
    // O((N + M)*logN)
    void build(int n, const vector<pair<int, int>>& from, const vector<pair<int, int>>& to) {
        N = n;
        segTreeGraph = vector<vector<pair<int, bool>>>(6 * n + int(from.size()));

        dfsBuildTree(1, 0, n - 1);
        for (int i = 0; i < int(from.size()); i++) {
            dfsBuildGraph(1, 0, n - 1, from[i].first, from[i].second, i, false);
            dfsBuildGraph(1, 0, n - 1, to[i].first, to[i].second, i, true);
        }
    }

    int findNode(int x) const {
        return dfsFind(1, 0, N - 1, x);
    }

    // shortest path from src to dst
    //  - src : source
    //  - dst : destination
    // return shortest distance from src to dst, -1 if there is no path
    // O((N + M)*logN)
    int shortestPath(int srcX, int dstX) const {
        int srcNode = findNode(srcX);
        int dstNode = findNode(dstX);

        vector<int> dist(segTreeGraph.size(), INF);
        deque<int> Q;

        dist[srcNode] = 0;
        Q.push_front(srcNode);

        while (!Q.empty()) {
            int v = Q.front();
            Q.pop_front();

            for (auto& e : segTreeGraph[v]) {
                int u = e.first;
                bool w = e.second;
                if (dist[v] + w < dist[u]) {
                    dist[u] = dist[v] + w;
                    if (w)
                        Q.push_back(u);
                    else
                        Q.push_front(u);
                }
            }
        }

        if (dist[dstNode] < INF)
            return dist[dstNode];

        return -1;
    }

private:
    void dfsBuildTree(int node, int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight) {
            segTreeGraph[node >> 1].emplace_back(node, false);
            segTreeGraph[node].emplace_back(4 * N + (node >> 1), false);
            return;
        }

        segTreeGraph[node >> 1].emplace_back(node, false);
        segTreeGraph[4 * N + node].emplace_back(4 * N + (node >> 1), false);

        int mid = (nodeLeft + nodeRight) / 2;
        dfsBuildTree(2 * node, nodeLeft, mid);
        dfsBuildTree(2 * node + 1, mid + 1, nodeRight);
    }

    int dfsFind(int node, int nodeLeft, int nodeRight, int x) const {
        if (nodeLeft == nodeRight)
            return node;

        int mid = (nodeLeft + nodeRight) / 2;
        if (x <= mid)
            return dfsFind(2 * node, nodeLeft, mid, x);
        else
            return dfsFind(2 * node + 1, mid + 1, nodeRight, x);
    }

    void dfsBuildGraph(int node, int nodeLeft, int nodeRight, int left, int right, int edgeIndex, bool isTo) {
        if (nodeRight < left || right < nodeLeft)
            return;

        if (left <= nodeLeft && nodeRight <= right) {
            if (isTo) {
                segTreeGraph[6 * N + edgeIndex].emplace_back(node, true);
            } else {
                if (nodeLeft == nodeRight)
                    segTreeGraph[node].emplace_back(6 * N + edgeIndex, false);
                else
                    segTreeGraph[4 * N + node].emplace_back(6 * N + edgeIndex, false);
            }
            return;
        }

        int mid = (nodeLeft + nodeRight) / 2;
        dfsBuildGraph(node * 2, nodeLeft, mid, left, right, edgeIndex, isTo);
        dfsBuildGraph(node * 2 + 1, mid + 1, nodeRight, left, right, edgeIndex, isTo);
    }
};
