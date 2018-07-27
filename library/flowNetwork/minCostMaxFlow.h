#pragma once

template <typename T, const T INF = 0x3f3f3f3f>
struct MinCostMaxFlow {
    struct Edge {
        int to;                 // v
        int revIndex;           // for (v -> u)

        T   flow;
        T   cost;

        T   capacity;           // capacity for flow
    };

    int N;                      // the number of vertices
    vector<vector<Edge>> edges;
    vector<bool> visited;
    vector<T> cost;

    MinCostMaxFlow() : N(0) {
    }

    explicit MinCostMaxFlow(int n) : N(n), edges(N), visited(N), cost(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<Edge>>(N);
        visited = vector<bool>(N);
        cost = vector<T>(N);
    }

    // add edges to a directed graph
    void addEdge(int u, int v, T capacity, T cost) {
        int uN = int(edges[u].size());
        int vN = int(edges[v].size());
        edges[u].push_back(Edge{ v, vN, 0, cost, capacity });
        edges[v].push_back(Edge{ u, uN, 0, -cost, 0 });
    }

    void clearFlow() {
        for (auto& vec : edges) {
            for (auto& e : vec)
                e.flow = 0;
        }
    }

    // time complexity: O((VE + V^2) * flow)
    // (flow, cost)
    pair<T, T> calcMinCostMaxFlow(int s, int t) {
        //clearFlow();

        pair<T, T> res;

        while (bfsUpdateCost(s, t)) {
            auto r = dfsUpdateFlow(s, t, INF);
            res.first += r.first;
            res.second += r.second;
        }

        return res;
    }

private:
    // update cost
    bool bfsUpdateCost(int s, int t) {
        fill(cost.begin(), cost.end(), INF);
        fill(visited.begin(), visited.end(), false);

        cost[s] = 0;

        queue<int> Q;
        Q.push(s);
        visited[s] = true;

        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (auto& e : edges[u]) {
                if (e.flow < e.capacity && cost[e.to] > cost[u] + e.cost) {
                    cost[e.to] = cost[u] + e.cost;
                    if (!visited[e.to]) {
                        Q.push(e.to);
                        visited[e.to] = true;
                    }
                }
            }

            visited[u] = false;
        }

        //return cost[t] < 0;
        return cost[t] < INF;
    }

    // update flow and return (flow, cost)
    pair<T, T> dfsUpdateFlow(int u, int t, T flow) {
        if (u == t)
            return make_pair(flow, flow * cost[u]);

        visited[u] = true;

        pair<T, T> res;
        for (auto& e : edges[u]) {
            if (!visited[e.to] && e.flow < e.capacity && cost[e.to] == cost[u] + e.cost) {
                auto r = dfsUpdateFlow(e.to, t, min(flow - res.first, e.capacity - e.flow));

                res.first += r.first;
                res.second += r.second;

                e.flow += r.first;
                edges[e.to][e.revIndex].flow -= r.first;

                if (res.first == flow)
                    break;
            }
        }
        return res;
    }
};