#pragma once
#pragma once

template <int BaseChar = 'a'>
struct PersistentTrieAM {
    static constexpr int ch2i(char ch) {
        return ch - BaseChar;
    }

    static int popcnt(unsigned x) {
#ifndef __GNUC__
        return int(__popcnt(x));
        /*
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        x = (x + (x >> 4)) & 0x0F0F0F0F;
        x = x + (x >> 8);
        x = x + (x >> 16);
        return int(x & 0x0000003F);
        */
#else
        return __builtin_popcount(x);
#endif
    }

    struct Node {
        int leafCount;
        int parent;

        unsigned childSet;
        vector<int> children;

        explicit Node(int parent = -1) {
            init(parent);
        }

        void init(int parent = -1) {
            this->leafCount = 0;
            this->parent = parent;
            this->childSet = 0;
            this->children.clear();
        }

        bool isLeaf() const {
            return leafCount > 0;
        }

        bool isEmpty() const {
            if (leafCount > 0)
                return false;
            return childSet == 0;
        }

        bool hasChild(int chIdx) const {
            return (childSet & (1 << chIdx)) != 0;
        }

        int getChild(int chIdx) const {
            if ((childSet & (1 << chIdx)) == 0)
                return -1;
            int idx = popcnt(childSet & ((1 << chIdx) - 1));
            return children[idx];
        }

        void setChild(int chIdx, int node) {
            int idx = popcnt(childSet & ((1 << chIdx) - 1));
            if ((childSet & (1 << chIdx)) != 0) {
                children[idx] = node;
            } else {
                children.insert(children.begin() + idx, node);
                childSet |= (1 << chIdx);
            }
        }

        void eraseChild(int chIdx) {
            int idx = popcnt(childSet & ((1 << chIdx) - 1));
            if ((childSet & (1 << chIdx)) != 0) {
                children.erase(children.begin() + idx);
                childSet &= ~(1 << chIdx);
            }
        }
    };

    int initRoot;
    vector<Node> nodes;

    PersistentTrieAM() {
        initRoot = 0;
        nodes.emplace_back(-1);
    }

    int init() {
        nodes.clear();
        nodes.emplace_back(-1);
        return initRoot = 0;
    }


    // return (new root, true if it's a new string).
    pair<int, bool> insert(int root, const string& s) {
        return insert(root, s.c_str(), int(s.length()));
    }

    // return (new root, true if it's a new string).
    pair<int, bool> insert(int root, const char* s, int len) {
        if (len <= 0)
            return make_pair(root, false);

        root = cloneNode(root);
        int cur = root;
        for (int i = 0; i < len; i++) {
            int idx = ch2i(s[i]);
            int next = cloneNode(nodes[cur].getChild(idx), cur);
            nodes[cur].setChild(idx, next);
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
            cur = nodes[cur].getChild(idx);
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
            cur = nodes[cur].getChild(idx);
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
