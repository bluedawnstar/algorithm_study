#pragma once

/*
    https://en.wikipedia.org/wiki/Chordal_graph
    http://www.secmem.org/blog/2019/03/10/Finding-perfect-elimination-ordering-in-choral-graphs/

  <Chordal Graph>
    - one in which all cycles of four or more vertices have a chord,
      which is an edge that is not part of the cycle but connects two vertices of the cycle
    - every induced cycle in the graph should have exactly three vertices

  1. Perfect Elimination Ordering
    - an ordering of the vertices of the graph such that, for each vertex v,
      v and the neighbors of v that occur after v in the order form a clique
*/

// undirected graph
struct ChordalGraph {
    int N;
    vector<vector<pair<int, int>>> edges;
    int edgeCount;

    ChordalGraph() : N(0), edgeCount(0) {
    }

    explicit ChordalGraph(int n) : N(n), edges(N), edgeCount(0) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<pair<int, int>>>(N);
        edgeCount = 0;
    }

    void addEdge(int u, int v) {
        edges[u].emplace_back(v, edgeCount);
        edges[v].emplace_back(u, edgeCount);
        edgeCount++;
    }

    //---

    // O(V + E)
    bool isPerfectEliminationOrdering(const vector<int>& order) const {
        if (int(order.size()) != N)
            return false;

        vector<int> rev(N);
        for (int i = 0; i < N; i++)
            rev[order[i]] = i;

        unordered_set<long long> edgeSet;
        for (int u = 0; u < N; u++) {
            for (auto& v : edges[u]) {
                if (u > v.first)
                    continue;
                edgeSet.insert(1ll * u * N + v.first);
            }
        }

        for (auto u : order) {
            int minNode = -1;
            for (auto& it : edges[u]) {
                int v = it.first;
                if ((rev[u] < rev[v]) && (minNode < 0 || rev[v] < rev[minNode]))
                    minNode = v;
            }
            if (minNode < 0)
                continue;

            for (auto& it : edges[u]) {
                int v = it.first;
                long long key = 1ll * min(minNode, v) * N + max(minNode, v);
                if ((rev[v] > rev[minNode]) && (edgeSet.find(key) == edgeSet.end()))
                    return false;
            }
        }

        return true;
    }


    // O(E*sqrt(E))
    vector<int> findPerfectEliminationOrdering() {
        vector<long long> C(N);
        // C[u] = the number of all pairs among neighbors - the number of 3-cycles
        {
            for (int u = 0; u < N; u++) {
                if (!edges[u].empty())
                    sort(edges[u].begin(), edges[u].end());
                int sz = int(edges[u].size());
                C[u] = 1ll * sz * (sz - 1) / 2;
            }

            vector<bool> neighbor(N);
            for (int u = 0; u < N; u++) {
                for (auto& x : edges[u])
                    neighbor[x.first] = true;

                for (auto& x : edges[u]) {
                    if ((edges[x.first].size() < edges[u].size()) ||
                        (edges[x.first].size() == edges[u].size() && x.first < u)) {
                        for (auto& y : edges[x.first]) {
                            if (neighbor[y.first])
                                C[y.first]--;
                        }
                    }
                }

                for (auto& x : edges[u])
                    neighbor[x.first] = false;
            }
        }

        queue<int> Q;
        vector<bool> vis(N);

        vector<bool> edgeErased(edgeCount);
        vector<int> erasedCnt(N);

        for (int u = 0; u < N; u++) {
            if (C[u] == 0) {
                Q.push(u);
                vis[u] = true;
            }
        }

        vector<int> res;

        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            res.push_back(u);

            int sz = int(edges[u].size()) - erasedCnt[u];
            for (auto& it : edges[u]) {
                if (edgeErased[it.second])
                    continue;
                edgeErased[it.second] = true;

                int v = it.first;
                C[v] += (sz - 1) - (int(edges[v].size()) - ++erasedCnt[v]);
                if (C[v] == 0 && !vis[v]) {
                    vis[v] = true;
                    Q.push(v);
                }
            }
        }

        if (int(res.size()) != N)
            res.clear();

        return res;
    }

    // perfect elimination ordering with Lex-BFS, O(V + E)
    vector<int> findPerfectEliminationOrderingWithLexBFS() {
        auto order = findLexicographicBFS();
        reverse(order.begin(), order.end());
        if (!isPerfectEliminationOrdering(order))
            return vector<int>{};
        return order;
    }

    //---

    // Lexicographic Breadth First Search (Lex-BFS), O(V + E)
    // https://en.wikipedia.org/wiki/Lexicographic_breadth-first_search
    vector<int> findLexicographicBFS() {
        // initialize a sequence ¥Ò of sets, to contain a single set containing all vertices.
        struct ListT {
            list<int> set;
            int time;
        };

        list<ListT> sigma;
        {
            sigma.resize(1);
            for (int i = 0; i < N; i++)
                sigma.back().set.push_back(i);
            sigma.back().time = 0;
        }

        vector<bool> visit(N);
        vector<list<ListT>::iterator> setInSigma(N, sigma.begin());
        vector<list<int>::iterator> nodeInSet;
        nodeInSet.reserve(N);
        for (auto it = sigma.front().set.begin(); it != sigma.front().set.end(); ++it)
            nodeInSet.push_back(it);

        int time = 0;
        vector<int> res;

        while (!sigma.empty()) {
            if (sigma.front().set.empty()) {
                sigma.pop_front();
                continue;
            }

            // find and remove a vertex u from the first set in ¥Ò
            int u = sigma.front().set.front();
            sigma.front().set.pop_front();

            // if the first set in ¥Ò is now empty, remove it from ¥Ò
            if (sigma.front().set.empty())
                sigma.pop_front();

            // add u to the end of the output sequence.
            res.push_back(u);
            visit[u] = true;
            ++time;

            // for each edge u-v such that v still belongs to a set S in ¥Ò :
            for (auto& it : edges[u]) {
                int v = it.first;
                if (visit[v])
                    continue;

                // If the set S containing v has not yet been replaced while processing u,
                //   create a new empty replacement set T and place it prior to S in the sequence;
                // otherwise, let T be the set prior to S.

                auto iter = setInSigma[v];

                setInSigma[v]->set.erase(nodeInSet[v]);
                if (setInSigma[v]->time < time) {
                    setInSigma[v]->time = time;
                    setInSigma[v] = sigma.insert(setInSigma[v], ListT{ {}, 0 });
                } else {
                    --setInSigma[v];
                }
                nodeInSet[v] = setInSigma[v]->set.insert(setInSigma[v]->set.end(), v);

                if (iter->set.empty())
                    sigma.erase(iter);
            }
        }

        return res;
    }
};
