#pragma once

/*
  1. how to use
    1) prefixPattern
       string prefixPattern;
       //...
       PrefixFunctionTree tree;
       tree.build(prefixPattern);

    2) prefixPattern + string
       string prefixPattern;
       string S;
       //...
       PrefixFunctionTree tree;
       tree.build(prefixPattern, S);
*/
struct PrefixFunctionTree {
    int N;
    int logN;
    vector<vector<int>> children;       // children[0] is heavy children
    vector<vector<int>> P;              // 0 is the root, 1 <= index <= N
    vector<int> level;
    vector<int> subtreeSize;

    vector<pair<int, int>> visTime;
    vector<int> time2Node;
    int currTime;

    // O(N*logN)
    void build(const string& prefixPattern) {
        vector<int> pref = prefixFunction(prefixPattern);

        N = int(prefixPattern.length()) + 1;
#ifndef __GNUC__
        logN = _lzcnt_u32(1u) - _lzcnt_u32((unsigned int)(N - 1)) + 2;
#else
        logN = __builtin_clz(1u) - __builtin_clz((unsigned int)(N - 1)) + 2;
#endif
        children = vector<vector<int>>(N);
        P = vector<vector<int>>(logN, vector<int>(N));
        level = vector<int>(N);
        subtreeSize = vector<int>(N);

        visTime = vector<pair<int, int>>(N);
        time2Node = vector<int>(N);

        // 0 is the root, 1 <= index <= prefixPattern.length()
        P[0][0] = 0;
        for (int i = 1; i <= int(prefixPattern.length()); i++) {
            P[0][i] = pref[i - 1];
            children[P[0][i]].push_back(i);
        }

        currTime = 0;
        dfs(0);
        makeLcaTable();
    }

    // O((N + M)*log(N + M))
    void build(const string& prefixPattern, const string& s) {
        vector<int> pref = prefixFunction(prefixPattern);

        N = int(prefixPattern.length()) + int(s.length()) + 1;
#ifndef __GNUC__
        logN = _lzcnt_u32(1u) - _lzcnt_u32((unsigned int)(N - 1)) + 2;
#else
        logN = __builtin_clz(1u) - __builtin_clz((unsigned int)(N - 1)) + 2;
#endif
        children = vector<vector<int>>(N);
        P = vector<vector<int>>(logN, vector<int>(N));
        level = vector<int>(N);
        subtreeSize = vector<int>(N);

        visTime = vector<pair<int, int>>(N);
        time2Node = vector<int>(N);

        // 0 is the root, 1 <= index <= prefixPattern.length()
        P[0][0] = 0;
        for (int i = 1; i <= int(prefixPattern.length()); i++) {
            P[0][i] = pref[i - 1];
            children[P[0][i]].push_back(i);
        }

        // " " + prefixPattern + s
        int m = int(prefixPattern.length());
        int base = m + 1;
        for (int i = 0, j = 0; i < int(s.length()); i++) {
            while (j > 0 && s[i] != prefixPattern[j])
                j = pref[j - 1];

            if (s[i] == prefixPattern[j]) {
                P[0][base + i] = j + 1;
                if (j == m - 1) {
                    //P is found at i - m + 1
                    j = pref[j];
                } else {
                    j++;
                }
            } else {
                P[0][base + i] = j;
            }
            children[P[0][base + i]].push_back(base + i);
        }

        currTime = 0;
        dfs(0);
        makeLcaTable();
    }

    //--- lca

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

    // find LCA when the root is changed
    int findLCA(int root, int A, int B) const {
        int lca = findLCA(A, B);

        int temp = findLCA(A, root);
        if (level[temp] > level[lca])
            lca = temp;

        temp = findLCA(B, root);
        if (level[temp] > level[lca])
            lca = temp;

        return lca;
    }

    int distance(int u, int v) const {
        return level[u] + level[v] - level[findLCA(u, v)] * 2;
    }

private:
    vector<int> prefixFunction(const string& s) {
        vector<int> p(s.size());
        int j = 0;
        for (int i = 1; i < int(s.size()); i++) {
            while (j > 0 && s[j] != s[i])
                j = p[j - 1];

            if (s[j] == s[i])
                j++;
            p[i] = j;
        }
        return p;
    }

    void dfs(int u) {
        visTime[u].first = currTime;
        time2Node[currTime++] = u;

        subtreeSize[u] = 1;

        int maxSize = 0;
        int maxChildIdx = -1;
        for (int i = 0; i < int(children[u].size()); i++) {
            int v = children[u][i];

            level[v] = level[u] + 1;
            dfs(v);
            subtreeSize[u] += subtreeSize[v];

            if (maxSize < subtreeSize[v]) {
                maxSize = subtreeSize[v];
                maxChildIdx = i;
            }
        }

        if (maxChildIdx > 0)
            swap(children[u][0], children[u][maxChildIdx]);

        visTime[u].second = currTime;
    }

    void makeLcaTable() {
        for (int i = 1; i < logN; i++) {
            for (int j = 0; j < N; j++)
                P[i][j] = P[i - 1][P[i - 1][j]];
        }
    }
};
