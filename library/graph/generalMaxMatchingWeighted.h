#pragma once

/*
  Maximum Weighted Matching in General Undirected Graphs
  - Blossom Algorithm, O(N^3)

  Note: each vertex is 1-indexed internally.

  from https://github.com/koosaga/DeobureoMinkyuParty/blob/master/teamnote.pdf
*/
template <typename T, T INF = 0x3f3f3f3f>
struct WeightedGeneralMaxMatching {
    struct Edge {
        int u, v;
        T w;

        Edge() {
        }

        Edge(int u, int v, T w) : u(u), v(v), w(w) {
        }
    };

    int N, currN;
    vector<vector<Edge>> graph;     // (u, v, w)
    vector<T> label;
    vector<int> match, slack, group, link;
    vector<vector<int>> flowerFrom;
    vector<vector<int>> flower;
    vector<int> state;              // in {-1, 0, 1}
    vector<int> visitTime;
    int currTime;
    queue<int> Q;

    WeightedGeneralMaxMatching() : N(0), currN(0) {
    }

    explicit WeightedGeneralMaxMatching(int n) {
        init(n);
    }

    void init(int n) {
        N = n;
        currN = n;

        graph = vector<vector<Edge>>((n + 1) * 2, vector<Edge>((n + 1) * 2));
        label = vector<T>((n + 1) * 2);

        match = vector<int>((n + 1) * 2);
        slack = vector<int>((n + 1) * 2);
        group = vector<int>((n + 1) * 2);
        link = vector<int>((n + 1) * 2);

        flowerFrom = vector<vector<int>>((n + 1) * 2, vector<int>((n + 1) * 2));
        flower = vector<vector<int>>((n + 1) * 2);
        state = vector<int>((n + 1) * 2);
        visitTime = vector<int>((n + 1) * 2);
        currTime = 0;

        for (int u = 1; u <= n; ++u)
            for (int v = 1; v <= n; ++v)
                graph[u][v] = Edge(u, v, 0);
    }

    const Edge& getEdge(int u, int v) const {
        u++; v++;
        return graph[u][v];
    }

    // 0 <= u, v < N
    void addEdge(int u, int v, T w) {
        u++; v++;
        graph[u][v].w = graph[v][u].w = w;
    }

    //---

    // O(N^3)
    // return (weight-sum, match pairs)
    // - match-pairs[i] = { (u, v), ... }   ,  0 <= u, v < N
    pair<long long, vector<pair<int, int>>> calcMaxMatching(int maxMatchCount = numeric_limits<int>::max()) {
        memset(match.data() + 1, 0, sizeof(int) * N);

        currN = N;
        int matchCount = 0;
        long long weightSum = 0;
        for (int u = 0; u <= N; ++u) {
            group[u] = u;
            flower[u].clear();
        }

        T maxW = 0;
        for (int u = 1; u <= N; ++u) {
            for (int v = 1; v <= N; ++v) {
                flowerFrom[u][v] = (u == v ? u : 0);
                maxW = max(maxW, graph[u][v].w);
            }
        }
        for (int u = 1; u <= N; ++u)
            label[u] = maxW;

        while (matchCount < maxMatchCount && matching())
            ++matchCount;

        vector<pair<int, int>> matchedPairs;
        for (int u = 1; u <= N; ++u) {
            if (match[u] && match[u] < u) {
                weightSum += graph[u][match[u]].w;
                matchedPairs.emplace_back(match[u] - 1, u - 1);
            }
        }

        return make_pair(weightSum, matchedPairs);
    }

private:
    T edgeDelta(const Edge& e) {
        return label[e.u] + label[e.v] - graph[e.u][e.v].w * 2;
    }

    void updateSlack(int u, int x) {
        if (!slack[x] || edgeDelta(graph[u][x]) < edgeDelta(graph[slack[x]][x]))
            slack[x] = u;
    }

    void setSlack(int x) {
        slack[x] = 0;
        for (int u = 1; u <= N; ++u) {
            if (graph[u][x].w > 0 && group[u] != x && state[group[u]] == 0)
                updateSlack(u, x);
        }
    }

    void pushQ(int x) {
        if (x <= N) {
            Q.push(x);
        } else {
            for (auto f : flower[x])
                pushQ(f);
        }
    }

    void setGroup(int x, int b) {
        group[x] = b;
        if (x > N) {
            for (auto f : flower[x])
                setGroup(f, b);
        }
    }

    int getPosition(int b, int xr) {
        int res = int(find(flower[b].begin(), flower[b].end(), xr) - flower[b].begin());
        if (res & 1) {
            reverse(flower[b].begin() + 1, flower[b].end());
            return int(flower[b].size()) - res;
        } else {
            return res;
        }
    }

    void setMatch(int u, int v) {
        match[u] = graph[u][v].v;
        if (u <= N)
            return;
        Edge e = graph[u][v];
        int xr = flowerFrom[u][e.u];
        int pr = getPosition(u, xr);
        for (int i = 0; i < pr; ++i)
            setMatch(flower[u][i], flower[u][i ^ 1]);
        setMatch(xr, v);
        rotate(flower[u].begin(), flower[u].begin() + pr, flower[u].end());
    }

    void augment(int u, int v) {
        while (true) {
            int xnv = group[match[u]];
            setMatch(u, v);
            if (!xnv)
                return;
            setMatch(xnv, group[link[xnv]]);
            u = group[link[xnv]];
            v = xnv;
        }
    }

