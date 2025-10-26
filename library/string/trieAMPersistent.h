#pragma once

#if defined(_MSC_VER)
#include <intrin.h>
#endif
#include <immintrin.h>

template <int BaseChar = 'a'>
struct PersistentTrieAM {
    static constexpr int toIndex(char ch) {
        return ch - BaseChar;
    }

    static int popcnt(unsigned x) {
#if defined(_MSC_VER)
        return int(__popcnt(x));
#elif defined(__GNUC__)
        return __builtin_popcount(x);
#else
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        x = (x + (x >> 4)) & 0x0F0F0F0F;
        x = x + (x >> 8);
        x = x + (x >> 16);
        return int(x & 0x0000003F);
#endif
    }

    struct Node {
        bool        terminal;
        unsigned    childSet;
        vector<int> children;

        Node() {
            init();
        }

        void init() {
            terminal = false;
            childSet = 0;
            children.clear();
        }

        bool isLeaf() const {
            return childSet == 0;
        }

        bool hasChild(int index) const {
            return (childSet & (1u << index)) != 0;
        }

        int getChild(int index) const {
            if ((childSet & (1u << index)) == 0)
                return -1;
            int idx = popcnt(childSet & ((1u << index) - 1u));
            return children[idx];
        }

        void setChild(int index, int node) {
            int idx = popcnt(childSet & ((1u << index) - 1u));
            if ((childSet & (1 << index)) != 0) {
                children[idx] = node;
            } else {
                children.insert(children.begin() + idx, node);
                childSet |= (1 << index);
            }
        }
    };

    int initRoot;
    vector<Node> nodes;

    PersistentTrieAM() {
        initRoot = 0;
        nodes.emplace_back(Node{});
    }

    int init() {
        nodes.clear();
        nodes.emplace_back(Node{});
        return initRoot = 0;
    }

    // return (new root, true if it's a new string)
    int insert(int root, const string& s) {
        if (s.empty())
            return root;

        root = cloneNode(root);
        int cur = root;
        for (int i = 0; i < int(s.length()); i++) {
            int next = toIndex(s[i]);
            int nextNode = cloneNode(nodes[cur].getChild(next));
            nodes[cur].setChild(next, nextNode);
            cur = nextNode;
        }
        nodes[cur].terminal = true;
        return root;
    }

    // return exactly matched word
    int find(int root, const string& s) const {
        auto res = findLongestPrefix(root, s);
        if (res.first != int(s.length()))
            return -1;
        return nodes[res.second].terminal ? res.second : -1;
    }

    // prefix matching
    // return (prefix_matching_length, node_id)
    pair<int, int> findLongestPrefix(int root, const string& s) const {
        int cur = root;
        if (s.empty())
            return make_pair(0, cur);

        int i;
        for (i = 0; i < int(s.length()); i++) {
            int next = toIndex(s[i]);
            int nextNode = nodes[cur].getChild(next);
            if (nextNode < 0)
                break;
            cur = nextNode;
        }
        return make_pair(i, cur);
    }

private:
    int allocNode() {
        int res = int(nodes.size());
        nodes.emplace_back(Node{});
        return res;
    }

    int cloneNode(int src) {
        int res = int(nodes.size());
        nodes.resize(nodes.size() + 1);
        if (src >= 0)
            nodes.back() = nodes[src];
        return res;
    }
};
