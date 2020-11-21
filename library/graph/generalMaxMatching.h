#pragma once

/*
  Blossom Algorithm - general matching for undirected graph
  - O(V^3)

  https://en.wikipedia.org/wiki/Blossom_algorithm
  https://www.secmem.org/blog/2020/04/18/Blossom/
  https://blog.kyouko.moe/20?category=767011
*/
struct GeneralMaxMatching {
    int N;
    vector<vector<int>> edges;

    GeneralMaxMatching() : N(0) {
    }

    GeneralMaxMatching(int n) : N(n), edges(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<int>>(N);
    }

    // add edges to a undirected graph
    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    // Edmonds' Blossom Algorithm : O(V^3)
    int calcMaxMatching() {
        match.assign(N, -1);
        parent.assign(N, 0);

        int res = 0;
        for (int u = 0; u < N; u++) {
            if (match[u] == -1) {
                int v = findPath(u);
                if (v != -1) {
                    augmentPath(v);
                    res++;
                }
            }
        }
        return res;
    }

    vector<pair<int, int>> getLastMaxMatchingEdges() {
        vector<pair<int, int>> res;
        for (int u = 0; u < N; u++) {
            int v = match[u];
            if (v < 0)
                continue;

            if (u < v)
                res.emplace_back(u, v);
        }
        return res;
    }

private:
    vector<int> match;
    vector<int> parent;

    int findPath(int root) {
        parent.assign(N, -1);

        vector<bool> used(N);
        vector<int> base(N);

        iota(base.begin(), base.end(), 0);
        used[root] = true;

        queue<int> Q;
        Q.push(root);
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (int v : edges[u]) {
                if (base[u] == base[v] || match[u] == v)
                    continue;

                if (v == root || match[v] != -1 && parent[match[v]] != -1) {
                    int lca = findLCA(base, u, v);

                    vector<bool> blossom(N);
                    markBlossom(base, blossom, u, v, lca);
                    markBlossom(base, blossom, v, u, lca);

                    for (int i = 0; i < N; ++i) {
                        if (blossom[base[i]]) {
                            base[i] = lca;
                            if (!used[i]) {
                                used[i] = true;
                                Q.push(i);
                            }
                        }
                    }
                } else if (parent[v] == -1) {
                    parent[v] = u;
                    if (match[v] == -1)
                        return v;

                    v = match[v];
                    if (!used[v]) {
                        used[v] = true;
                        Q.push(v);
                    }
                }
            }
        }

        return -1;
    }

    void augmentPath(int v) {
        while (v != -1) {
            int pv = parent[v];
            int ppv = match[pv];
            match[pv] = v;
            match[v] = pv;
            v = ppv;
        }
    }

    int findLCA(vector<int>& base, int u, int v) {
        vector<bool> used(N);

        while (true) {
            u = base[u];
            used[u] = true;

            if (match[u] == -1) // root
                break;
            u = parent[match[u]];
        }

        while (true) {
            v = base[v];
            if (used[v])
                return v;

            v = parent[match[v]];
        }

        return -1;
    }

    void markBlossom(vector<int>& base, vector<bool>& blossom, int u, int v, int lca) {
        for (; base[u] != lca; u = parent[v]) {
            blossom[base[u]] = blossom[base[match[u]]] = true;
            parent[u] = v;
            v = match[u];
        }
    }
};
