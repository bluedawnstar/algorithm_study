#pragma once

#if defined(_MSC_VER)
#include <intrin.h>
#endif
#include <immintrin.h>

#include "blockAllocator.h"

//--- Array Mapped Trie -------------------------------------------------------

template <int BaseChar = 'a'>
struct TrieAM {
    static const int AllocBlockSizeBit = 10;
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
        bool            terminal;
        unsigned        childSet;
        vector<Node*>   children;

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

        Node* getChild(int index) const {
            if ((childSet & (1u << index)) == 0)
                return nullptr;
            int idx = popcnt(childSet & ((1u << index) - 1u));
            return children[idx];
        }

        void setChild(int index, Node* node) {
            int idx = popcnt(childSet & ((1u << index) - 1u));
            if ((childSet & (1u << index)) != 0) {
                children[idx] = node;
            } else {
                children.insert(children.begin() + idx, node);
                childSet |= (1u << index);
            }
        }

        void resetChild(int index) {
            int idx = popcnt(childSet & ((1 << index) - 1u));
            if ((childSet & (1u << index)) != 0) {
                children.erase(children.begin() + idx);
                childSet &= ~(1u << index);
            }
        }
    };

    Node root;

    TrieAM() {
        freeNodeList = nullptr;
    }

    void clear() {
        for (auto* p : root.children)
            deleteNode(p);
        root.init();
    }

    // return true if it's a new string
    bool insert(const string& s) {
        if (s.empty())
            return false;

        Node* curr = &root;
        for (int i = 0; i < int(s.length()); i++) {
            int next = toIndex(s[i]);
            auto* nextNode = curr->getChild(next);
            if (!nextNode) {
                nextNode = allocNode();
                curr->setChild(next, nextNode);
            }
            curr = nextNode;
        }

        if (curr->terminal)
            return false;

        curr->terminal = true;
        return true;
    }

    // delete a exactly matched word
    bool erase(const string& s) {
        if (s.empty())
            return false;
        return erase(&root, s, 0);
    }

    // return exactly matched word
    Node* find(const string& s) const {
        auto res = findLongestPrefix(s);
        if (res.first != int(s.length()))
            return nullptr;
        return res.second->terminal ? res.second : nullptr;
    }

    // prefix matching
    // return (prefix_matching_length, node_pointer)
    pair<int, Node*> findLongestPrefix(const string& s) const {
        Node* curr = const_cast<Node*>(&root);
        if (s.empty())
            return make_pair(0, curr);

        int i;
        for (i = 0; i < int(s.length()); i++) {
            int next = toIndex(s[i]);
            auto* nextNode = curr->getChild(next);
            if (!nextNode)
                break;
            curr = nextNode;
        }
        return make_pair(i, curr);
    }

private:
    BlockAllocator<Node, AllocBlockSizeBit> allocator;
    Node* freeNodeList = nullptr;

    Node* allocNode() {
        Node* p = nullptr;
        if (freeNodeList) {
            p = freeNodeList;
            freeNodeList = freeNodeList->children[0];
            p->init();
        } else {
            p = allocator.alloc();
            p->init();
        }

        return p;
    }

    void freeNode(Node* node) {
        node->children.clear();
        node->children.push_back(freeNodeList);
        freeNodeList = node;
    }

    void deleteNode(Node* p) {
        if (!p)
            return;

        for (auto* t : p->children)
            deleteNode(t);
        freeNode(p);
    }

    //---

    // return (removed, free_node)
    bool erase(Node* node, const string& s, int index) {
        if (index >= int(s.length())) {
            if (!node->terminal)
                return false;
            node->terminal = false;
            return true;
        }

        int next = toIndex(s[index]);
        auto* nextNode = node->getChild(next);
        if (!nextNode)
            return false;

        auto res = erase(nextNode, s, index + 1);
        if (res && !nextNode->terminal && nextNode->isLeaf()) {
            node->resetChild(next);
            freeNode(nextNode);
        }
        return res;
    }
};
