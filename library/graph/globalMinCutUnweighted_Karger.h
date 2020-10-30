#pragma once

/*
  <Karger's algorithm>
  https://www.geeksforgeeks.org/kargers-algorithm-for-minimum-cut-set-1-introduction-and-implementation/

  - for undirected, unweighted and connected graph
  - probability that the cut produced by Karger¡¯s Algorithm is Min-Cut >= 1/n^2
*/
struct GlobalMinCutKarger {
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

    GlobalMinCutKarger() : N(0) {
    }

    explicit GlobalMinCutKarger(int n) : N(n) {
    }

    void init(int n) {
        N = n;
        edges.clear();
    }

    // add edges to undirected graph
    void addEdge(int u, int v) {
        edges.emplace_back(u, v);
    }

    // O(E) = O(V^2)
    // find global minimum cut probably, (N >= 2)
    int findMinCut(vector<int>& cutEdges) {
        DSU dsu(N);

        // initially there are N vertices in contracted graph 
        int vertices = N;

        // keep contracting vertices until there are 2 vertices. 
        while (vertices > 2) {
            // pick a random edge 
            int i = random(int(edges.size()));

            int g1 = dsu.find(edges[i].first);
            int g2 = dsu.find(edges[i].second);

            if (g1 != g2) {
                vertices--;
                dsu.merge(g1, g2);
            }
        }

        // now we have two vertices (or groups) left in the contracted graph,
        // so count the edges between two components and return the count. 
        cutEdges.clear();
        for (int i = 0; i < int(edges.size()); i++) {
            int g1 = dsu.find(edges[i].first);
            int g2 = dsu.find(edges[i].second);
            if (g1 != g2)
                cutEdges.push_back(i);
        }

        return int(cutEdges.size());
    }

private:
    static int random(int n) {
        //static std::random_device rd;
        //static std::mt19937 eng(rd());
        static std::mt19937 eng(1);
        static std::uniform_int_distribution<int> dist(0, std::numeric_limits<int>::max());
        return dist(eng) % n;
    }
};
