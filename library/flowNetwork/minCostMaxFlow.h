#pragma once

template <typename T>
struct MinCostMaxFlow {
    static const T INF = T(0x3f3f3f3f);

    struct Node {
        int to;         // v
        int revIndex;   // for (v -> u)

        T   flow;
        T   cost;
    };

    vector<vector<Node>> edges;
    vector<bool> visited;
    vector<T> cost;

    MinCostMaxFlow() {
    }

    MinCostMaxFlow(int maxN) : edges(maxN + 1), visited(maxN + 1), cost(maxN + 1) {
    }

    void init(int maxN) {
        edges = vector<vector<Node>>(maxN + 1);
        visited = vector<bool>(maxN + 1);
        cost = vector<T>(maxN + 1);
    }

    // add edges to a directed graph
    void addEdge(int u, int v, T flow, T cost) {
        int uN = (int)edges[u].size();
        int vN = (int)edges[v].size();
        edges[u].push_back(Node{ v, vN, flow, cost });
        edges[v].push_back(Node{ u, uN, 0, -cost });
    }

    //### TODO: check stop condition!!!
    bool isStopCondition(int t) const {
        return cost[t] < 0;
    }

    // update cost
    bool bfsUpdateCost(int s, int t) {
        fill(cost.begin(), cost.end(), INF);
        fill(visited.begin(), visited.end(), false);
        //memset(&cost[0], 0x3f, sizeof(cost[0]) * cost.size());
        //memset(&visited[0], 0, sizeof(visited[0]) * visited.size());

        cost[s] = 0;

        queue<int> Q;
        Q.push(s);
        visited[s] = true;

        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (auto& v : edges[u]) {
                if (v.flow > 0 && cost[v.to] > cost[u] + v.cost) {
                    cost[v.to] = cost[u] + v.cost;
                    if (!visited[v.to]) {
                        Q.push(v.to);
                        visited[v.to] = true;
                    }
                }
            }

            visited[u] = false;
        }

        return isStopCondition(t);
    }

    // update flow and return (flow, cost)
    pair<T, T> dfsUpdateFlow(int u, int t, T flow) {
        if (u == t)
            return make_pair(flow, flow * cost[u]);

        visited[u] = true;

        pair<T, T> res;
        for (auto& v : edges[u]) {
            if (!visited[v.to] && v.flow > 0 && cost[v.to] == cost[u] + v.cost) {
                auto r = dfsUpdateFlow(v.to, t, min(flow - res.first, v.flow));

                res.first += r.first;
                res.second += r.second;

                v.flow -= r.first;
                edges[v.to][v.revIndex].flow += r.first;

                if (res.first == flow)
                    break;
            }
        }
        return res;
    }

    // time complexity: O(E^2 * flow * cost)??
    // (flow, cost)
    pair<T, T> doMinCostMaxFlow(int s, int t) {
        pair<T, T> res;

        while (bfsUpdateCost(s, t)) {
            auto r = dfsUpdateFlow(s, t, INF);
            res.first += r.first;
            res.second += r.second;
        }

        return res;
    }
};