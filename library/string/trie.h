#pragma once

#include "blockAllocator.h"

//--- Trie --------------------------------------------------------------------

template <int R = 26, int BaseChar = 'a'>
struct Trie {
    static const int AllocBlockSizeBit = 10;
    static constexpr int toIndex(char ch) {
        return ch - BaseChar;
    }

    struct Node {
        bool            terminal;
        unsigned char   childrenCount;
        Node*           children[R];

        Node() {
            init();
        }

        void init() {
            terminal = false;
            childrenCount = 0;
            memset(children, 0, sizeof(children));
        }

        bool isLeaf() const {
            return childrenCount == 0;
        }

        int countChildren() const {
            return static_cast<int>(childrenCount);
        }

        void setChild(int index, Node* child) {
            if (!children[index])
                childrenCount++;
            children[index] = child;
        }

        void resetChild(int index) {
            if (children[index])
                childrenCount--;
            children[index] = nullptr;
        }
    };

    Node root;

    Trie() {
        freeNodeList = nullptr;
    }

    void clear() {
        for (int i = 0; i < R; i++) {
            if (root.children[i]) {
                deleteNode(root.children[i]);
                root.children[i] = nullptr;
            }
        }
    }

    // return true if it's a new string
    bool insert(const string& s) {
        if (s.empty())
            return false;

        Node* curr = &root;
        for (int i = 0; i < int(s.length()); i++) {
            int next = toIndex(s[i]);
            if (!curr->children[next])
                curr->setChild(next, allocNode());
            curr = curr->children[next];
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
            if (!curr->children[next])
                break;
            curr = curr->children[next];
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
        node->children[0] = freeNodeList;
        freeNodeList = node;
    }

    void deleteNode(Node* p) {
        if (!p)
            return;

        for (int i = 0; i < R; i++) {
            if (p->children[i])
                deleteNode(p->children[i]);
        }
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
        Node* nextNode = node->children[next];
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
