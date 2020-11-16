#pragma once

// https://www.codechef.com/NOV20A/problems/UNSQUERS

// offline algorithm
// 
// query(u, v) = dot(vertex_weight_vector_of_path_from_u_to_root, vertex_weight_vector_of_path_from_v_to_root)
//
struct TreePathVertexWeightVectorDot {
    int N;
    int logN;
    vector<vector<int>> edges;
    vector<int> W;

    vector<vector<int>> P;
    vector<int> level;

    vector<pair<int, int>> visTime; // node index to (visit time, leave time)
    vector<int> time2Node;          // time to node index (0 <= time < 2 * N)
    int currTime;                   //

    TreePathVertexWeightVectorDot() : N(0), logN(0) {
    }

    TreePathVertexWeightVectorDot(const int w[], int n) {
        init(w, n);
    }

    TreePathVertexWeightVectorDot(const vector<int>& w) {
        init(w);
    }

    void init(const int w[], int n) {
        N = n;
        edges = vector<vector<int>>(n);
        W.assign(w, w + n);

        logN = 1;
        while ((1 << logN) <= N)
            logN++;

        P = vector<vector<int>>(logN, vector<int>(n));
        level = vector<int>(n);

        visTime = vector<pair<int, int>>(n);
        time2Node = vector<int>(n * 2);
        currTime = 0;
    }

    void init(const vector<int>& w) {
        init(w.data(), int(w.size()));
    }

    void add(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    void build() {
        dfs(0, -1);
        makeLcaTable();
    }

    //---

    // PRECONDITION : level[qry[i].first] == level[qry[i].second]
    // O(Q*(logN + logQ) + Q*sqrt(N))
    vector<long long> solve(const vector<pair<int, int>>& qry) {
        build(qry);

        vector<long long> res(qry.size());

        int currL = 0;
        int currR = 0;
        for (int i = 0; i < int(qry.size()); i++) {
            int nextL = get<0>(MO[i]);
            int nextR = get<1>(MO[i]);
            int qi = get<2>(MO[i]);

            while (currL < nextL)
                add(false, currL++);
            while (currL > nextL)
                remove(false, --currL);

            while (currR < nextR)
                add(true, currR++);
            while (currR > nextR)
                remove(true, --currR);

            long long x = (stkValid > 0) ? stkDP[stkValid - 1] : 0;
            for (; stkValid < stkTop[0]; stkValid++) {
                x += 1ll * stk[0][stkValid] * stk[1][stkValid];
                stkDP[stkValid] = x;
            }

            res[qi] = x;
        }

        return res;
    }

private:
    void dfs(int u, int parent) {
        visTime[u].first = currTime;
        time2Node[currTime++] = u;

        P[0][u] = parent;

        for (int v : edges[u]) {
            if (v == parent)
                continue;

            level[v] = level[u] + 1;
            dfs(v, u);
        }

        visTime[u].second = currTime;
        time2Node[currTime++] = u;
    }

    void makeLcaTable() {
        for (int i = 1; i < logN; i++) {
            for (int j = 0; j < N; j++) {
                int pp = P[i - 1][j];
                P[i][j] = pp < 0 ? pp : P[i - 1][pp];
            }
        }
    }

    int climbTree(int node, int dist) const {
        if (dist <= 0)
            return node;

        for (int i = 0; dist > 0; i++) {
            if (dist & 1)
                node = P[i][node];
            dist >>= 1;
        }

        return node;
    }

    int findLCA(int A, int B) const {
        if (level[A] < level[B])
            swap(A, B);

        A = climbTree(A, level[A] - level[B]);

        if (A == B)
            return A;

        int bitCnt = 0;
        for (int x = level[A]; x; x >>= 1)
            bitCnt++;

        for (int i = bitCnt - 1; i >= 0; i--) {
            if (P[i][A] > 0 && P[i][A] != P[i][B]) {
                A = P[i][A];
                B = P[i][B];
            }
        }

        return P[0][A];
    }

    //--- tree MO's algorithm

    vector<bool> active[2];             // left or right
    vector<tuple<int, int, int>> MO;

    int stkTop[2];
    int stkValid;
    vector<int> stk[2];
    vector<long long> stkDP;

    // O(Q*(logN + logQ))
    void build(const vector<pair<int, int>>& qry) {
        active[0] = vector<bool>(N);
        active[1] = vector<bool>(N);
        MO.reserve(qry.size());

        for (int i = 0; i < int(qry.size()); i++) {
            int u = qry[i].first;
            int v = qry[i].second;

            int lca = findLCA(u, v);
            if (lca == u)
                MO.emplace_back(visTime[u].first + 1, visTime[v].first + 1, i);
            else
                MO.emplace_back(visTime[u].second, visTime[v].first + 1, i);
        }

        const int blockN = int(sqrt(2 * N));
        sort(MO.begin(), MO.end(), [blockN](const auto& l, const auto& r) {
#if 0
            if (get<0>(l) / blockN != get<0>(r) / blockN)
                return get<0>(l) / blockN < get<0>(r) / blockN;
            return get<1>(l) < get<1>(r);
#else
            if (get<0>(l) / blockN != get<0>(r) / blockN)
                return l < r;
            return ((get<0>(l) / blockN) & 1) ? (get<1>(l) < get<1>(r)) : (get<1>(l) > get<1>(r));
#endif
        });

        stkTop[0] = 0;
        stkTop[1] = 0;
        stkValid = 0;
        stk[0].resize(N);
        stk[1].resize(N);
        stkDP.resize(N);
    }

    void onAdd(bool right, int t, int u) {
        stk[right][stkTop[right]++] = W[u];
    }

    void onRemove(bool right, int t, int u) {
        stkValid = min(stkValid, --stkTop[right]);
    }

    void add(bool right, int t) {
        int u = time2Node[t];
        if (active[right][u]) {
            remove(right, t);
        } else {
            active[right][u] = true;
            onAdd(right, t, u);
        }
    }

    void remove(bool right, int t) {
        int u = time2Node[t];
        if (!active[right][u]) {
            add(right, t);
        } else {
            active[right][u] = false;
            onRemove(right, t, u);
        }
    }
};
