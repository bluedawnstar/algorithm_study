// Dijkstra, O(nlogn)
class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times, int N, int K) {
        if (N <= 1)
            return 0;

        const int INF = 0x3f3f3f3f;

        vector<vector<pair<int, int>>> edges(N); // (to, weight)
        for (auto& it : times)
            edges[it[0] - 1].emplace_back(it[1] - 1, it[2]);

        vector<int> weight(N, INF);
        priority_queue<pair<int, int>> Q;        // (weight, node)

        weight[K - 1] = 0;
        Q.emplace(0, K - 1);
        while (!Q.empty()) {
            int w = -get<0>(Q.top());
            int u = get<1>(Q.top());
            Q.pop();

            if (weight[u] < w)
                continue;

            for (auto it : edges[u]) {
                int v = it.first;
                int vw = w + it.second;
                if (weight[v] <= vw)
                    continue;

                weight[v] = vw;
                Q.emplace(-vw, v);
            }
        }

        int farthest = *max_element(weight.begin(), weight.end());
        return (farthest == INF) ? -1 : farthest;
    }
};