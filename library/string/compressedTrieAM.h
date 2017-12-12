#pragma once

#include "trie.h"

//--- Array Mapped Compressed Trie ---------------------------------------------------------

struct CompressedTrieAM {
    static const size_t MaxCharN = 26;
    static const size_t AllocBlockSize = 128;
    static int ch2i(char ch) {
        return ch - 'a';
    }

    static int popcnt(unsigned x) {
#ifndef __GNUC__
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        x = (x + (x >> 4)) & 0x0F0F0F0F;
        x = x + (x >> 8);
        x = x + (x >> 16);
        return int(x & 0x0000003F);
#else
        return __builtin_popcount(x);
#endif
    }

    struct Node {
        const char*     text;
        int             textLen;
        int             leafCount;
        Node*           parent;

        unsigned        childSet;
        vector<Node*>   children;

        void init(Node* parent = nullptr) {
            this->text = nullptr;
            this->textLen = 0;
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
    };

    Node mRoot;

    CompressedTrieAM() {
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

        int idx = ch2i(s[0]);
        if (!p->hasChild(idx)) {
            auto* t = allocNode(p);
            p->setChild(idx, t);
            p = t;
            p->text = s;
            p->textLen = len;
        } else {
            p = p->getChild(idx);
            for (int i = 0; i < len; ) {
                int j;
                for (j = 0; j < p->textLen && i < len; i++, j++) {
                    if (s[i] != p->text[j])
                        break;
                }

                if (j < p->textLen)
                    p = split(p, j);

                if (i < len) {
                    idx = ch2i(s[i]);
                    if (!p->hasChild(idx)) {
                        // add new node
                        auto* t = allocNode(p);
                        p->setChild(idx, t);
                        p = t;
                        p->text = s + i;
                        p->textLen = len - i;
                        break;
                    } else {
                        p = p->getChild(idx);
                    }
                }
            }
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

        Node* p = mRoot.getChild(ch2i(s[0]));
        for (int i = 0; p; ) {
            for (int j = 0; j < p->textLen; i++, j++) {
                if (i >= len || s[i] != p->text[j])
                    return nullptr;
            }
            if (i >= len)
                break;

            p = p->getChild(ch2i(s[i]));
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

        Node* p = mRoot.getChild(ch2i(s[0]));
        int i = 0;
        while (p) {
            for (int j = 0; j < p->textLen; i++, j++) {
                if (i >= len || s[i] != p->text[j])
                    return make_pair(i, false);
            }
            if (i >= len)
                break;

            p = p->getChild(ch2i(s[i]));
        }

        return make_pair(i, p && p->leafCount > 0);
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
    bool erase(const string& s, bool all = false, bool merge = true) {
        return erase(&s[0], (int)s.length(), all, merge);
    }

    // delete a exactly matched word
    bool erase(const char* s, int len, bool all = false, bool merge = true) {
        if (len <= 0)
            return false;

        Node* p = find(s, len);
        if (!p)
            return false;

        if (all)
            p->leafCount = 0;
        else
            p->leafCount--;

        if (p->isEmpty()) {
            Node* del = p;
            p = p->parent;
            p->setChild(ch2i(del->text[0]), nullptr);
            freeNode(del);

            if (merge && !p->isLeaf() && p != &mRoot) {
                // If there is only one child of parent, the parent is merged to the child.
                int idx = -1, cnt = 0;
                for (int i = 0; i < MaxCharN; i++) {
                    if (p->hasChild(i)) {
                        idx = i;
                        cnt++;
                    }
                }
                if (cnt == 1) {
                    Node* parent = p->parent;
                    Node* child = p->getChild(idx);

                    child->text -= p->textLen;
                    child->textLen += p->textLen;

                    idx = ch2i(p->text[0]);
                    parent->setChild(idx, child);
                    child->parent = parent;

                    freeNode(p);
                }
            }
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

    Node* split(Node* node, int offset) {
        Node* newNode = allocNode(node->parent);
        newNode->text = node->text;
        newNode->textLen = offset;
        newNode->setChild(ch2i(node->text[offset]), node);
        newNode->parent->setChild(ch2i(newNode->text[0]), newNode);

        node->text += offset;
        node->textLen -= offset;
        node->parent = newNode;

        return newNode;
    }
};
