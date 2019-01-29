#pragma once

struct BipartiteGraphCheck {
    static bool isBipartiteGraph(const vector<vector<int>>& edges, int start) {
        int N = int(edges.size());
        vector<int> color(N, -1);

        queue<int> Q;
        Q.push(start);
        color[start] = 0;

        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (auto v : edges[u]) {
                if (color[v] == color[u])
                    return false;
                if (color[v] < 0) {
                    color[v] = 1 - color[u];
                    Q.push(v);
                }
            }
        }

        return true;
    }
};