    int getLca(int u, int v) {
        for (++currTime; u || v; swap(u, v)) {
            if (u == 0)
                continue;
            if (visitTime[u] == currTime)
                return u;
            visitTime[u] = currTime;
            u = group[match[u]];
            if (u)
                u = group[link[u]];
        }
        return 0;
    }

    void addBlossom(int u, int lca, int v) {
        int b = N + 1;
        while (b <= currN && group[b])
            ++b;
        if (b > currN)
            ++currN;
        label[b] = 0;
        state[b] = 0;
        match[b] = match[lca];
        flower[b].clear();
        flower[b].push_back(lca);
        for (int x = u, y; x != lca; x = group[link[y]]) {
            flower[b].push_back(x);
            y = group[match[x]];
            flower[b].push_back(y);
            pushQ(y);
        }
        reverse(flower[b].begin() + 1, flower[b].end());
        for (int x = v, y; x != lca; x = group[link[y]]) {
            flower[b].push_back(x);
            y = group[match[x]];
            flower[b].push_back(y);
            pushQ(y);
        }
        setGroup(b, b);
        for (int x = 1; x <= currN; ++x)
            graph[b][x].w = graph[x][b].w = 0;
        for (int x = 1; x <= N; ++x)
            flowerFrom[b][x] = 0;
        for (size_t i = 0; i < flower[b].size(); ++i) {
            int xs = flower[b][i];
            for (int x = 1; x <= currN; ++x) {
                if (graph[b][x].w == 0 || edgeDelta(graph[xs][x]) < edgeDelta(graph[b][x])) {
                    graph[b][x] = graph[xs][x];
                    graph[x][b] = graph[x][xs];
                }
            }
            for (int x = 1; x <= N; ++x) {
                if (flowerFrom[xs][x])
                    flowerFrom[b][x] = xs;
            }
        }
        setSlack(b);
    }

    void expandBlossom(int b) {
        for (size_t i = 0; i < flower[b].size(); ++i)
            setGroup(flower[b][i], flower[b][i]);

        int xr = flowerFrom[b][graph[b][link[b]].u];
        int pr = getPosition(b, xr);
        for (int i = 0; i < pr; i += 2) {
            int xs = flower[b][i];
            int xns = flower[b][i + 1];
            link[xs] = graph[xns][xs].u;
            state[xs] = 1;
            state[xns] = 0;
            slack[xs] = 0;
            setSlack(xns);
            pushQ(xns);
        }
        state[xr] = 1;
        link[xr] = link[b];
        for (size_t i = pr + 1; i < flower[b].size(); ++i) {
            int xs = flower[b][i];
            state[xs] = -1;
            setSlack(xs);
        }
        group[b] = 0;
    }

    bool onFoundEdge(const Edge& e) {
        int u = group[e.u];
        int v = group[e.v];
        if (state[v] == -1) {
            link[v] = e.u;
            state[v] = 1;
            int nu = group[match[v]];
            slack[v] = slack[nu] = 0;
            state[nu] = 0;
            pushQ(nu);
        } else if (state[v] == 0) {
            int lca = getLca(u, v);
            if (!lca) {
                augment(u, v);
                augment(v, u);
                return true;
            } else {
                addBlossom(u, lca, v);
            }
        }
        return false;
    }

    bool matching() {
        memset(state.data() + 1, -1, sizeof(int) * currN);
        memset(slack.data() + 1, 0, sizeof(int) * currN);
        Q = queue<int>();

        for (int x = 1; x <= currN; ++x) {
            if (group[x] == x && !match[x]) {
                link[x] = 0;
                state[x] = 0;
                pushQ(x);
            }
        }

        if (Q.empty())
            return false;

        while (true) {
            while (Q.size()) {
                int u = Q.front();
                Q.pop();
                if (state[group[u]] == 1)
                    continue;
                for (int v = 1; v <= N; ++v) {
                    if (graph[u][v].w > 0 && group[u] != group[v]) {
                        if (edgeDelta(graph[u][v]) == 0) {
                            if (onFoundEdge(graph[u][v]))
                                return true;
                        } else {
                            updateSlack(u, group[v]);
                        }                    }                }            }
            T d = INF;
            for (int b = N + 1; b <= currN; ++b) {
                if (group[b] == b && state[b] == 1)
                    d = min(d, label[b] / 2);
            }
            for (int x = 1; x <= currN; ++x) {
                if (group[x] == x && slack[x]) {
                    if (state[x] == -1)
                        d = min(d, edgeDelta(graph[slack[x]][x]));
                    else if (state[x] == 0)
                        d = min(d, edgeDelta(graph[slack[x]][x]) / 2);
                }
            }
            for (int u = 1; u <= N; ++u) {
                if (state[group[u]] == 0) {
                    if (label[u] <= d)
                        return 0;
                    label[u] -= d;
                } else if (state[group[u]] == 1) {
                    label[u] += d;
                }
            }
            for (int b = N + 1; b <= currN; ++b) {
                if (group[b] == b) {
                    if (state[group[b]] == 0)
                        label[b] += d * 2;
                    else if (state[group[b]] == 1)
                        label[b] -= d * 2;
                }
            }
            Q = queue<int>();
            for (int x = 1; x <= currN; ++x) {
                if (group[x] == x && slack[x] && group[slack[x]] != x && edgeDelta(graph[slack[x]][x]) == 0) {
                    if (onFoundEdge(graph[slack[x]][x]))
                        return true;
                }
            }
            for (int b = N + 1; b <= currN; ++b) {
                if (group[b] == b && state[b] == 1 && label[b] == 0)
                    expandBlossom(b);
            }
        }
        return false;
    }
};
