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
struct RangeBasedDenseGraph2 {
    const int INF = 0x3f3f3f3f;

    int N;
    vector<vector<pair<int, bool>>> segTreeGraph;    // [u] = { (v, one more step?), ... }

    // - 0 <= x < n
    // - edges : from[u] -> to[u]
    // O((N + M)*logN)
    void build(int n, const vector<pair<int, int>>& from, const vector<pair<int, int>>& to) {
        N = n + (n & 1);
        segTreeGraph = vector<vector<pair<int, bool>>>(3 * N + int(from.size()));

        buildTree(n);
        buildGraph(from, to);
    }

    // shortest path from src to dst
    //  - srcX : source
    //  - dstX : destination
    // return shortest distance from srcX to dstX, -1 if there is no path
    // O((N + M)*logN)
    int shortestPath(int srcX, int dstX) const {
        int srcNode = N + srcX;
        int dstNode = N + dstX;

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
    void buildTree(int n) {
        for (int i = 0; i < n; i++) {
            int j = (N + i) >> 1;
            segTreeGraph[j].emplace_back(N + i, false);
            segTreeGraph[N + i].emplace_back(2 * N + j, false);
        }

        for (int i = N - 1; i > 0; i--) {
            int j = i >> 1;
            segTreeGraph[j].emplace_back(i, false);
            segTreeGraph[2 * N + i].emplace_back(2 * N + j, false);
        }
    }

    void buildGraph(const vector<pair<int, int>>& from, const vector<pair<int, int>>& to) {
        int M = int(from.size());
        for (int i = 0; i < M; i++) {
            for (int L = N + from[i].first, R = N + from[i].second + 1; L < R; L >>= 1, R >>= 1) {
                if (L & 1) {
                    if (L >= N)
                        segTreeGraph[L].emplace_back(3 * N + i, false);
                    else
                        segTreeGraph[2 * N + L].emplace_back(3 * N + i, false);
                    L++;
                }
                if (R & 1) {
                    if (--R >= N)
                        segTreeGraph[R].emplace_back(3 * N + i, false);
                    else
                        segTreeGraph[2 * N + R].emplace_back(3 * N + i, false);
                }
            }

            for (int L = N + to[i].first, R = N + to[i].second + 1; L < R; L >>= 1, R >>= 1) {
                if (L & 1)
                    segTreeGraph[3 * N + i].emplace_back(L++, true);
                if (R & 1)
                    segTreeGraph[3 * N + i].emplace_back(--R, true);
            }
        }
    }
};
