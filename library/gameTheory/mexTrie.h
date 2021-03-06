#pragma once

// http://codeforces.com/blog/entry/58316
// https://www.geeksforgeeks.org/combinatorial-game-theory-set-3-grundy-numbersnimbers-and-mex/
// https://www.geeksforgeeks.org/combinatorial-game-theory-set-2-game-nim/
// https://en.wikipedia.org/wiki/Mex_(mathematics)
// http://letuskode.blogspot.kr/2014/08/grundy-numbers.html

template <int MaxBitN = 31>
struct MexTrie {
    // index 0 = root
    vector<array<int, 3>> trie;     // (L, R, count)

    MexTrie() {
        trie.push_back({ 0, 0, 0 });
    }


    int size() const {
        return int(trie.size());
    }

    int count() const {
        return trie[0][2];
    }


    bool exists(int x) const {
        int u = 0;
        for (int i = MaxBitN - 1; i >= 0; i--) {
            int b = (x >> i) & 1;
            if (!trie[u][b])
                return false;
            u = trie[u][b];
        }
        return true;
    }

    bool insert(int x) {
        if (exists(x))
            return false;

        int u = 0;
        trie[u][2]++;       // increase count
        for (int i = MaxBitN - 1; i >= 0; i--) {
            int b = (x >> i) & 1;
            if (!trie[u][b]) {
                int v = int(trie.size());
                trie[u][b] = v;
                trie.push_back({ 0, 0, 0 });
            }
            u = trie[u][b];
            trie[u][2]++;   // increase count
        }

        return true;
    }

    // return mex{A[0]^xorAdd, A[1]^xorAdd, ..., A[n-1]^xorAdd}
    int mex(int xorAdd = 0) const {
        int res = 0;

        int u = 0;
        for (int i = MaxBitN - 1; i >= 0; i--) {
            int b = (xorAdd >> i) & 1;

            int v = trie[u][b];
            if (!v)
                return res;

            // sub-tree is full
            if (trie[v][2] == (1 << i)) {
                b ^= 1;
                res |= 1 << i;
            }
            if (!trie[u][b])
                return res;

            u = trie[u][b];
        }

        //assert(false);
        return 0;
    }

    //---

    void merge(const MexTrie<MaxBitN>& rhs, int xorAdd = 0) {
        vector<int> values;
        values.reserve(trie[0][2]);
        rhs.dfs(values, 0, 0, MaxBitN);

        for (auto x : values)
            insert(x ^ xorAdd);
    }

private:
    void dfs(vector<int>& values, int u, int val, int h) const {
        if (h == 0) {
            values.push_back(val);
            return;
        }
        if (trie[u][0])
            dfs(values, trie[u][0], val, h - 1);
        if (trie[u][1])
            dfs(values, trie[u][1], val | (1 << (h - 1)), h - 1);
    }
};
