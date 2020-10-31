#pragma once

/*
  <Karger-Stein algorithm (1996)>
  https://en.wikipedia.org/wiki/Karger%27s_algorithm#Karger%E2%80%93Stein_algorithm
  http://www.secmem.org/blog/2019/10/20/Kargers-Algorithm/

  - for undirected, unweighted and connected graph
  - randomized algorithm

  - probability that the cut produced by Karger-Stein Algorithm is Min-Cut >= 1/logn
  - one operation : O(V^2*logV)
  - high probability : O((logV)^2) times -> O(V^2*(logV)^3)
*/
struct GlobalMinCutKargerStein {
    struct DSU {
        vector<int> parent;
        vector<int> rank;

        DSU() {
        }

        explicit DSU(int N) {
            init(N);
        }

        void init(int N) {
            parent.resize(N);
            rank.assign(N, 0);
            for (int i = 0; i < N; i++)
                parent[i] = i;
        }

        int find(int x) {
            if (parent[x] == x)
                return x;
            return parent[x] = find(parent[x]);
        }

        int merge(int x, int y) {
            int xset = find(x);
            int yset = find(y);
            if (xset == yset)
                return xset;

            if (rank[xset] < rank[yset]) {
                parent[xset] = yset;
                return yset;
            } else {
                parent[yset] = xset;
                if (rank[xset] == rank[yset])
                    rank[xset]++;
                return xset;
            }
        }
    };


    int N;                              // the number of vertices
    vector<pair<int, int>> edges;       // edges

    GlobalMinCutKargerStein() : N(0) {
    }

    explicit GlobalMinCutKargerStein(int n) {
        init(n);
    }

    void init(int n) {
        N = n;
        edges.clear();
    }

    // add edges to undirected graph
    void addEdge(int u, int v) {
        edges.emplace_back(u, v);
    }

    // O(V^2*logV)
    // find global minimum cut probably, (N >= 2)
    int findMinCut(vector<int>& cutEdges) {
        cutEdges.clear();

        vector<pair<pair<int,int>,int>> edgeSubset;
        edgeSubset.reserve(edges.size());
        for (int i = 0; i < int(edges.size()); i++)
            edgeSubset.emplace_back(edges[i], i);

        return findMinCut(N, edgeSubset, cutEdges);
    }

private:
    int findMinCut(int NN, vector<pair<pair<int, int>, int>>& edgeSubset, vector<int>& cutEdges) {
        if (NN < 6) {
            int res = int(edgeSubset.size());
            vector<int> selected;

            for (int i = 1, limit = (1 << NN) - 1; i < limit; i++) {
                int connect = 0;
                vector<int> sel;
                for (auto& it : edgeSubset) {
                    int u = it.first.first;
                    int v = it.first.second;
                    if ((i & (1 << u)) ^ (i & (1 << v))) {
                        connect++;
                        sel.push_back(it.second);
                    }
                }
                if (connect < res) {
                    swap(selected, sel);
                    res = connect;
                }
            }
            cutEdges.insert(cutEdges.end(), selected.begin(), selected.end());
            return res;
        }

        vector<pair<pair<int, int>, int>> newEdges[2];
        int t = 1 + int(NN / sqrt(2));
        for (int rep = 0; rep < 2; rep++) {
            DSU dsu(NN);

            int cnt = NN;
            random_shuffle(edgeSubset.begin(), edgeSubset.end(), GlobalMinCutKargerStein::random);
            //random_shuffle(edgeSubset.begin(), edgeSubset.end());

            for (int i = 0; i < int(edgeSubset.size()); i++) {
                int u = edgeSubset[i].first.first;
                int v = edgeSubset[i].first.second;
                if (cnt <= t) {
                    vector<int> reindex(NN);
                    int counter = 0;
                    for (int j = 0; j < NN; j++) {
                        if (dsu.find(j) == j)
                            reindex[j] = counter++;
                    }

                    for (int j = i; j < int(edgeSubset.size()); j++) {
                        int uu = dsu.find(edgeSubset[j].first.first);
                        int vv = dsu.find(edgeSubset[j].first.second);
                        if (uu != vv)
                            newEdges[rep].emplace_back(pair<int,int>{ reindex[uu], reindex[vv] }, edgeSubset[j].second);
                    }
                    break;
                }
                u = dsu.find(u);
                v = dsu.find(v);
                if (u != v) {
                    cnt--;
                    dsu.merge(u, v);
                }
            }
        }

        vector<int> cut0;
        int res0 = findMinCut(t, newEdges[0], cut0);

        vector<int> cut1;
        int res1 = findMinCut(t, newEdges[1], cut1);

        if (res0 > res1) {
            swap(res0, res1);
            swap(cut0, cut1);
        }

        cutEdges.insert(cutEdges.end(), cut0.begin(), cut0.end());

        return res0;
    }
    
    static int random(vector<pair<pair<int, int>, int>>::difference_type n) {
        //static std::random_device rd;
        //static std::mt19937 eng(rd());
        static std::mt19937 eng(1);
        static std::uniform_int_distribution<int> dist(0, std::numeric_limits<int>::max());
        return dist(eng) % n;
    }
};
