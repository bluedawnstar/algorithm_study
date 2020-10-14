#pragma once

/*
  Minimum spanning tree by degree of root node
  https://www.codechef.com/problems/ROOTMST

  1. precondition
    - root node is not articulation point
    - root node has N - 1 edges, it means root node has direct edges to all other nodes

  2. algorithm
    1) connect all edges to root node(node 0) and sum weight of the edges
       -> make N - 1 groups except node 0

    2) find group of min increasing weight

       new_cost_sum = current_cost_sum +
                      (weight_of_new_connected_edge_from_a_group_to_another_group - weight_of_erased_edge_from_the_root)

    3) cut a edge from root, merge two groups, and update cost

    4) repeat 2, 3
*/
struct SpecialMST {
    struct UnionFind {
        vector<int> parent;
        vector<int> rootEdgeWeight;         // rootEdgeWeight[i] = edge weight between root and i
        vector<bool> used;

        vector<set<pair<int, int>>> S;      // (edge-weight, group-to)

        UnionFind() {
        }

        explicit UnionFind(int N, const int weightOfRootEdges[]) {
            init(N, weightOfRootEdges);
        }

        explicit UnionFind(int N, const vector<int>& weightOfRootEdges) {
            init(N, weightOfRootEdges.data());
        }

        void init(int N, const int weightOfRootEdges[]) {
            parent.resize(N);
            used.assign(N, true);
            rootEdgeWeight.assign(weightOfRootEdges, weightOfRootEdges + N);
            S.resize(N);
            for (int i = 0; i < N; i++)
                parent[i] = i;
        }

        void init(int N, const vector<int>& weightOfRootEdges) {
            init(N, weightOfRootEdges.data());
        }


        int find(int x) {
            if (parent[x] == x)
                return x;
            return parent[x] = find(parent[x]);
        }

        int mergeGroup(int xset, int yset) {
            if (rootEdgeWeight[xset] < rootEdgeWeight[yset])
                swap(xset, yset);

            parent[xset] = yset;
            rootEdgeWeight[yset] = min(rootEdgeWeight[yset], rootEdgeWeight[xset]);

            // merge small to large

            int w, gv;
            if (S[xset].size() > S[yset].size())
                swap(S[xset], S[yset]);

            for (auto& it : S[xset]) {
                tie(w, gv) = it;
                if (!used[gv])
                    gv = find(gv);
                if (gv != xset && gv != yset)
                    S[yset].emplace(w, gv);
            }

            while (!S[yset].empty()) {
                tie(w, gv) = *S[yset].begin();
                if (!used[gv])
                    gv = find(gv);
                if (gv != xset && gv != yset)
                    break;
                S[yset].erase(S[yset].begin());
            }

            used[xset] = false;
            S[xset].clear();

            return yset;
        }

        int merge(int x, int y) {
            int xset = find(x);
            int yset = find(y);
            if (xset == yset)
                return xset;
            return mergeGroup(xset, yset);
        }
    };

    // 0 is the root node

    int N;
    vector<vector<pair<int, int>>> graph;   // graph[u] = { (v, edge_index), ... }
    vector<tuple<int, int, int>> edges;     // edges = { (u, v, w), ... }

    vector<int> rootEdgeWeight;             // rootEdgeWeight[i] = edge weight between root and i

    SpecialMST() : N(0) {
    }

    explicit SpecialMST(int N) {
        init(N);
    }

    void init(int N) {
        this->N = N;
        graph = vector<vector<pair<int, int>>>(N);
        rootEdgeWeight = vector<int>(N);
    }

    void addEdge(int u, int v, int w) {
        if (u > v)
            swap(u, v);

        int index = int(edges.size());

        graph[u].emplace_back(v, index);
        graph[v].emplace_back(u, index);
        edges.emplace_back(u, v, w);

        if (u == 0)
            rootEdgeWeight[v] = w;
    }

    // total cost of minimum spanning tree by degree of root node
    // - result[root_degree] = cost of MST
    // - root is 0
    // O((V+E)*logE)
    vector<long long> calcMstCostByDegreeOfRootNode() {
        vector<long long> res(N);

        int u, v, w;
        UnionFind dsu(N, rootEdgeWeight);

        long long weight = 0;
        for (int i = 0; i < int(edges.size()); i++) {
            if (get<0>(edges[i]) == 0)
                weight += get<2>(edges[i]);
            else {
                tie(u, v, w) = edges[i];
                dsu.S[u].emplace(w, v);
                dsu.S[v].emplace(w, u);
            }
        }
        res[N - 1] = weight;

        vector<int> vecW(N);        // [group] = weight
        set<pair<int, int>> setW;   // (weight, group)
        for (int u = 1; u < N; u++) {
            vecW[u] = dsu.S[u].begin()->first - dsu.rootEdgeWeight[u];
            setW.emplace(vecW[u], u);
        }

        int gu, gv;
        for (int j = N - 2; j > 0; j--) {
            tie(w, gv) = *setW.begin();
            gu = dsu.S[gv].begin()->second;

            gv = dsu.find(gv);
            gu = dsu.find(gu);

            if (j > 1) {
                setW.erase(make_pair(vecW[gu], gu));
                setW.erase(make_pair(vecW[gv], gv));

                int tg = dsu.mergeGroup(gu, gv);
                if (tg == gu) {
                    vecW[gu] = dsu.S[gu].begin()->first - dsu.rootEdgeWeight[gu];
                    setW.emplace(vecW[gu], gu);
                } else {
                    vecW[gv] = dsu.S[gv].begin()->first - dsu.rootEdgeWeight[gv];
                    setW.emplace(vecW[gv], gv);
                }
            }

            weight += w;
            res[j] = weight;
        }

        return res;
    }
};
