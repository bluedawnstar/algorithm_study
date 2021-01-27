#pragma once

template <int MaxBitN = 31>
struct PersistentMexTrie {
    // index 0 = null node
    vector<array<int, 3>> trie;     // (L, R, count)

    PersistentMexTrie() {
        trie.push_back({ 0, 0, 0 });
    }

    void init() {
        trie.clear();
        trie.push_back({ 0, 0, 0 });
    }


    int size() const {
        return int(trie.size());
    }

    int count(int root) const {
        return trie[root][2];
    }

    bool exists(int root, int x) const {
        if (root <= 0)
            return false;

        int u = root;
        for (int i = MaxBitN - 1; i >= 0; i--) {
            int b = (x >> i) & 1;
            if (!trie[u][b])
                return false;
            u = trie[u][b];
        }
        return true;
    }

    // return (new root, added?)
    pair<int, bool> insert(int root, int x) {
        if (exists(root, x))
            return make_pair(root, false);

        int u = int(trie.size());
        trie.push_back(trie[root]);

        root = u;
        trie[u][2]++;       // increase count
        for (int i = MaxBitN - 1; i >= 0; i--) {
            int b = (x >> i) & 1;

            int v = int(trie.size());
            if (!trie[u][b])
                trie.push_back({ 0, 0, 0 });
            else
                trie.push_back(trie[trie[u][b]]);
            trie[u][b] = v;

            u = v;
            trie[u][2]++;   // increase count
        }

        return make_pair(root, true);
    }

    // return mex{A[0]^xorAdd, A[1]^xorAdd, ..., A[n-1]^xorAdd}
    int mex(int root, int xorAdd = 0) const {
        int res = 0;
        if (root <= 0)
            return res;

        int u = root;
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

    // return new root
    int merge(int toRoot, int fromRoot, int xorAdd = 0) {
        vector<int> values;
        values.reserve(trie[0][2]);
        dfs(values, fromRoot, 0, MaxBitN);

        for (auto x : values)
            toRoot = insert(toRoot, x ^ xorAdd).first;

        return toRoot;
    }

    // return new root
    int mergeMove(int toRoot, int fromRoot, int xorAdd = 0) {
        return dfsMergeMove(toRoot, fromRoot, MaxBitN, xorAdd).first;
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

    //---

    // return the number of values
    int dfsFlip(int u, int h, int flip) {
        if (h == 0)
            return 1;

        int res = 0;
        if (trie[u][0])
            res += dfsFlip(trie[u][0], h - 1, flip);
        if (trie[u][1])
            res += dfsFlip(trie[u][1], h - 1, flip);

        if (flip & (1 << (h - 1)))
            swap(trie[u][0], trie[u][1]);

        return res;
    }

    // return (root, added count)
    pair<int, int> dfsMergeMove(int to, int from, int h, int flip) {
        if (h == 0)
            return make_pair(to, 0);

        int child[2] = { trie[to][0], trie[to][1] };
        if (flip & (1 << (h - 1)))
            swap(trie[from][0], trie[from][1]);

        int addedN = 0;

        for (int i = 0; i < 2; i++) {
            if (trie[to][i] == 0) {
                if (trie[from][i] != 0) {
                    addedN += dfsFlip(trie[from][i], h - 1, flip);
                    child[i] = trie[from][i];
                }
            } else {
                if (trie[from][i] != 0) {
                    auto r = dfsMergeMove(trie[to][i], trie[from][i], h - 1, flip);
                    child[i] = r.first;
                    addedN += r.second;
                }
            }
        }

        if (addedN > 0) {
            trie.push_back({ child[0], child[1], trie[to][2] + addedN });
            to = int(trie.size()) - 1;
        }
        return make_pair(to, addedN);
    }
};
