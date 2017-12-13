#pragma once

#include "trie.h"

//--- Array Mapped Trie -------------------------------------------------------

struct TrieAM {
    static const size_t MaxCharN = 26;
    static const size_t AllocBlockSize = 128;
    static int ch2i(char ch) {
        return ch - 'a';
    }

    static int popcnt(unsigned x) {
#ifndef __GNUC__
        return (int)__popcnt(x);
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
        int             leafCount;
        Node*           parent;

        unsigned        childSet;
        vector<Node*>   children;

        void init(Node* parent = nullptr) {
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

        Node* getChild(int chIdx) const {
            if ((childSet & (1 << chIdx)) == 0)
                return 0;
            int idx = popcnt(childSet & ((1 << chIdx) - 1));
            return children[idx];
        }

        void setChild(int chIdx, Node* node) {
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
                children[idx] = nullptr;
                childSet &= ~(1 << chIdx);
            }
        }
    };

    Node mRoot;

    TrieAM() {
        mRoot.init();
        mFreeNode = nullptr;
    }

    void clear() {
        for (auto* p : mRoot.children) {
            deleteNode(p);
        }
    }

    // return true if it's a new string.
    bool insert(const string& s) {
        return insert(&s[0], (int)s.length());
    }

    // return true if it's a new string.
    bool insert(const char* s, int len) {
        if (len <= 0)
            return false;

        Node* p = &mRoot;
        for (int i = 0; i < len; i++) {
            int idx = ch2i(s[i]);
            if (!p->hasChild(idx))
                p->setChild(idx, allocNode(p));
            p = p->getChild(idx);
        }
        return ++p->leafCount == 1;
    }

    // return exactly matched word
    Node* find(const string& s) const {
        return find(&s[0], (int)s.length());
    }

    // return exactly matched word
    Node* find(const char* s, int len) const {
        if (len <= 0)
            return nullptr;

        Node* p = (Node*)&mRoot;
        for (int i = 0; i < len && p; i++) {
            int idx = ch2i(s[i]);
            p = p->getChild(idx);
        }
        return (p && p->leafCount > 0) ? p : nullptr;
    }

    // prefix matching
    // return (prefix_matching_length, word_matched?)
    pair<int, bool> search(const string& s) const {
        return move(search(&s[0], (int)s.length()));
    }

    // prefix matching
    // return (prefix_matching_length, word_matched?)
    pair<int, bool> search(const char* s, int len) const {
        if (len <= 0)
            return make_pair(0, false);

        Node* p = (Node*)&mRoot;
        for (int i = 0; i < len; i++) {
            int idx = ch2i(s[i]);
            p = p->getChild(idx);
            if (!p)
                return make_pair(i, false);
        }
        return make_pair(len, p->isLeaf());
    }

    // remove a exactly matched word, but the real node is not deleted
    bool remove(const string& s) {
        return remove(&s[0], (int)s.length());
    }

    // remove a exactly matched word, but the real node is not deleted
    bool remove(const char* s, int len) {
        Node* p = find(s, len);
        if (!p)
            return false;

        p->leafCount--;

        return true;
    }

    // delete a exactly matched word
    bool erase(const string& s, bool all = false) {
        return erase(&s[0], (int)s.length(), all);
    }

    // delete a exactly matched word
    bool erase(const char* s, int len, bool all = false) {
        if (len <= 0)
            return false;

        Node* p = find(s, len);
        if (!p)
            return false;

        if (all)
            p->leafCount = 0;
        else
            p->leafCount--;

        for (int i = len - 1; i >= 0 && p != &mRoot && p->isEmpty(); i--) {
            Node* del = p;
            p = p->parent;
            p->eraseChild(ch2i(s[i]));
            freeNode(del);
        }

        return true;
    }

private:
    Allocator<Node, AllocBlockSize> mAlloc;
    Node* mFreeNode;

    Node* allocNode(Node* parent = nullptr) {
        Node* p = nullptr;
        if (mFreeNode) {
            p = mFreeNode;
            mFreeNode = mFreeNode->parent;
            p->init(parent);
        } else {
            p = mAlloc.alloc();
            p->init(parent);
        }

        return p;
    }

    void freeNode(Node* node) {
        node->parent = mFreeNode;
        mFreeNode = node;
    }

    void deleteNode(Node* p) {
        if (!p)
            return;

        for (auto* t : p->children)
            deleteNode(t);
        freeNode(p);
    }
};
