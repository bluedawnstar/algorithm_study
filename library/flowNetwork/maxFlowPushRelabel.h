#pragma once

// https://www.geeksforgeeks.org/push-relabel-algorithm-set-1-introduction-and-illustration/
// https://www.geeksforgeeks.org/push-relabel-algorithm-set-2-implementation/

// Push-Relabel
template <typename T>
struct MaxFlowPushRelabel {
    static const T INF = T(0x3f3f3f3f);

    struct Vertex {
        int height;
        T   flow;
    };

    struct Edge {
        int to;         // v
        int revIndex;   // for (v -> u)

        T   flow;
        T   capacity;
    };

    int N;
    vector<Vertex> vertices;
    vector<vector<Edge>> edges;

    MaxFlowPushRelabel() : N(0) {
    }

    explicit MaxFlowPushRelabel(int n) : N(n), vertices(N, Vertex{ 0, 0 }), edges(N) {
    }

    void init(int n) {
        N = n;
        vertices = vector<Vertex>(N, Vertex{ 0, 0 });
        edges = vector<vector<Edge>>(N);
    }

    // add edges to a directed graph
    void addEdge(int u, int v, T capacity, T capacityRev) {
        int uN = (int)edges[u].size();
        int vN = (int)edges[v].size();
        edges[u].push_back(Edge{ v, vN, 0, capacity });
        edges[v].push_back(Edge{ u, uN, 0, capacityRev });
    }

    void clearFlow() {
        for (auto& vec : edges) {
            for (auto& e : vec)
                e.flow = 0;
        }
        for (auto& v : vertices) {
            v.height = 0;
            v.flow = 0;
        }
    }

    // O(V^3)
    T calcMaxFlow(int s, int t) {
        //clearFlow();

        preflow(s);

        int u;
        while ((u = findOverflowVertex(s, t)) >= 0) {
            if (!push(u))
                relabel(u);
        }

        return vertices[t].flow;
    }

private:
    void preflow(int s) {
        vertices[s].height = N;

        for (auto& e : edges[s]) {
            e.flow = e.capacity;
            vertices[e.to].flow += e.flow;
            edges[e.to][e.revIndex].flow = -e.flow;
        }
    }

    int findOverflowVertex(int s, int t) {
        for (int u = 0; u < N; u++) {
            if (u == s || u == t)
                continue;

            if (vertices[u].flow > 0)
                return u;
        }

        return -1;
    }

    bool push(int u) {
        for (auto& e : edges[u]) {
            if (e.flow >= e.capacity)
                continue;

            if (vertices[u].height > vertices[e.to].height) {
                T flow = min(vertices[u].flow, e.capacity - e.flow);
                vertices[u].flow -= flow;
                vertices[e.to].flow += flow;

                e.flow += flow;
                edges[e.to][e.revIndex].flow -= flow;

                return true;
            }
        }
        return false;
    }

    void relabel(int u) {
        T minHeight = INF;

        for (auto& e : edges[u]) {
            if (e.flow >= e.capacity)
                continue;

            if (minHeight > vertices[e.to].height)
                minHeight = vertices[e.to].height;
        }

        if (minHeight != INF)
            vertices[u].height = minHeight + 1;
    }
};
