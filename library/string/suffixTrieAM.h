#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

#include "suffixTrie.h"

// Array Mapped Suffix Trie
template <int MaxCharN = 26, int BaseChar = 'a'>
struct SuffixTrieAM {
    static const size_t AllocBlockSize = 128;
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
        Node*           parent;
        Node*           suffixLink;
        int             suffixIndex;

        unsigned        childSet;
        vector<Node*>   children;

        void init(Node* parent = nullptr) {
            this->parent = parent;
            this->suffixLink = nullptr;
            this->suffixIndex = -1;
            this->childSet = 0;
            this->children.clear();
        }

        bool isLeaf() const {
            return childSet == 0;
        }

        bool isEnd() const {
            return suffixIndex >= 0;
        }


        bool hasChild(int chIdx) const {
            return (childSet & (1 << chIdx)) != 0;
        }

        Node* getChild(int chIdx) const {
            if ((childSet & (1 << chIdx)) == 0)
                return nullptr;
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
                //children[idx] = nullptr;
                children.erase(children.begin() + idx);
                childSet &= ~(1 << chIdx);
            }
        }
    };

    Node mRoot;

    string mText;
    Node* mSuffixLink[MaxCharN];

    SuffixTrieAM() {
        mRoot.init();
        mFreeNode = nullptr;
        memset(mSuffixLink, 0, sizeof(mSuffixLink));
    }

    void clear() {
        for (auto* p : mRoot.children) {
            deleteNode(p);
        }
        mRoot.childSet = 0;
        mRoot.children.clear();

        mText.clear();
        memset(mSuffixLink, 0, sizeof(mSuffixLink));
    }


    // build a suffix trie from string 's'
    int build(const char* s, int len, bool finalize = true) {
        if (len <= 0)
            return 0;

        mText.clear();
        memset(mSuffixLink, 0, sizeof(mSuffixLink));

        int added = 0;
        for (int i = 0; i < len; i++)
            added += extendSuffix(s[i]);

        if (finalize)
            setSuffixIndex();

        return added;
    }

    // update suffix indexes.
    // It means there is no update(adding / deleting), because a node with suffix index is a leaf node.
    void setSuffixIndex() {
        if (mText.empty())
            return;

        Node* slink = mSuffixLink[ch2i(mText.back())];
        for (int i = 0; slink != &mRoot; i++) {
            slink->suffixIndex = i;
            slink = slink->suffixLink;
        }
    }

    // reset suffix indexes of current leaf nodes
    void resetSuffixIndex() {
        if (mText.empty())
            return;

        Node* slink = mSuffixLink[ch2i(mText.back())];
        for (int i = 0; slink != &mRoot; i++) {
            slink->suffixIndex = -1;
            slink = slink->suffixLink;
        }
    }


    // add a character into the last of a suffix
    int extendSuffix(char ch) {
        if (mText.empty()) {
            Node* p = allocNode(&mRoot);
            mRoot.setChild(ch2i(ch), p);
            mSuffixLink[ch2i(ch)] = p;

            mText += ch;
            return 1;
        }

        int idx = ch2i(ch);
        Node* slink = mSuffixLink[ch2i(mText.back())];

        Node* p = allocNode(slink);
        mSuffixLink[idx] = p;
        slink->setChild(idx, p);

        int cnt = 1;
        for (slink = slink->suffixLink; slink; slink = slink->suffixLink) {
            if (slink->hasChild(idx)) {
                p->suffixLink = slink->getChild(idx);
                break;
            }

            p = p->suffixLink = allocNode(slink);
            slink->setChild(idx, p);
            cnt++;
        }
        mText += ch;

        return cnt;
    }

    // delete a last character
    int shrinkSuffix() {
        if (mText.empty())
            return 0;

        int cnt = 0;

        int idx = ch2i(mText.back());
        Node* p = mSuffixLink[idx];
        while (p && p != &mRoot && p->childSet == 0) {
            Node* del = p;
            p->parent->eraseChild(idx);
            p = p->suffixLink;

            freeNode(del);

            cnt++;
        }

        mSuffixLink[idx] = (p && p != &mRoot) ? p : nullptr;
        mText.pop_back();

        return cnt;
    }


    // prefix matching
    // return (prefix_matching_length, suffix_index)
    pair<int, int> search(const string& s) const {
        return search(&s[0], int(s.length()));
    }

    // prefix matching
    // return (prefix_matching_length, suffix_index)
    pair<int, int> search(const char* s, int len) const {
        if (len <= 0)
            return make_pair(0, -1);

        Node* p = (Node*)&mRoot;
        for (int i = 0; i < len; i++) {
            int idx = ch2i(s[i]);
            if (!p->hasChild(idx))
                return make_pair(i, -1);
            p = p->getChild(idx);
        }
        return make_pair(len, p->suffixIndex);
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
        p->suffixLink = &mRoot;

        return p;
    }

    void freeNode(Node* node) {
        node->parent = mFreeNode;
        mFreeNode = node;
    }

    void deleteNode(Node* p) {
        if (!p)
            return;

        for (auto* t : p->children) {
            deleteNode(t);
        }

        freeNode(p);
    }
};
