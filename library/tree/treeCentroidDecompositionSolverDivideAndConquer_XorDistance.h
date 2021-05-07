#pragma once

// https://www.codechef.com/problems/MOVCOIN2

struct TreeCentroidDecompositionSolverDivideAndConquer_XorDistance {
    struct BitSetSimple64 {
        static int ctz(unsigned long long x) {
#if defined(_M_X64)
            return int(_tzcnt_u64(x));
#elif defined(__GNUC__)
            return __builtin_ctzll(x);
#else
            if ((x >> 32) != 0)
                return int(_tzcnt_u32(unsigned(x >> 32)));
            else
                return 32 + int(_tzcnt_u32(unsigned(x)));
#endif
        }

        int N;
        vector<unsigned long long> values;

        BitSetSimple64() {
        }

        explicit BitSetSimple64(int n) {
            init(n);
        }

        void init(int n) {
            N = n;
            values = vector<unsigned long long>((N + 63) >> 6);
        }

        BitSetSimple64& flip(int pos) {
            int idx = pos >> 6;
            int off = pos & 0x3f;
            values[idx] ^= 1ull << off;
            return *this;
        }
    };

    int N;
    vector<vector<int>> edges;
    vector<bool> values;                // 

    // find a center node in a tree
    vector<int> treeLevel;
    vector<int> treeSize;
    vector<bool> ctMarked;

    int currTime;
    vector<pair<int, int>> visitTime;
    vector<int> timeToNode;

    vector<int> answer;

    void init(int N) {
        this->N = N;
        edges = vector<vector<int>>(N);
        values = vector<bool>(N);

        treeLevel = vector<int>(N);
        treeSize = vector<int>(N);
        ctMarked = vector<bool>(N);

        //---
        currTime = 0;
        visitTime = vector<pair<int, int>>(N);
        timeToNode = vector<int>(N);

        answer = vector<int>(N);
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    void setValue(int u, bool enable) {
        values[u] = enable;
    }

    //---

    // O(N*(logN)^2)
    void solve() {
        dfsSize(0, -1);
        dfsSolve(0, -1);
    }

private:
    static int clz(int x) {
        if (!x)
            return 32;
#ifndef __GNUC__
        return int(_lzcnt_u32((unsigned)x));
#else
        return __builtin_clz((unsigned)x);
#endif
    }

    int dfsDist(int u, int parent, int depth, BitSetSimple64& all) {
        if (values[u])
            all.flip(depth);

        treeSize[u] = 1;
        treeLevel[u] = depth;

        timeToNode[currTime] = u;
        visitTime[u].first = currTime++;

        int res = depth;

        for (int v : edges[u]) {
            if (v != parent && !ctMarked[v]) {
                res = max(res, dfsDist(v, u, depth + 1, all));
                treeSize[u] += treeSize[v];
            }
        }

        visitTime[u].second = currTime;

        return res;
    }

    void apply(int u, int parent, int size, int baseDepth = 0) {
        BitSetSimple64 all(size + baseDepth);

        currTime = 0;
        int maxDepth = dfsDist(u, parent, baseDepth, all);

        int logH = 32 - clz(maxDepth * 2);  // max distance = maxDepth * 2

        int cnt = 0;
        int xxor = 0;
        vector<vector<int>> dist(logH);
        for (int i = 0; i < logH; i++)
            dist[i].resize(size_t(1) << i);

        for (int j = 0, k = 0; j <= maxDepth; j += 64, k++) {
            auto bits = all.values[k];
            while (bits) {
                int idx = j + BitSetSimple64::ctz(bits);
                cnt++;
                for (int k = 0, msb = 1; k < logH; k++, msb <<= 1) {
                    dist[k][idx & (msb - 1)] ^= msb;
                    xxor ^= idx & msb;
                }
                bits &= bits - 1;
            }
        }

        answer[u] ^= xxor;
        if (cnt > 0) {
            vector<int> X;
            X.push_back(xxor);
            for (int v : edges[u]) {
                if (ctMarked[v])
                    continue;
                for (int t = visitTime[v].first; t < visitTime[v].second; t++) {
                    int vt = timeToNode[t];
                    int d = treeLevel[vt] - baseDepth;
                    if (d >= X.size()) {
                        for (int i = 0, size = 1; i < logH; i++, size <<= 1)
                            xxor ^= dist[i][(size - d) & (size - 1)];
                        X.push_back(xxor);
                    }
                    answer[vt] ^= X[d];
                }
            }
        }
    }

    void dfsSolve(int u, int parent) {
        int size = treeSize[u];
        int root = findCentroid(u, parent, size);

        if (parent >= 0)
            apply(u, parent, size, 2);  // subtract

        u = root;
        apply(u, -1, size, 0);          // add

        ctMarked[u] = true;
        for (int v : edges[u]) {
            if (!ctMarked[v])
                dfsSolve(v, u);
        }
    }

    //--- centroid

    void dfsSize(int u, int parent) {
        treeSize[u] = 1;
        for (int v : edges[u]) {
            if (v != parent && !ctMarked[v]) {
                dfsSize(v, u);
                treeSize[u] += treeSize[v];
            }
        }
    }

    int findCentroid(int u, int parent, int size) {
        bool isMajor = true;
        for (int v : edges[u]) {
            if (v == parent || ctMarked[v])
                continue;

            int res = findCentroid(v, u, size);
            if (res != -1)
                return res;

            if (treeSize[v] + treeSize[v] > size)
                isMajor = false;
        }

        if (isMajor && 2 * (size - treeSize[u]) <= size)
            return u;

        return -1;
    }
};
