#pragma once

// 2-Satisfiability (2-SAT)
//
// <Property #1>
//   (A or B) <=> (not A => B) and (not B => A)
//
// <Property #2>
//  1) if edge(X -> not X) exists, X must be false.
//  2) if edge(not X -> X) exists, X must be true.
//  3) if edge(X -> not X) and edge(not X -> X) exist, it is impossible.
struct SAT2 {
    static int nodeIndex(int variableIndex, bool notVar) {
        return (variableIndex << 1) + int(notVar);
    }

    // even index: X, odd index: not X
    static vector<bool> solve2Sat(int variableN, const vector<int>& A, const vector<int>& B) {
        int N = variableN * 2;

        vector<vector<int>> edges(N);
        for (int i = 0; i < int(A.size()); i++) {
            edges[A[i] ^ 1].push_back(B[i]);
            edges[B[i] ^ 1].push_back(A[i]);
        }

        vector<bool> used(N);
        vector<int> order;
        order.reserve(N);
        for (int u = 0; u < N; u++)
            if (!used[u])
                dfs1(edges, used, order, u);

        vector<vector<int>> revEdges(N);
        for (int u = 0; u < N; u++) {
            for (int v : edges[u])
                revEdges[v].push_back(u);
        }

        vector<int> component(N, -1);
        for (int i = N - 1, group = 0; i >= 0; i--) {
            int u = order[i];
            if (component[u] == -1)
                dfs2(revEdges, component, u, group++);
        }

        vector<bool> res;

        for (int i = 0; i < N; i++) {
            if (component[i] == component[i ^ 1])
                return res; // impossible
        }

        res.resize(variableN);
        for (int i = 0; i < N; i += 2)
            res[i >> 1] = component[i] > component[i ^ 1];
        return res;
    }

private:
    static void dfs1(const vector<vector<int>>& edges, vector<bool>& used, vector<int>& order, int u) {
        used[u] = true;
        for (int v : edges[u])
            if (!used[v])
                dfs1(edges, used, order, v);

        order.push_back(u);
    }

    static void dfs2(const vector<vector<int>>& revEdges, vector<int>& component, int u, int group) {
        component[u] = group;
        for (int v : revEdges[u])
            if (component[v] == -1)
                dfs2(revEdges, component, v, group);
    }
};