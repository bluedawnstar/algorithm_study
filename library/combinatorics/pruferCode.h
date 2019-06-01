#pragma once

// Combinatorics on a tree

// https://en.wikipedia.org/wiki/Pr%C3%BCfer_sequence
//  "The Prufer sequence (also Prufer code or Prufer numbers) of a labeled tree is a unique sequence associated with the tree."
//  "The sequence for a tree on n vertices has length n - 2."
//  "Prufer sequences were first used by Heinz Prufer to prove Cayley's formula in 1918."

// https://forthright48.com/prufer-code-linear-representation-of-a-labeled-tree/
//  "Every labeled tree of n nodes can be uniquely represented as an array of n - 2 integers and vice versa."
//
//  <Properties>
//   1. If a node has degree d, then that node will appear in prufer code exactly d-1 times.
//   2. Leaves never appear in Prufer Code.

struct PruferCode {
    // convert a tree to a Prufer Code, O(N*logN)
    static vector<int> convertTreeToPruferCode(const vector<vector<int>>& edges) {
        int N = int(edges.size());

        vector<int> P(N);
        vector<int> D(N);
        priority_queue<int> leaves;

        const function<void(int,int)> dfs = [&](int u, int parent) {
            P[u] = parent;
            D[u] = int(edges[u].size());
            for (int v : edges[u]) {
                if (v == parent)
                    continue;
                dfs(v, u);
            }
            if (D[u] == 1)
                leaves.push(-u);
        };
        dfs(N - 1, -1);

        vector<int> pruferCode;
        int need = N - 2;
        for (int cnt = N - 2; cnt > 0; cnt--) {
            int v = -leaves.top();
            leaves.pop();

            int u = P[v];
            if (--D[u] == 1)
                leaves.push(-u);
            pruferCode.push_back(u);
        }

        return pruferCode;
    }

    //---

    // convert a Prufer code to a tree, O(N*logN)
    static void convertPruferCodeToTree(const vector<int>& pruferCode, vector<vector<int>>& edges) {
        int N = int(pruferCode.size()) + 2;

        edges.clear();
        edges.resize(N);

        vector<int> D(N);

        for (auto u : pruferCode)
            ++D[u];

        priority_queue<int> leaves;
        for (int u = 0; u < N; u++) {
            if (D[u] == 0)
                leaves.push(-u);
        }

        for (auto u : pruferCode){
            int v = -leaves.top();
            leaves.pop();

            edges[u].push_back(v);
            edges[v].push_back(u);

            if (--D[u] == 0)
                leaves.push(-u);
        }

        int u = -leaves.top();
        leaves.pop();
        int v = -leaves.top();

        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    static vector<vector<int>> convertPruferCodeToTree(const vector<int>& pruferCode) {
        vector<vector<int>> edges;
        convertPruferCodeToTree(pruferCode, edges);
        return edges;
    }
};
