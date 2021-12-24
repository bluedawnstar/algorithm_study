#pragma once

template <int MaxCharN = 26, int BaseChar = 'a'>
struct PersistentTrie {
    static constexpr int ch2i(char ch) {
        return ch - BaseChar;
    }

    struct Node {
        int leafCount;
        int parent;
        int children[MaxCharN];

        explicit Node(int parent = -1) {
            init(parent);
        }

        void init(int parent = -1) {
            this->leafCount = 0;
            this->parent = parent;
            memset(this->children, -1, sizeof(this->children));
        }

        bool isLeaf() const {
            return leafCount > 0;
        }

        bool isEmpty() const {
            if (leafCount > 0)
                return false;
            for (int i = 0; i < MaxCharN; i++) {
                if (children[i] >= 0)
                    return false;
            }
            return true;
        }
    };

    int initRoot;
    vector<Node> nodes;

    PersistentTrie() {
        initRoot = 0;
        nodes.emplace_back(-1);
    }

    int init() {
        nodes.clear();
        nodes.emplace_back(-1);
        return initRoot = 0;
    }


    // return (new root, true if it's a new string).
    pair<int,bool> insert(int root, const string& s) {
        return insert(root, s.c_str(), int(s.length()));
    }

    // return (new root, true if it's a new string).
    pair<int,bool> insert(int root, const char* s, int len) {
        if (len <= 0)
            return make_pair(root, false);

        root = cloneNode(root);
        int cur = root;
        for (int i = 0; i < len; i++) {
            int idx = ch2i(s[i]);
            int next = cloneNode(nodes[cur].children[idx], cur);
            nodes[cur].children[idx] = next;
            cur = next;
        }
        return make_pair(root, ++nodes[cur].leafCount == 1);
    }


    // return exactly matched word
    int find(int root, const string& s) const {
        return find(root, s.c_str(), int(s.length()));
    }

    // return exactly matched word
    int find(int root, const char* s, int len) const {
        if (len <= 0)
            return -1;

        int cur = root;
        for (int i = 0; i < len && cur >= 0; i++) {
            int idx = ch2i(s[i]);
            cur = nodes[cur].children[idx];
        }
        return (cur >= 0 && nodes[cur].leafCount > 0) ? cur : -1;
    }


    // prefix matching
    // return (prefix_matching_length, word_matched?)
    pair<int, bool> search(int root, const string& s) const {
        return search(root, s.c_str(), int(s.length()));
    }

    // prefix matching
    // return (prefix_matching_length, word_matched?)
    pair<int, bool> search(int root, const char* s, int len) const {
        if (len <= 0)
            return make_pair(0, false);

        int cur = root;
        for (int i = 0; i < len; i++) {
            int idx = ch2i(s[i]);
            cur = nodes[cur].children[idx];
            if (cur < 0)
                return make_pair(i, false);
        }
        return make_pair(len, nodes[cur].isLeaf());
    }

private:
    int allocNode(int parent = -1) {
        int res = int(nodes.size());
        nodes.emplace_back(parent);
        return res;
    }

    int cloneNode(int src) {
        int res = int(nodes.size());
        nodes.resize(nodes.size() + 1);
        if (src >= 0)
            nodes.back() = nodes[src];
        return res;
    }

    int cloneNode(int src, int parent) {
        int res = int(nodes.size());
        nodes.resize(nodes.size() + 1);
        if (src >= 0) {
            nodes.back() = nodes[src];
            nodes.back().parent = parent;
        }
        return res;
    }
};
