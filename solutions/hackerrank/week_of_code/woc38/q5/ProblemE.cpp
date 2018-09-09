// https://www.hackerrank.com/contests/w38/challenges/cargo-delivery

#include <memory.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cctype>
#include <cstring>
#include <climits>
#include <cmath>
#include <vector>
#include <string>
#include <memory>
#include <numeric>
#include <limits>
#include <functional>
#include <tuple>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iostream>

using namespace std;

#define PROFILE_START(i)    clock_t start##i = clock()
#define PROFILE_STOP(i)     fprintf(stderr, "elapsed time (" #i ") = %f\n", double(clock() - start##i) / CLOCKS_PER_SEC)

typedef long long           ll;
typedef unsigned long long  ull;

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
        int uN = (int)edges[u].size();
        int vN = (int)edges[v].size();
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

    // time complexity: O((VE + V^2) * flow)
    // (flow, cost)
    pair<T, T> calcMinCostMaxFlow(int s, int t, T maxFlow) {
        //clearFlow();

        pair<T, T> res;

        while (res.first < maxFlow && bfsUpdateCost(s, t)) {
            auto r = dfsUpdateFlow(s, t, maxFlow - res.first);
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

template <typename T, const T INF = 0x3f3f3f3f>
struct MinCostMaxFlowPotential {
    struct Edge {
        int to;                 // v
        int revIndex;           // for (v -> u)

        T   flow;
        T   cost;

        T   capacity;           // capacity for flow
    };

    int N;                      // the number of vertices
    vector<vector<Edge>> edges;

    MinCostMaxFlowPotential() : N(0) {
    }

    explicit MinCostMaxFlowPotential(int n) : N(n), edges(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<Edge>>(N);
    }

    // add edges to a directed graph
    void addEdge(int u, int v, T capacity, T cost) {
        int uN = (int)edges[u].size();
        int vN = (int)edges[v].size();
        edges[u].push_back(Edge{ v, vN, 0, cost, capacity });
        edges[v].push_back(Edge{ u, uN, 0, -cost, 0 });
    }

    void clearFlow() {
        for (auto& vec : edges) {
            for (auto& e : vec)
                e.flow = 0;
        }
    }

    // time complexity: O(min(V * E^2 * log(V), E * log(V) * flow))
    // (flow, cost)
    pair<T, T> calcMinCostMaxFlow(int s, int t, T maxFlow = INF) {
        //clearFlow();

        // bellmanFord can be skipped if edges' costs are non-negative
        vector<T> potential(N, INF);
        bellmanFord(s, potential);

        vector<T> currFlow(N);
        vector<pair<int, int>> parent(N);  // (u, edge index in u's edges)

        pair<T, T> res;
        while (res.first < maxFlow) {
            priority_queue<pair<T, int>> Q;
            Q.emplace(T(0), s);

            vector<T> priority(N, INF);
            priority[s] = 0;

            vector<bool> finished(N);
            currFlow[s] = INF;
            while (!Q.empty() && !finished[t]) {
                auto uPriority = -Q.top().first;
                auto u = Q.top().second;
                Q.pop();

                if (uPriority != priority[u])
                    continue;

                finished[u] = true;
                for (int i = 0; i < (int)edges[u].size(); i++) {
                    Edge& e = edges[u][i];
                    if (e.flow >= e.capacity)
                        continue;

                    int v = e.to;
                    T newPriority = priority[u] + e.cost + potential[u] - potential[v];
                    if (priority[v] > newPriority) {
                        priority[v] = newPriority;
                        Q.emplace(-newPriority, v);

                        parent[v] = make_pair(u, i);

                        currFlow[v] = min(currFlow[u], e.capacity - e.flow);
                    }
                }
            }
            if (priority[t] == INF)
                break;

            for (int i = 0; i < N; i++) {
                if (finished[i])
                    potential[i] += priority[i] - priority[t];
            }

            T flow = min(currFlow[t], maxFlow - res.first);
            res.first += flow;
            for (int v = t; v != s; v = parent[v].first) {
                int u = parent[v].first;
                Edge& e = edges[u][parent[v].second];

                e.flow += flow;
                edges[e.to][e.revIndex].flow -= flow;

                res.second += flow * e.cost;
            }
        }

        return res;
    }

private:
    void bellmanFord(int s, vector<T>& dist) {
        dist[s] = 0;

        vector<bool> inQ(N);

        queue<int> Q;
        Q.push(s);
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();
            inQ[u] = false;

            for (int i = 0; i < (int)edges[u].size(); i++) {
                auto& e = edges[u][i];
                if (e.flow >= e.capacity)
                    continue;

                int v = e.to;
                if (dist[v] > dist[u] + e.cost) {
                    dist[v] = dist[u] + e.cost;
                    if (!inQ[v]) {
                        Q.push(v);
                        inQ[v] = true;
                    }
                }
            }
        }
    }
};

template <typename T, const T INF = 0x3f3f3f3f>
struct MinCostMaxFlowBellmanFord {
    struct Edge {
        int to;                 // v
        int revIndex;           // for (v -> u)

        T   flow;
        T   cost;

        T   capacity;           // capacity for flow
    };

    int N;                      // the number of vertices
    vector<vector<Edge>> edges;

    MinCostMaxFlowBellmanFord() : N(0) {
    }

    explicit MinCostMaxFlowBellmanFord(int n) : N(n), edges(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<Edge>>(N);
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

    // time complexity: O(min(V^2 * E^2, V * E * flow))
    // (flow, cost)
    pair<T, T> calcMinCostMaxFlow(int s, int t, T maxFlow = INF) {
        //clearFlow();

        pair<T, T> res;
        while (res.first < maxFlow) {
            vector<T> dist(N, INF);
            vector<T> currFlow(N);
            vector<pair<int, int>> parent(N);  // (u, edge index in u's edges)

            bellmanFord(s, dist, currFlow, parent);
            if (dist[t] == INF)
                break;

            T flow = min(currFlow[t], maxFlow - res.first);
            res.first += flow;
            for (int v = t; v != s; v = parent[v].first) {
                int u = parent[v].first;
                Edge& e = edges[u][parent[v].second];

                e.flow += flow;
                edges[e.to][e.revIndex].flow -= flow;

                res.second += flow * e.cost;
            }
        }

        return res;
    }

private:
    void bellmanFord(int s, vector<T>& dist, vector<T>& currFlow, vector<pair<int, int>>& parent) {
        dist[s] = 0;
        currFlow[s] = INF;

        vector<bool> inQ(N);

        queue<int> Q;
        Q.push(s);
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();
            inQ[u] = false;

            for (int i = 0; i < int(edges[u].size()); i++) {
                auto& e = edges[u][i];
                if (e.flow >= e.capacity)
                    continue;

                int v = e.to;
                if (dist[v] > dist[u] + e.cost) {
                    dist[v] = dist[u] + e.cost;

                    parent[v] = make_pair(u, i);
                    currFlow[v] = min(currFlow[u], e.capacity - e.flow);
                    if (!inQ[v]) {
                        Q.push(v);
                        inQ[v] = true;
                    }
                }
            }
        }
    }
};

template <typename T, const T INF = 0x3f3f3f3f>
struct MinCostMaxFlowSPFA {
    struct Edge {
        int to;                 // v
        int revIndex;           // for (v -> u)

        T   flow;
        T   cost;

        T   capacity;           // capacity for flow
    };

    int N;                      // the number of vertices
    vector<vector<Edge>> edges;

    MinCostMaxFlowSPFA() : N(0) {
    }

    explicit MinCostMaxFlowSPFA(int n) : N(n), edges(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<Edge>>(N);
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

    // time complexity: O(V * E * flow) => practical O((V + E) * flow)
    // (flow, cost)
    pair<T, T> calcMinCostMaxFlow(int s, int t, T maxFlow = INF) {
        //clearFlow();

        vector<bool> inQ(N);

        pair<T, T> res;
        while (res.first < maxFlow) {
            vector<pair<int, int>> parent(N, make_pair(-1, -1));  // (u, edge index in u's edges)

            vector<T> dist(N, INF);
            dist[s] = 0;

            queue<int> Q;
            Q.push(s);
            while (!Q.empty()) {
                int u = Q.front();
                Q.pop();
                inQ[u] = false;

                for (int i = 0; i < int(edges[u].size()); i++) {
                    Edge& e = edges[u][i];
                    if (e.flow >= e.capacity)
                        continue;

                    int v = e.to;
                    if (dist[v] > dist[u] + e.cost) {
                        dist[v] = dist[u] + e.cost;
                        parent[v] = make_pair(u, i);
                        if (!inQ[v]) {
                            Q.push(v);
                            inQ[v] = true;
                        }
                    }
                }
            }
            if (parent[t].first < 0)
                break;

            T flow = maxFlow - res.first;
            T cost = 0;
            for (int v = t; v != s; v = parent[v].first) {
                int u = parent[v].first;
                Edge& e = edges[u][parent[v].second];
                if (flow > e.capacity - e.flow)
                    flow = e.capacity - e.flow;
                cost += e.cost;
            }
            for (int v = t; v != s; v = parent[v].first) {
                int u = parent[v].first;
                Edge& e = edges[u][parent[v].second];
                e.flow += flow;
                edges[e.to][e.revIndex].flow -= flow;
            }
            res.first += flow;
            res.second += flow * cost;
        }

        return res;
    }
};

template <typename G>
void initGraph(G& g, int cap) {
    for (auto& vec : g.edges) {
        for (auto& e : vec) {
            e.flow = 0;
            if (e.capacity > 0 && e.capacity != 0x3f3f3f3f)
                e.capacity = cap;
        }
    }
}

// Complete the minimumBrokenness function below.
int minimumBrokenness(int N, int M, int K, int T) {
    MinCostMaxFlow<int> G(N);
    //MinCostMaxFlowPotential<int> G(N);
    //MinCostMaxFlowBellmanFord<int> G(N);
    //MinCostMaxFlowSPFA<int> G(N);
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        G.addEdge(u, v, N, 0);
        G.addEdge(v, u, N, 0);
        G.addEdge(u, v, 0x3f3f3f3f, 1);
        G.addEdge(v, u, 0x3f3f3f3f, 1);
    }

    int lo = 0, hi = K - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        bool possible = true;
        {
            initGraph(G, mid + 1);
            auto r = G.calcMinCostMaxFlow(0, N - 1, K);
            possible = (r.first >= K && r.second <= T);
        }
        if (possible)
            hi = mid - 1;
        else
            lo = mid + 1;
    }

    return lo;
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, K, T;
    cin >> N >> M >> K >> T;

    cout << minimumBrokenness(N, M, K, T) << endl;

    return 0;
}
